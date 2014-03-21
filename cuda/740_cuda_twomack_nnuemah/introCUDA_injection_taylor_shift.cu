#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <cuda.h>


/*********************************/
/** constants/define statements **/
/*********************************/

#define THREADS_PER_BLOCK 1024
#define MAX_BLOCKS 65535
#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
#define BUFFER_CHAR 'A'
#define PAD_AMT 5


/**********************/
/** function headers **/
/**********************/

void usage(void);
int init_data(char** data, unsigned int num_chars);
int init_data_pad(char** data, unsigned int num_chars);
int read_data(char* data, char* file, unsigned int num_genomes,
                unsigned int genome_len, unsigned int buffer_len,
                unsigned int buffed_len, unsigned int vicinity);
void free_data(char* data, unsigned int num_genomes);
__global__ void readcmp(char* a, char* b, /*char* result,*/
                            unsigned long nthreads, unsigned int str_len, 
                            unsigned int vicinity, unsigned int tstride, 
                            char *reduce, unsigned int pop_thresh, 
                            unsigned int threads_per_block, 
                            unsigned int shift_amt);
__global__ void reduce(char *g_idata, char *g_odata, unsigned long nthreads, 
                            unsigned int str_len, unsigned int pop_thresh,
                            unsigned int tstride);
void print_device_info(void);
unsigned int next_power_2(unsigned int v);
unsigned int log_2(unsigned int v);


/***************/
/** functions **/
/***************/

/*
 * Function - gpuAssert
 *
 * Inputs:
 *      code - gpu error code
 *      file - current source file 
 *      line - line within this file
 *      abort - if true, the prgram aborts
 *
 * Description:
 *      This function checks the cuda error code, and aborts if it is not a
 *      success.
 */

inline void gpuAssert(cudaError_t code, char *file, int line, bool abort=true)
{
   if (code != cudaSuccess) 
   {
      fprintf(stderr,"GPUassert: %s %s %d\n",cudaGetErrorString(code),file,line);
      if (abort) exit(code);
   }
}


/*
 * Kernel - readcmp
 *
 * Inputs:
 *      a - a pointer to one read
 *      b - a pointer to the other read
 *      nthreads - the maximum number of threads used
 *      str_len - the length of the reads to be compared (this is a power of 2)
 *      vicinity - the bit-flip vicinity
 *      tstride - the read index stride distance for each thread
 *      pop_thresh - the pop count threshold
 *      thread_per_block - the number of threads per block
 *      shift_amt - the maximum shift amount between the reads
 *
 * Outputs:
 *      reduce - the array returned which contains 0 for "reads match" or 1 for 
 *               "reads don't match"
 *
 * Description:
 *      This Kernel does two things: for each pair of reads, it compares the
 *      characters and stores the bit-op results. It then performas a sum
 *      reduction on each read comparison and checks if it is above the edit
 *      threshold.
 */

