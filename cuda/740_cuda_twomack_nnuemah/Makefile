TARGET=introCUDA_injection_taylor_shift

all: build
	rm -rf *.o

build:
	"/usr/local/cuda-5.5"/bin/nvcc -ccbin g++ -I../../common/inc -m64 -gencode arch=compute_10,code=sm_10 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=\"sm_35,compute_35\" -o $(TARGET).o -c $(TARGET).cu
	/usr/local/cuda-5.5/bin/nvcc -ccbin g++ -m64 -o inj_taylor_shift.out $(TARGET).o
        
clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf experiments/*.txt

