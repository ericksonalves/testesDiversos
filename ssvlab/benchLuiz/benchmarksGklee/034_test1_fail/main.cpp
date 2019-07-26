 
#include <stdio.h>
#include <cuda.h>
#include <assert.h>
#define N 2 

__global__ void foo(int* p) {

  p[threadIdx.x] = 0;

}

int main() {
	int *c;
	int *dev_c;
	c = (int*)malloc(N*sizeof(int));

	for (int i = 0; i < N; ++i)
		c[i] = rand() %5+1;

	 
	cudaMalloc((void**)&dev_c, N*sizeof(int));

	cudaMemcpy(dev_c, c, N*sizeof(int), cudaMemcpyHostToDevice);
{	__set_CUDAConfig(1, N); 
          
	foo(dev_c);}
          
	 

	cudaMemcpy(c, dev_c, N*sizeof(int), cudaMemcpyDeviceToHost);

	 

	for (int i = 0; i < N; ++i){
	 
			assert(c[i] != 0);
	}
	free(c);
	cudaFree(dev_c);

	   return 0;
}