__global__ void readcmp(char *dev_test_array, char *a, char *b, /*char *result,*/ unsigned long nthreads, unsigned int str_len, unsigned int vicinity, unsigned int tstride, char *reduce, unsigned int pop_thresh, unsigned int threads_per_block, unsigned int shift_amt) {

    // Set up shared memory
    extern __shared__ char shared_data[];
    char* sdata = (char *)shared_data;
    char* result = (char *)&shared_data[threads_per_block];

    // Find index of this thread
    unsigned long x = threadIdx.x + blockIdx.x * blockDim.x;
    unsigned long y = threadIdx.y + blockIdx.y * blockDim.y;
    unsigned long tid = x + y * blockDim.x * gridDim.x;
    unsigned long i = threadIdx.x; //local block tid

    int j, k;

    while(tid < nthreads) {
	
        //make the first xor comparison without shifting
        result[i] = a[tid+PAD_AMT] ^ b[tid+PAD_AMT];
        __syncthreads();

        //check the vicinity for 100...01
        if (result[i]!=0) {
            for (j=1; j<vicinity; j++) {
                if(result[i+j]!=0)
                    break;
            }
            if (result[i+j]!=0) {
                for (k=1; k<j; k++) {
                    result[i+k]=0xff;
                }
            }
        }

        //make the remaining xor comparisons with shifting up until the limit
        for(unsigned int cur_shift = 1; cur_shift <= shift_amt; cur_shift++) {
            __syncthreads();
            sdata[i] = a[tid+PAD_AMT] ^ b[tid+PAD_AMT-cur_shift]; //shift b left
            __syncthreads();

            //check vicinity
            if (sdata[i] != 0) {
                for (j=1; j<vicinity; j++) {
                    if(sdata[i+j]!=0)
                        break;
                }
                if (sdata[i+j]!=0) {
                    for (k=1; k<j; k++) {
                        sdata[i+k]=0xff;
                    }
                }
            }

            //AND result
            __syncthreads();
            result[i] = result[i] && sdata[i];

            sdata[i] = b[tid+PAD_AMT] ^ a[tid+PAD_AMT-cur_shift]; //shift a left
            __syncthreads();

            //check vicinity
            if (sdata[i]!=0) {
                for (j=1; j<vicinity; j++) {
                    if(sdata[i+j]!=0)
                        break;
                }
                if (sdata[i+j]!=0) {
			for (k=1; k<j; k++) {
				sdata[i+k]=0xff;
                    }
                }
            }

            //AND result
            __syncthreads();
            result[i] = result[i] && sdata[i];
            __syncthreads();
        }

        /////////////////////////////////////////////////////////////////////
        // the code below is used for the sum reduce
        /////////////////////////////////////////////////////////////////////
 
        sdata[i] = result[i];
	__syncthreads();
	
	
	/*
	 * conservative reduction implemented by John Emmons Feb. 2014
	 * EX. if vicinity = 3 then 111100110111111 -> 100100110100101
	 */

	
	if(sdata[i] != 0 && (i == 0 || sdata[i-1] == 0)){	
		int m, n = i;
		bool flag = true;
		while(true){
			for(m = 1; m < vicinity + 1; m++){
				if(n + m < str_len) {
					if(sdata[n + m] != 0){ continue; }
					else if(m < 2){ flag = false; break; }
					else{ break; }
				}
				else{
					if(m < 2){ flag = false; break; }
                                        else{ break; }
				}
			}
			if(flag){
				for(m -= 2; m > 0; m--)
					sdata[n + m] = 0x00;
					n += vicinity;
			}
			else{ break; }	
		}
	}
	__syncthreads();
	
	// conservative reduction debugging
		
	dev_test_array[i] = sdata[i];
	__syncthreads();

        // do reduction in shared mem
        for(unsigned int s=str_len/2; s>0; s >>= 1){
            if(i < s) {
                sdata[i] += sdata[i + s];
            }
            __syncthreads();
        }
 
        __syncthreads();

        // write result for this block to global mem
        if(i%str_len == 0) {
            reduce[tid/str_len] = (sdata[i]<=pop_thresh)?0:1;
        }

        ///////////////////////////////////////////////////////////////////////

        __syncthreads();
        tid += tstride; //increase tid by thread stride amount
    }
}


/*********************************/
/** THIS FUNCTION IS DEPRECATED **/
/*********************************/

__global__ void reduce(char *g_idata, char *g_odata, unsigned long nthreads, unsigned int str_len, unsigned int pop_thresh, unsigned int tstride) {
    extern __shared__ char sdata[];
    // each thread loads one element from global to shared mem
    unsigned long x = threadIdx.x + blockIdx.x * blockDim.x;
    unsigned long y = threadIdx.y + blockIdx.y * blockDim.y;
    unsigned long i = x + y * blockDim.x * gridDim.x; //global tid

    unsigned int tid = threadIdx.x; //local block tid

    while(i < nthreads) {
        sdata[tid] = g_idata[i];
        __syncthreads();
        // do reduction in shared mem
        for(unsigned int s=str_len/2; s>0; s >>= 1){
            if (tid<s) {
                sdata[tid] += sdata[tid + s];
            }
            __syncthreads();
        }

        // write result for this block to global mem
        if (tid%str_len == 0) {
            g_odata[i/str_len] = (sdata[tid]<=pop_thresh)?1:0;
            g_odata[i/str_len] = sdata[tid];
        }

        i += tstride;
    }
}


/*
 * Function - main
 *
 * Arguments:
 *      argc - the number of command line arguments
 *      argv - an array of the command line arguments
 *
 * Outputs:
 *      int - 0 if success, 1 if failure
 *
 * Description:
 *      This is the main function. It initializes memory. It reads in the
 *      files which contain the reads. It, then, launches the kernel on
 *      the GPU.
 */

