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
__global__ void readcmp(char* a, char* b, char* result,
                            unsigned long nthreads, unsigned int str_len, 
		                    unsigned int vicinity, unsigned int tstride);
__global__ void reduce(char *g_idata, char *g_odata, unsigned long nthreads, 
                            unsigned int str_len, unsigned int pop_thresh);
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
 * Arguments:
 *      a - a pointer to one base
 *      b - a pointer to the other base
 *
 * Description:
 *      TODO...
 */

__global__ void readcmp(char *a, char *b, char *result, unsigned long nthreads, unsigned int str_len, unsigned int vicinity, unsigned int tstride) {

	extern __shared__ char sdata[];
	// Find index of this thread
	unsigned long x = threadIdx.x + blockIdx.x * blockDim.x;
	unsigned long y = threadIdx.y + blockIdx.y * blockDim.y;
	unsigned long tid = x + y * blockDim.x * gridDim.x;
	unsigned long i = threadIdx.x; //local block tid

    int j, k;

	while(tid < nthreads) {
		//result[tid] = a[tid+PAD_AMT] ^ b[tid+PAD_AMT];
		result[tid] = a[tid+PAD_AMT];
		/*__syncthreads();

        if (result[tid]!=0) {
		    for (j=1; j<=vicinity; j++) {
			    if(result[tid+j]!=0)
                    break;
            }
            for (k=1; k<j; k++) {
                result[tid+k]=0xff;
            }
		}*/

		//sdata[i] = a[tid+PAD_AMT] ^ b[tid+PAD_AMT-1];
        //__syncthreads();

        /*if (sdata[i] != 0) {
            for (j=1; j<=vicinity; j++) {
                if(sdata[i+j]!=0)
                    break;
            }
            for (k=1; k<j; k++) {
                sdata[i+k]=0xff;
            }
        }*/

        /*result[tid] &= sdata[i];
        __syncthreads();
		sdata[i] = b[tid+PAD_AMT] ^ a[tid+PAD_AMT-1];
        __syncthreads();*/

        /*if (sdata[i]!=0) {
            for (j=1; j<=vicinity; j++) {
                if(sdata[i+j]!=0)
                    break;
            }
            for (k=1; k<j; k++) {
                sdata[i+k]=0xff;
            }
        }*/

        //result[tid] &= sdata[i];
        //__syncthreads();
    	//result[tid] = (result[tid]==0)?0:1;
		
        tid += tstride;
    }
}

