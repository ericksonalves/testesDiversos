//pass: o caso deve passar por causa do assert que sempre é verdadeiro
//--blockDim=16 --gridDim=16 --no-inline
//a = 12
//b = 36
//c = 48

#include <stdio.h>
#include <cuda.h>
//
#include <assert.h>
#define N 2//16

__global__ void example(unsigned a, unsigned b, unsigned c) {

    __requires(a == 12);
    __requires(b == 36);

    //__assert(a + b != c);
	__assert((a+b) != c);

}