int main(int argc, char *argv[]) {

    /* check the number of command line arguments */
    if(argc != 8) {
        usage();
        return 1;
    }

    /* get arguments */
    char* file_1;
    char* file_2;
    unsigned int num_genomes, genome_len, buffed_len, 
                 buffer_len, vicinity, errors, pop_thresh, shift_amt;
    FILE *pop_count_file;
    file_1 = argv[1]; //contains reads
    file_2 = argv[2]; //contains reads
    num_genomes = atoi(argv[3]); //the number of reads in each file
    genome_len = atoi(argv[4]); //the length of each read
    errors = atoi(argv[5]); //the number of edits allowed between two reads
    vicinity = atoi(argv[6]); //the vicinity for bit flips
    shift_amt = atoi(argv[7]); //the maximum shift amount when comparing reads

    /* calculate important values */
    pop_thresh = /*(vicinity-1)*(errors-1) +*/ errors; //popcount threshold (is simply the num of allowed errors with conservative reduction)
    buffed_len = next_power_2(genome_len); //genome length + buffer space
    buffer_len = buffed_len - genome_len; //difference bw genome len and buf len
    unsigned long num_chars = num_genomes*buffed_len; //the total number of chars in every buffed read

    /* initialize and allocate strings to compare */
    char* genome_1_data; //first genome data
    char* genome_2_data; //second genome data
    char* reduce_data;   //sum of "errors" in each string
    if(init_data_pad(&genome_1_data, num_chars))  return 1;
    if(init_data_pad(&genome_2_data, num_chars))  return 1;
    if(init_data(&reduce_data, num_genomes)) return 1;

    // conservative reduction debugging
    char* test_array;
    if(init_data(&test_array, num_chars+PAD_AMT)) return 1;

    /* read in the data */
    if(read_data(genome_1_data, file_1, num_genomes, genome_len, buffer_len, buffed_len, vicinity+1))  return 1;
    if(read_data(genome_2_data, file_2, num_genomes, genome_len, buffer_len, buffed_len, vicinity+1))  return 1;

    /* create timing events */
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    /* initialize and allocate memoer for GPU input and output arrays */
    char *dev_genome_1_data;
    char *dev_genome_2_data;
    char *dev_reduce_data;
    gpuErrchk( cudaMalloc((void**)&dev_genome_1_data, (num_chars+PAD_AMT)*sizeof(char)));
    gpuErrchk( cudaMalloc((void**)&dev_genome_2_data, (num_chars+PAD_AMT)*sizeof(char)));
    gpuErrchk( cudaMalloc((void**)&dev_reduce_data, num_genomes*sizeof(char) ));

    // conservative reduction debugging
    char *dev_test_array;
    gpuErrchk( cudaMalloc((void**)&dev_test_array, (num_chars+PAD_AMT)*sizeof(char) ));

      /******************/
     /** START TIMING **/
    /******************/

    /*========================================================================*/

    /* set start time */
    cudaEventRecord(start, 0);

    /* copy data to GPU */    
    gpuErrchk(cudaMemcpy( dev_genome_1_data, genome_1_data, 
        (num_chars+PAD_AMT)*sizeof(char), cudaMemcpyHostToDevice ));
    gpuErrchk(cudaMemcpy( dev_genome_2_data, genome_2_data, 
        (num_chars+PAD_AMT)*sizeof(char), cudaMemcpyHostToDevice ));    

    // conservative reduction debugging
    gpuErrchk(cudaMemcpy( dev_test_array, test_array,
        (num_chars+PAD_AMT)*sizeof(char), cudaMemcpyHostToDevice ));

    /* figure out thread count and dimensions for GPU */
    unsigned int num_blocks_x = 128;
    unsigned int num_blocks_y = 128;
    unsigned int threads_per_block = buffed_len;
    unsigned int tstride = threads_per_block*num_blocks_x*num_blocks_y;
    dim3 grid_size(num_blocks_x, num_blocks_y, 1);
    unsigned int log_len = log_2(buffed_len); //TODO: ALL OF THIS SHOULD PROBABLY BE MOVED ABOVE THE BEGINNING OF TIMING

    /* create and run GPU threads */
    readcmp<<<grid_size,threads_per_block,2*threads_per_block>>>(dev_test_array, dev_genome_1_data,
            dev_genome_2_data,/* dev_result_data,*/ num_chars, buffed_len, vicinity,
            tstride, dev_reduce_data, pop_thresh, threads_per_block, shift_amt);
    gpuErrchk(cudaThreadSynchronize());

    /* write the results back */
    gpuErrchk(cudaMemcpy( reduce_data, dev_reduce_data, 
                num_genomes*sizeof(char), cudaMemcpyDeviceToHost ));
    
    // conservative reduction debugging
    gpuErrchk(cudaMemcpy( test_array, dev_test_array,
                (num_chars + PAD_AMT) *sizeof(char), cudaMemcpyDeviceToHost ));

    /*========================================================================*/

    /****************/
    /** END TIMING **/
    /****************/

    /* set stop time */
    cudaEventRecord(stop,0);
    cudaEventSynchronize( stop );

    /* calculate elapsed time for GPU computation */
    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop);
    printf("Time to complete comparison %1.4f ms\n", elapsedTime);

    // Writing output pop count to file
    // for data collection purposes
    unsigned int matches=0;
    for (unsigned int q=0; q<num_genomes; q++) {
        if (reduce_data[q]==0)
            matches++;
            
    }

    // conservative reduction debugging
	int j = 0;
	int bad_strings[num_genomes];
	for(int i=0; i < num_genomes; i++)
		bad_strings[i] = 0;

	// find the bad strings
	for(int i=0; i < num_genomes; i++){
		if (reduce_data[i]!=0){	
			bad_strings[j] = i;
			j++;
		}
	}

	for(int i=0; i < num_genomes; i++){
		if(bad_strings[i] != 0){
			printf("genome number %d is a false negative\n", bad_strings[i]);
			//printf("Original genome:\n");
			for(int k=0; k < genome_len; k++){
				printf("%c", genome_1_data[k + bad_strings[i]]);
				}
			printf("\n");
			//printf("Edited genome:\n");
			for(int k=0; k < genome_len; k++){ 
				printf("%c", genome_2_data[k + bad_strings[i]]);
			}
			printf("\n");

		}
	}
        
