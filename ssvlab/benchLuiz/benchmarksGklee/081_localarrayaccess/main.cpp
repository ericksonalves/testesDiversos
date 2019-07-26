 
 

#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <assert.h>

#define N 2

__global__ void foo() {

  __shared__ int A[10];

  A[threadIdx.x] = 2;

  __syncthreads ();  

  int x = A[threadIdx.x + 1];

}

int main(){
{	__set_CUDAConfig(1, N); 
          
	foo();}
          
	 

	cudaThreadSynchronize();

}