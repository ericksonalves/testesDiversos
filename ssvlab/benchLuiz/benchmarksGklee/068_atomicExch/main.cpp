 
 

#include <cuda.h>
#include <stdio.h>

#define N 2  

__global__ void definitions (int* A, unsigned int* B, unsigned long long int* C, float* D)
{
	atomicExch(A,10);

	atomicExch(B,100);

	atomicExch(C,20);

	atomicExch(D,200.0);
}

int main (){

	int a = 5;
	int *dev_a;

	cudaMalloc ((void**) &dev_a, sizeof(int));

	cudaMemcpy(dev_a, &a, sizeof(int),cudaMemcpyHostToDevice);

	unsigned int b = 5;
	unsigned int *dev_b;

	cudaMalloc ((void**) &dev_b, sizeof(unsigned int));

	cudaMemcpy(dev_b, &b, sizeof(unsigned int),cudaMemcpyHostToDevice);

	unsigned long long int c = 5;
	unsigned long long int *dev_c;

	cudaMalloc ((void**) &dev_c, sizeof(unsigned long long int));

	cudaMemcpy(dev_c, &c, sizeof(unsigned long long int),cudaMemcpyHostToDevice);

	float d = 5;
	float *dev_d;

	cudaMalloc ((void**) &dev_d, sizeof(float));

	cudaMemcpy(dev_d, &d, sizeof(float),cudaMemcpyHostToDevice);
{	__set_CUDAConfig(1, N); 
          
	definitions (dev_a,dev_b,dev_c,dev_d);}
          
	 

	cudaMemcpy(&a,dev_a,sizeof(int),cudaMemcpyDeviceToHost);
	cudaMemcpy(&b,dev_b,sizeof(unsigned int),cudaMemcpyDeviceToHost);
	cudaMemcpy(&c,dev_c,sizeof(unsigned long long int),cudaMemcpyDeviceToHost);
	cudaMemcpy(&d,dev_d,sizeof(float),cudaMemcpyDeviceToHost);

	 

	assert(a==10);
	assert(b==100);
	assert(c==20);

	cudaFree(dev_a);
	cudaFree(dev_b);
	cudaFree(dev_c);
	cudaFree(dev_d);
	return 0;

}
