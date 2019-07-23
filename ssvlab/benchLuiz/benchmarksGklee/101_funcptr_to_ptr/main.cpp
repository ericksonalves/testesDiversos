 
 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cuda.h>

#define N 8

typedef float(*funcType)(float*, unsigned int);

__device__ float multiplyByTwo(float *v, unsigned int tid)
{
    return v[tid] * 2.0f;
}

__device__ float divideByTwo(float *v, unsigned int tid)
{
    return v[tid] * 0.5f;
}

 

	__device__ funcType p_mul_func = multiplyByTwo;

	__device__ funcType p_div_func = divideByTwo;


__global__ void foo(float *v, funcType f, unsigned int size, int i)
{
    assert(f == divideByTwo);
	assert(i != 0);

	uint tid = blockIdx.x * blockDim.x + threadIdx.x;

    void *x = (void*)f;  

    if (i == 0)
      x = NULL;

    funcType g = (funcType)x;

    if (tid < size)
    {
        v[tid] = (*g)(v, tid);
    }
}

int main(){
	float* w;
	float* dev_w;

	int size = N*sizeof(float);

	w =(float*) malloc(size);

	for (int i = 0; i < N; ++i){
		w[i] = i;
	}

	cudaMalloc((void**)&dev_w, size);

	cudaMemcpy(dev_w,w, size,cudaMemcpyHostToDevice);

	funcType host_f;

	cudaMemcpyFromSymbol( &host_f, p_div_func, sizeof( funcType ) );

	funcType dev_f = host_f;
{	__set_CUDAConfig(1, N); 
          
	foo (dev_w, dev_f, N, 1);}
          

	cudaThreadSynchronize();

	cudaMemcpy(w,dev_w,size,cudaMemcpyDeviceToHost);

	printf("\nw:");
	for (int i = 0; i < N; ++i){
		printf(" %f	",	w[i]);
	}

	free(w);

	return EXIT_SUCCESS;
}
