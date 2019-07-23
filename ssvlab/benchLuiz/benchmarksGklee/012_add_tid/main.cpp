 
 

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <cuda.h>
#include <cuda_runtime_api.h>
#include <sm_atomic_functions.h>

#define N 2

__global__ void race_test (unsigned int* i, int* A)
{
	int tid = threadIdx.x;
	int j = atomicAdd(i,tid);
	A[j] = tid;
}

int main(){

	unsigned int *i;
	int *A;
	unsigned int *dev_i;
	int *dev_A;

	A = (int*)malloc(N*sizeof(int));

	for (int t = 0; t < N; ++t){
		A[t] = 0;
		printf(" %d  ", A[t]);
	}

	i = (unsigned int*)malloc(sizeof(unsigned int));
	
	*i = 0;

	cudaMalloc((void**)&dev_A, N*sizeof(int));
	cudaMalloc((void**)&dev_i, sizeof(unsigned int));

	cudaMemcpy(dev_A, A, N*sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_i, i, sizeof(unsigned int), cudaMemcpyHostToDevice);
{	__set_CUDAConfig(1, N); 
          
	race_test(dev_i, dev_A);}
          
		 
		
	cudaMemcpy(A, dev_A, N*sizeof(int), cudaMemcpyDeviceToHost);

 	
	int tid = 0;
	for (int t=0; t<N;){
		printf ("A[%d]=%d ", t, A[t]);
		 
		assert(A[t] == 0 || A[t] == 1);  
		tid++;
		t = t + tid;
	}

	free(A);
	free(i);
	cudaFree(dev_A);
	cudaFree(dev_i);

	return 0;
}
