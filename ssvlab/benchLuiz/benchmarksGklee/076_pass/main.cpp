#include <stdio.h>
#include <stdlib.h>
#include "cuda.h"
#include <assert.h>

#define N 2 

__global__ void foo(float* A) {

  if(blockIdx.x == 0)
	  A[threadIdx.x] = 50.f;
}

int main() {
	float *a = 0;
	float *dev_a;

	a = (float*)malloc(N*sizeof(float));

	cudaMalloc((void**)&dev_a, N*sizeof(float));

	cudaMemcpy(dev_a, a, N*sizeof(float), cudaMemcpyHostToDevice);
{	__set_CUDAConfig(1, N); 
          
	foo(dev_a);}
          
		 

	cudaMemcpy(a, dev_a, N*sizeof(float), cudaMemcpyDeviceToHost);

	printf ("%.1f", *a);

		assert(*a == 50.0);

	free(a);
	cudaFree(dev_a);

	return 0;
}
