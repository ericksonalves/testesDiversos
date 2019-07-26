#include <stdio.h>
#include <stdlib.h>
#include "cuda.h"
#include <assert.h>

#define N 2 

__device__ int bar(float* A) {

  if(threadIdx.x != 0) {
	return 0;
  }

  return 1;

}

__global__ void foo(float* A) {

  int y = bar(A);
  A[threadIdx.x]=y;

}

int main(void){
	int i;
	float *A;
	float *dev_A;
	float size= N*sizeof(float);
	A=(float*)malloc(size);

	for(i=0;i<N;i++)
		A[i]=2;
			
	cudaMalloc((void**)&dev_A,size);

	cudaMemcpy(dev_A, A,size, cudaMemcpyHostToDevice);
{	__set_CUDAConfig(1, N); 
          
	foo(dev_A);}
          
	 

	cudaMemcpy(A,dev_A,size,cudaMemcpyDeviceToHost);

	for(i=0;i<N;i++){
 
		if (i!=0)
			assert(A[i]!=0);
		
	}
	cudaFree(dev_A);
	free(A);
	return 0;
}