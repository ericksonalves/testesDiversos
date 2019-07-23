#include <stdio.h>
#include <assert.h>
#include "cuda.h"

#define N 2

__global__ void foo(int* p) {
    p[threadIdx.x] = 2;
    __syncthreads();
}

int main(){

	int *a;
	int *dev_a;
	int size = N*sizeof(int);

	cudaMalloc((void**)&dev_a, size);

	a = (int*)malloc(N*size);

	for (int i = 0; i < N; i++)
		a[i] = 1;

	cudaMemcpy(dev_a,a,size, cudaMemcpyHostToDevice);
{	__set_CUDAConfig(1, N); 
          
	foo(dev_a);}
          
	 

	cudaMemcpy(a,dev_a,size,cudaMemcpyDeviceToHost);

	for (int i = 0; i < N; i++){
		assert(a[i]==2);
	}

	free(a);

	cudaFree(dev_a);

	return 0;
}