__global__ void reduce(char *g_idata, char *g_odata, unsigned long nthreads, unsigned int str_len, unsigned int pop_thresh) {
	extern __shared__ char sdata[];
	// each thread loads one element from global to shared mem
	unsigned long x = threadIdx.x + blockIdx.x * blockDim.x;
    unsigned long y = threadIdx.y + blockIdx.y * blockDim.y;
    unsigned long i = x + y * blockDim.x * gridDim.x; //global tid

	unsigned int tid = threadIdx.x; //local block tid

    if(i < nthreads) {
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
	    }
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
 *      TODO...
 */

int main(int argc, char *argv[]) {

    /* check the number of command line arguments */
    if(argc != 7) {
        usage();
        return 1;
    }

    /* get arguments */
    char* file_1;
    char* file_2;
    unsigned int num_genomes, genome_len, buffed_len, 
                 buffer_len, vicinity, errors, pop_thresh;

    file_1 = argv[1];
    file_2 = argv[2];    
    num_genomes = atoi(argv[3]);
    genome_len = atoi(argv[4]);
    errors = atoi(argv[5]);
    vicinity = atoi(argv[6]);

    /* calculate important values */
    pop_thresh = (vicinity-1)*(errors-1) + errors; //opocount threshold
    buffed_len = next_power_2(genome_len); //genome length + buffer space
    buffer_len = buffed_len - genome_len; //difference bw genome len and buf len
    unsigned long num_chars = num_genomes*buffed_len;

    /* initialize and allocate strings to compare */
    char* genome_1_data; //first genome data
    char* genome_2_data; //second genome data
    char* result_data;   //xor result data
    char* reduce_data;   //sum of "errors" in each string
    if(init_data_pad(&genome_1_data, num_chars))  return 1;
    if(init_data_pad(&genome_2_data, num_chars))  return 1;
    if(init_data(&result_data, num_chars))  return 1;
    if(init_data(&reduce_data, num_genomes)) return 1;

    /* read in the data */
    if(read_data(genome_1_data, file_1, num_genomes, genome_len, buffer_len, buffed_len, vicinity))  return 1;
    if(read_data(genome_2_data, file_2, num_genomes, genome_len, buffer_len, buffed_len, vicinity))  return 1;

	/* create timing events */
    //clock_t start_c, end_c;
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

    /* initialize and allocate memoer for GPU input and output arrays */
    char *dev_genome_1_data;
    char *dev_genome_2_data;
    char *dev_result_data;
    char *dev_reduce_data;
	gpuErrchk( cudaMalloc((void**)&dev_genome_1_data, (num_chars+PAD_AMT)*sizeof(char)));
	gpuErrchk( cudaMalloc((void**)&dev_genome_2_data, (num_chars+PAD_AMT)*sizeof(char)));
	gpuErrchk( cudaMalloc((void**)&dev_result_data, num_chars*sizeof(char) ));
	gpuErrchk( cudaMalloc((void**)&dev_reduce_data, num_genomes*sizeof(char) ));

	/* set start time */
	cudaEventRecord(start, 0);
    //start_c = clock(); //begin timing computation

    /* copy data to GPU */	
	gpuErrchk(cudaMemcpy( dev_genome_1_data, genome_1_data, 
        (num_genomes*genome_len+PAD_AMT)*sizeof(char), cudaMemcpyHostToDevice ));
	gpuErrchk(cudaMemcpy( dev_genome_2_data, genome_2_data, 
        (num_genomes*genome_len+PAD_AMT)*sizeof(char), cudaMemcpyHostToDevice ));	

    /* figure out thread count and dimensions for GPU */
    unsigned int num_blocks_x = 256;
    unsigned int num_blocks_y = 1;
    unsigned int threads_per_block = buffed_len;
    unsigned int tstride = threads_per_block*num_blocks_x*num_blocks_y;
    dim3 grid_size(num_blocks_x, num_blocks_y, 1);
    unsigned int log_len = log_2(buffed_len); //TODO: do we need this??

    printf("Number of threads per block: %d\n", threads_per_block);
    printf("Grid Dim X: %d\n", num_blocks_x);
    printf("Grid Dim Y:: %d\n", num_blocks_y);
    printf("num_chars: %lu\n", num_chars);

	/* set start time */
	//cudaEventRecord(start,0);
    //start_c = clock(); //begin timing computation


      /******************/
     /** START TIMING **/
    /******************/

    /*========================================================================*/


	/* create and run GPU threads */
	readcmp<<<grid_size,threads_per_block,threads_per_block>>>(dev_genome_1_data, dev_genome_2_data, dev_result_data, num_chars, buffed_len, vicinity, tstride);
    gpuErrchk(cudaThreadSynchronize());
	//reduce<<<grid_size,THREADS_PER_BLOCK,THREADS_PER_BLOCK>>>(dev_result_data, dev_reduce_data, num_chars, buffed_len, pop_thresh);
    //gpuErrchk(cudaThreadSynchronize());


    /*========================================================================*/

    /****************/
    /** END TIMING **/
    /****************/


	/* set stop time */
	//cudaEventRecord(stop,0);
	//cudaEventSynchronize( stop );	
    //end_c = clock(); //end timing computation
	gpuErrchk(cudaMemcpy( reduce_data, dev_reduce_data, 
	            num_genomes*sizeof(char), cudaMemcpyDeviceToHost ));
	gpuErrchk(cudaMemcpy( result_data, dev_result_data,
                num_chars*sizeof(char), cudaMemcpyDeviceToHost ));

    printf("\n---------------------------------------------------------------\n");
    for(unsigned int i = 80000256-1024; i < 80000256; i ++) {
        printf("%d: %c %c %d\n", i, genome_1_data[i+5], result_data[i], (genome_1_data[i+5] != result_data[i])?1:0);
    }

	cudaEventRecord(stop,0);
	cudaEventSynchronize( stop );
    //end_c = clock(); //end timing computation

	/* calculate elapsed time for GPU computation */
    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop);
	printf("Time to complete comparison %1.4f ms\n", elapsedTime);
    //printf("Total time of computation: %f\n", (end_c-start_c)/(double)CLOCKS_PER_SEC);
	
	/* print out result_data to check */
	/*for (int i = 0; i < num_chars; i++) {
                printf("%d",*(result_data+i));
                printf("\n");
        }
	printf("\n");*/

    /* print out reduce_data to check */
	//for (int i = 0; i < 2100; i++) {
	//	printf("%d",*(reduce_data+i));
    //    	printf("\n");
    //    }

    /* free and destroy all allocated information */
	cudaFree(dev_genome_1_data);
	cudaFree(dev_genome_2_data);
	cudaFree(dev_result_data);
	cudaFree(dev_reduce_data);

	cudaEventDestroy(start);
	cudaEventDestroy(stop);

    //cudaFreeHost(genome_1_data);
    //cudaFreeHost(genome_2_data);
    //cudaFreeHost(result_data);
    //cudaFreeHost(reduce_data);
    free(genome_1_data);
    free(genome_2_data);
    free(result_data);
    free(reduce_data);
}

/*
 * Function - usage
 *
 * Description:
 *      Just prints the usage invariant for this program.
 */
void usage(void) {
    printf("\nUsage:\n");
    printf("\t./a.out <file_1> <file_2> <num_genomes> <genome_len> <errors> <vicinity>\n\n");
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
    *data = (char*)malloc(num_chars * sizeof(char));
    //cudaHostAlloc((void**)data,num_chars*sizeof(char),cudaHostAllocDefault);
    if(NULL == *data) {
        printf("init_data - malloc failed\n");
        return 1;
    }

    return 0; //SUCCESS
}


/*
 * Function - init_data_pad TODO
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
int init_data_pad(char** data, unsigned int num_chars) {
    
    /* allocate pointers for the genome strings */
    *data = (char*)malloc((num_chars + PAD_AMT) * sizeof(char));
    //cudaHostAlloc((void**)data,(num_chars+PAD_AMT)*sizeof(char),cudaHostAllocDefault);
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

        //printf("%d\n",i);
        if(NULL != fgets(buf, genome_len + 2, ifp)) {
            
            for(int j = 0; j < limit_len; j++) {
                *(data + PAD_AMT + i*buffed_len + j) = buf[j];
            }

            for(int j = 0; j < buffed_len-limit_len; j++) {
                *(data + PAD_AMT + i*buffed_len + limit_len + j) = BUFFER_CHAR;
            }

            //buf[genome_len] = '\0';
            //printf("%s\n", buf);
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