//      for(unsigned int i=0; i < num_chars + PAD_AMT; i++){
//	printf("the test_array: %i\n", test_array[i]);
//      }
    

    pop_count_file = fopen("pop_output.txt","w");
    fprintf(pop_count_file, "%d %d\n", matches, num_genomes-matches);
    fclose(pop_count_file);
    
    /* free and destroy all allocated information */
    cudaFree(dev_genome_1_data);
    cudaFree(dev_genome_2_data);
    cudaFree(dev_reduce_data);

    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    cudaFreeHost(genome_1_data);
    cudaFreeHost(genome_2_data);
    cudaFreeHost(reduce_data);
}

/*
 * Function - usage
 *
 * Description:
 *      Just prints the usage invariant for this program.
 */
void usage(void) {
    printf("\nUsage:\n");
    printf("\t./a.out <file_1> <file_2> <num_genomes> <genome_len> <errors> <vicinity> <adj_errs>\n\n");
}


/*
 * Function - init_data
 *
 * Arguments:
 *      data - the array in which to place data
 *      num_chars - the number of chars to allocate
 *
 * Outputs:
 *      int - 0 if success, 1 if failure
 *
 * Description:
 *      This function initializes a data array. Pretty simple to follow.
 */
int init_data(char** data, unsigned int num_chars) {
    
    /* allocate pointers for the genome strings */
    cudaHostAlloc((void**)data,num_chars*sizeof(char),cudaHostAllocDefault);
    if(NULL == *data) {
        printf("init_data - malloc failed\n");
        return 1;
    }

    return 0; //SUCCESS
}


/*
 * Function - init_data_pad
 *
 * Arguments:
 *      data - the array in which to place data
 *      num_chars - the number of chars to allocate
 *
 * Outputs:
 *      int - 0 if success, 1 if failure
 *
 * Description:
 *      This function initializes a data array. Pretty simple to follow. It's
 *      the same as the above function except that it adds the PAD_AMT to
 *      to it.
 */
int init_data_pad(char** data, unsigned int num_chars) {
    
    /* allocate pointers for the genome strings */
    cudaHostAlloc((void**)data,(num_chars+PAD_AMT)*sizeof(char),cudaHostAllocDefault);
    if(NULL == *data) {
        printf("init_data - malloc failed\n");
        return 1;
    }

    return 0; //SUCCESS
}


/*
 * Function - read_data
 *
 * Arguments:
 *      data - the char* to which the data will be written 
 *      file - the filae that contains the genomes we care about
 *      num_genomes - the numebr of genomed to read 
 *      genome_len - the length of the genomes
 *      buffer_len - the length of the buffer at the end of each genome
 *      buffed_len - length of genome + buffer
 *
 * Outputs:
 *      int - 0 if success, 1 if failure
 *
 * Description:
 *      This function reads in all of the genome data from the given genome
 *      file. Each line contains a genome read, and this is read into each
 *      string.
 */
