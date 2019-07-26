 
 
#include <stdio.h>
#include <stdlib.h>

#define N 8

__device__ float multiplyByTwo(float *v, unsigned int tid) {

    return v[tid] * 2.0f;
}

__device__ float divideByTwo(float *v, unsigned int tid) {

    return v[tid] * 0.5f;
}

typedef float(*funcType)(float*, unsigned int);

__global__ void foo(float *v, funcType* f, unsigned int size)
{
	unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;

	assert(*f == divideByTwo || *f == multiplybyTwo);

    if (tid < size) {
        v[tid] = (*f)(v, tid);
    }
}

int main (){

	float* w;
	float* dev_w;

	int size = N*sizeof(float);

	w =(float*) malloc(size);

	for (int i = 0; i < N; ++i){
		w[i] = i;
	}

	cudaMalloc((void**)&dev_w, size);

	cudaMemcpy(dev_w,w, size,cudaMemcpyHostToDevice);

	funcType* g;
	funcType* dev_g;
	g =(funcType*) malloc(sizeof(funcType));

	 
	 

	cudaMalloc((void**)&dev_g, sizeof(funcType));

	cudaMemcpy(dev_g, g, sizeof(funcType),cudaMemcpyHostToDevice);
{	__set_CUDAConfig(1, N); 
          
	foo (dev_w, dev_g, N );}
          

	cudaMemcpy(w,dev_w,size,cudaMemcpyDeviceToHost);

	cudaMemcpy(g,dev_g,sizeof(funcType),cudaMemcpyDeviceToHost);

	printf("\nw:");
	for (int i = 0; i < N; ++i){
		printf(" %f	",	w[i]);
	}

	free(w);
	cudaFree(&dev_w);
	cudaFree(&dev_g);

	return 0;
}