int read_data(char* data, char* file, unsigned int num_genomes, 
                unsigned int genome_len, unsigned int buffer_len,
                unsigned int buffed_len,unsigned int vicinity) {

    /* basic info and variables */
    FILE* ifp; //ifp: "in file pointer"
    char* mode = "r";

    /* open the file */
    ifp = fopen(file, mode);
    if(NULL == ifp) {
        printf("Can't open input file %s!\n", file);
        return 1;
    }

    /* create read in buffer */
    char* buf = (char*)malloc((genome_len+2)*sizeof(char));

    /* initialize the padding at beginning of array */
    for(int i = 0; i < PAD_AMT; i++) {
        *(data + i) = BUFFER_CHAR;
    }
    int limit_len;

    /* calculate the limit to which we will read data */
    if (buffer_len < vicinity)
        limit_len = buffed_len - vicinity;
    else
        limit_len = genome_len;    

    /* read in the file */
    for(int i = 0; i < num_genomes; i++) {

        if(NULL != fgets(buf, genome_len + 2, ifp)) {
            
            for(int j = 0; j < limit_len; j++) {
                *(data + PAD_AMT + i*buffed_len + j) = buf[j];
            }

            for(int j = 0; j < buffed_len-limit_len; j++) {
                *(data + PAD_AMT + i*buffed_len + limit_len + j) = BUFFER_CHAR;
            }

        }
        else {
            printf("Failed to read from the file\n");
            return 1;
        }
    }

    /* close the file */
    fclose(ifp);
    free(buf);

    return 0; //SUCCESS
}


/*
 * Function - print_device_info
 *
 * Description:
 *      Prints valuable information out regarding the CUDA-capable devices
 *      in this system.
 */
void print_device_info(void) {
    cudaDeviceProp  prop;

    int count;
    cudaGetDeviceCount( &count ); 
    for (int i=0; i< count; i++) {
        cudaGetDeviceProperties( &prop, i );
        printf( " --- General Information for device %d ---\n", i ); 
        printf( "Name: %s\n", prop.name );
        printf( "Compute capability: %d.%d\n", prop.major, prop.minor ); 
        printf( "Clock rate: %d\n", prop.clockRate );
        printf( "Device copy overlap: " );
        if (prop.deviceOverlap)
            printf( "Enabled\n" ); 
        else
            printf( "Disabled\n" );
        printf( "Kernel execition timeout : " ); 
        if (prop.kernelExecTimeoutEnabled)
            printf( "Enabled\n" ); 
        else
            printf( "Disabled\n" );
        printf( "   --- Memory Information for device %d ---\n", i );
        printf( "Total global mem:  %ld\n", prop.totalGlobalMem );
        printf( "Total constant Mem:  %ld\n", prop.totalConstMem );
        printf( "Max mem pitch:  %ld\n", prop.memPitch );
        printf( "Texture Alignment:  %ld\n", prop.textureAlignment );
        printf( "   --- MP Information for device %d ---\n", i );
        printf( "Multiprocessor count:  %d\n", prop.multiProcessorCount );
        printf( "Shared mem per mp:  %ld\n", prop.sharedMemPerBlock );
        printf( "Registers per mp:  %d\n", prop.regsPerBlock );
        printf( "Threads in warp:  %d\n", prop.warpSize );
        printf( "Max threads per block:  %d\n", prop.maxThreadsPerBlock );
        printf( "Max thread dimensions:  (%d, %d, %d)\n",
            prop.maxThreadsDim[0], prop.maxThreadsDim[1],
            prop.maxThreadsDim[2] );
        printf( "Max grid dimensions:  (%d, %d, %d)\n",
            prop.maxGridSize[0], prop.maxGridSize[1],
            prop.maxGridSize[2] );
        printf( "\n" );
    }
}


/*
 * Function - next_power_2
 *
 * Arguments:
 *      v - the value for which we want to find the next power of 2
 *
 * Outputs:
 *      unsigned int - the next power of 2 greater than v
 *
 * Description:
 *      This code basically rounds v up to the next highest power of 2. So if
 *      v was 2, this function would return 2. If v was 15, this function would
 *      return 16. Etcetera.
 *
 *  Source:
 *      http://graphics.stanford.edu/~seander/bithacks.html
 */
unsigned int next_power_2(unsigned int v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

unsigned int log_2(unsigned int v) {
    unsigned int r=0;
    while (v >>= 1) // unroll for more speed...
    {
        r++;
    }
    return r;
}
