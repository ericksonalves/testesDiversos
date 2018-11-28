//#include <cuda.h>
#include <stdio.h>
#include <math.h>

typedef enum cublasstatus { CUBLAS_STATUS_SUCCESS,
	CUBLAS_STATUS_NOT_INITIALIZED,
	CUBLAS_STATUS_ALLOC_FAILED,
	CUBLAS_STATUS_INVALID_VALUE,
	CUBLAS_STATUS_ARCH_MISMATCH,
	CUBLAS_STATUS_MAPPING_ERROR,
	CUBLAS_STATUS_EXECUTION_FAILED,
	CUBLAS_STATUS_INTERNAL_ERROR,
	CUBLAS_STATUS_NOT_SUPPORTED,
	CUBLAS_STATUS_LICENSE_ERROR} custatusResult;

typedef enum cublasstatus cublasStatus_t;
typedef struct cublashandle {
} cublasHandle_t;

typedef enum cublasoperation {CUBLAS_OP_N,
	CUBLAS_OP_T,
	CUBLAS_OP_C} cuoperation;

typedef enum cublasoperation cublasOperation_t;

cublasStatus_t cublasCreate(cublasHandle_t *handle) {
/*
This function initializes the CUBLAS library and creates a handle to an opaque structure holding the CUBLAS library context. It allocates hardware resources on the host and device and must be called prior to making any other CUBLAS library calls. The CUBLAS library context is tied to the current CUDA device. To use the library on multiple devices, one CUBLAS handle needs to be created for each device. Furthermore, for a given device, multiple CUBLAS handles with different configuration can be created. Because cublasCreate allocates some internal resources and the release of those resources by calling cublasDestroy will implicitly call cublasDeviceSynchronize, it is recommended to minimize the number of cublasCreate/cublasDestroy occurences. For multi-threaded applications that use the same device from different threads, the recommended programming model is to create one CUBLAS handle per thread and use that CUBLAS handle for the entire life of the thread.
*/
	return CUBLAS_STATUS_SUCCESS;
}




cublasStatus_t cublasDestroy(cublasHandle_t handle) {

/*
This function releases hardware resources used by the CUBLAS library. This function is usually the last call with a particular handle to the CUBLAS library. Because cublasCreate allocates some internal resources and the release of those resources by calling cublasDestroy will implicitly call cublasDeviceSynchronize, it is recommended to minimize the number of cublasCreate/cublasDestroy occurences.
*/

	return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasSetMatrix(int rows, int cols, int elemSize,
                const void *A, int lda, void *B, int ldb) {
/*	This function copies a tile of rows x cols elements from a matrix A in host
memory space to a matrix B in GPU memory space. It is assumed that each element
requires storage of elemSize bytes and that both matrices are stored in column-major
 format, with the leading dimension of the source matrix A and destination matrix B
given in lda and ldb, respectively. The leading dimension indicates the number of rows
of the allocated matrix, even if only a submatrix of it is being used. In general,
 B is a device pointer that points to an object, or part of an object, that was
allocated in GPU memory space via cublasAlloc().
*/
	//Due to the Fortran column major the ldb must be the rows of matrix A
	//__ESBMC_assert(ldb == rows, "Full matrix is not bein copied");
        //cudaMemcpy(&B, &A, rows*cols*elemSize, cudaMemcpyDeviceToHost);


	return CUBLAS_STATUS_SUCCESS;
}


cublasStatus_t cublasGetMatrix(int rows, int cols, int elemSize,
                const void *A, int lda, void *B, int ldb) {
/*
This function copies a tile of rows x cols elements from a matrix A in GPU memory space
 to a matrix B in host memory space. It is assumed that each element requires storage
of elemSize bytes and that both matrices are stored in column-major format, with the
leading dimension of the source matrix A and destination matrix B given in lda and ldb,
 respectively. The leading dimension indicates the number of rows of the allocated
 matrix, even if only a submatrix of it is being used. In general, A is a device
pointer that points to an object, or part of an object, that was allocated in GPU
memory space via cublasAlloc().
*/

	//__ESBMC_assert(lda == cols, "Full matrix is not bein recovered");
	//cudaMemcpy(&B, &A, rows*cols*elemSize, cudaMemcpyDeviceToHost);
	return CUBLAS_STATUS_SUCCESS;

}

cublasStatus_t cublasSetVector(int n,  int elemSize,
                const void *A, int lda, void *B, int ldb) {


	//__ESBMC_assert(lda == ldb, "Full matrix is not bein copied");
        //cudaMemcpy(&B, &A, n*elemSize, cudaMemcpyHostToDevice);


	return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasGetVector(int n,  int elemSize,
                const void *A, int lda, void *B, int ldb) {

	//__ESBMC_assert(ldb == lda, "Full matrix is not bein copied");
        //cudaMemcpy(&B, &A, n*elemSize, cudaMemcpyDeviceToHost);


	return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasIsamin(cublasHandle_t handle, int n,
                            const float *x, int incx, int *result){

		int i = 0;
		int j;
		result = 0;
		for(i=0;i < n; i++){
			j = 1+(i-1)*incx;
			if(x[result[0]] >= x[j]) {
				result[0] = j;
		}
		return CUBLAS_STATUS_SUCCESS;
}
}

cublasStatus_t cublasIdamin(cublasHandle_t handle, int n,
                            const double *x, int incx, int *result){

		int i = 0;
		int j;
		result = 0;
		for(i=0;i < n; i++){
			j = 1+(i-1)*incx;
			if(x[result[0]] >= x[j]) {
				result[0] = j;
		}
		return CUBLAS_STATUS_SUCCESS;
}
}

cublasStatus_t  cublasSasum(cublasHandle_t handle, int n,
                            const float           *x, int incx, float  *result){

		int i = 0;
		int j;
		float calculate = 0;
		result = 0;
		for(i=0;i < n; i++){
			j = 1+(i-1)*incx;
			calculate = x[j] + calculate;
		}
		result[0] = calculate;
		return CUBLAS_STATUS_SUCCESS;
}



cublasStatus_t  cublasDasum(cublasHandle_t handle, int n,
                            const double          *x, int incx, double *result){

		int i = 0;
		int j;
		double calculate = 0;
		result = 0;
		for(i=0;i < n; i++){
			j = 1+(i-1)*incx;
			calculate = x[j] + calculate;
		}
		result[0] = calculate;
		return CUBLAS_STATUS_SUCCESS;
}


cublasStatus_t cublasIsamax(cublasHandle_t handle, int n,
                            const float *x, int incx, int *result){

		int i = 0;
		int j;
		result = 0;
		for(i=0;i < n; i++){
			j = 1+(i-1)*incx;
			if(x[result[0]] <= x[j]) {
				result[0] = j;
		}
		return CUBLAS_STATUS_SUCCESS;
}
}


cublasStatus_t cublasIdamax(cublasHandle_t handle, int n,
                            const double *x, int incx, int *result){

		int i = 0;
		int j;
		result = 0;
		for(i=0;i < n; i++){
			j = 1+(i-1)*incx;
			if(x[result[0]] <= x[j]) {
				result[0] = j;
		}
		return CUBLAS_STATUS_SUCCESS;
}
}


cublasStatus_t  cublasSscal(cublasHandle_t handle, int n,
                            const float           *alpha,
                            float           *x, int incx) {

		int i = 0;
		int j;
		for(i=0;i < n; i++){
			j = 1+(i-1)*incx;
			x[j]= ((float)alpha[0])*x[j];
		}
		return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t  cublasDscal(cublasHandle_t handle, int n,
                            const double           *alpha,
                            double           *x, int incx) {

		int i = 0;
		int j;
		for(i=0;i < n; i++){
			j = 1+(i-1)*incx;
			x[j]= ((double)alpha[0])*x[j];
		}
		return CUBLAS_STATUS_SUCCESS;
}


cublasStatus_t cublasSswap(cublasHandle_t handle, int n, float           *x,
                           int incx, float           *y, int incy){

		int i = 0;
		int j, k;;
		float aux = 0;
		for(i=0;i < n; i++){
			k = 1+(i-1)*incx;
			j = 1+(i-1)*incy;
			aux = y[j];
			y[j]= x[k];
			x[k] = y[j];
		}
		return CUBLAS_STATUS_SUCCESS;
}


cublasStatus_t cublasDswap(cublasHandle_t handle, int n, double          *x,
                           int incx, double          *y, int incy){

		int i = 0;
		int j, k;
		double aux = 0;
		for(i=0;i < n; i++){
			k = 1+(i-1)*incx;
			j = 1+(i-1)*incy;
			aux = y[j];
			y[j]= x[k];
			x[k] = y[j];
		}
		return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasSdot (cublasHandle_t handle, int n,
                           const float           *x, int incx,
                           const float           *y, int incy,
                           float           *result){

		int i = 0;
		int j, k;
		float aux = 0;
		for(i=0;i < n; i++){
			k = 1+(i-1)*incx;
			j = 1+(i-1)*incy;
			aux = y[j]*x[k] + aux;
		}
		result[0] = aux;
		return CUBLAS_STATUS_SUCCESS;
}



cublasStatus_t cublasDdot (cublasHandle_t handle, int n,
                           const double          *x, int incx,
                           const double          *y, int incy,
                           double          *result){

		int i = 0;
		int j, k;
		double aux = 0;
		for(i=0;i < n; i++){
			k = 1+(i-1)*incx;
			j = 1+(i-1)*incy;
			aux = y[j]*x[k] + aux;
		}
		result[0] = aux;
		return CUBLAS_STATUS_SUCCESS;
}




cublasStatus_t cublasScopy(cublasHandle_t handle, int n,
                           const float           *x, int incx,
                           float                 *y, int incy){

		int i = 0;
		int j, k;
		for(i=0;i < n; i++){
			k = 1+(i-1)*incx;
			j = 1+(i-1)*incy;
			y[j]= x[k];
		}
		return CUBLAS_STATUS_SUCCESS;
}


cublasStatus_t cublasDcopy(cublasHandle_t handle, int n,
                           const double          *x, int incx,
                           double                *y, int incy){

		int i = 0;
		int j, k;
		for(i=0;i < n; i++){
			k = 1+(i-1)*incx;
			j = 1+(i-1)*incy;
			y[j]= x[k];
		}
		return CUBLAS_STATUS_SUCCESS;
}



cublasStatus_t cublasSgemm(cublasHandle_t handle,
			cublasOperation_t transa, cublasOperation_t transb,
			int m, int n, int k,
			const float *alpha,
			const float *A, int lda,
			const float *B, int ldb,
			const float *beta,
			float *C, int ldc) {
	int contadorX = 0, contadorY = 0;
	int contadorZ = 0;
	float result = 0;
	//__ESBMC_assert(lda == m || lda == k, "The leading dimensions doens't correspond to matrix A dimensions. Array out of bounds.");
	//__ESBMC_assert(ldb == k || ldb == n, "The leading dimensions doens't correspond to matrix B dimensions. Array out of bounds.");
	//__ESBMC_assert(ldc == m || ldc == k, "The leading dimensions doens't correspond to matrix C dimensions. Array out of bounds.");
	if ((transa == CUBLAS_OP_N) && (transb == CUBLAS_OP_N)) {
		result = 0;
		for(contadorZ=0; contadorZ<m; contadorZ++){
			for(contadorY=0; contadorY<n; contadorY++) {
				result = 0;
				for(contadorX=0;contadorX<k; contadorX++) {
					//result =  (A[contadorX + contadorY*k] * B[contadorX*n + contadorY]) + result;
					result =  (A[contadorX + contadorZ*lda] * B[contadorX*ldb + contadorY]) + result;
					}
				C[contadorY + contadorZ*ldc] = alpha[0]*result + beta[0]*C[contadorY + contadorZ*ldc];
			}
		}
	}

	else if ((transa == CUBLAS_OP_N) && (transb == CUBLAS_OP_T)) {
		result = 0;
		for(contadorZ=0; contadorZ<m; contadorZ++){
			for(contadorY=0; contadorY<n; contadorY++) {
				result = 0;
				for(contadorX=0;contadorX<k; contadorX++) {
					//result =  (A[contadorX + contadorY*k] * B[contadorX*n + contadorY]) + result;
					result =  (A[contadorX + contadorZ*k] * B[contadorX + contadorY*n]) + result;
					}
				C[contadorY + contadorZ*m] = alpha[0]*result + beta[0]*C[contadorY + contadorZ*m];
			}
		}
	}
	else if ((transa == CUBLAS_OP_T) && (transb == CUBLAS_OP_N)) {
		result = 0;
		for(contadorZ=0; contadorZ<m; contadorZ++){
			for(contadorY=0; contadorY<n; contadorY++) {
				result = 0;
				for(contadorX=0;contadorX<k; contadorX++) {
					//result =  (A[contadorX + contadorY*k] * B[contadorX*n + contadorY]) + result;
					result =  (A[contadorX*k + contadorZ] * B[contadorX*n + contadorY]) + result;
					}
				C[contadorY + contadorZ*m] = alpha[0]*result + beta[0]*C[contadorY + contadorZ*m];
			}
		}
	}
	else if ((transa == CUBLAS_OP_T) && (transb == CUBLAS_OP_T)) {
		result = 0;
		for(contadorZ=0; contadorZ<m; contadorZ++){
			for(contadorY=0; contadorY<n; contadorY++) {
				result = 0;
				for(contadorX=0;contadorX<k; contadorX++) {
					//result =  (A[contadorX + contadorY*k] * B[contadorX*n + contadorY]) + result;
					result =  (A[contadorX*k + contadorZ] * B[contadorX + contadorY*n]) + result;
					}
				C[contadorY + contadorZ*m] = alpha[0]*result + beta[0]*C[contadorY + contadorZ*m];
			}
		}

	}
	return CUBLAS_STATUS_SUCCESS;

}

cublasStatus_t cublasDgemm(cublasHandle_t handle,
			cublasOperation_t transa, cublasOperation_t transb,
			int m, int n, int k,
			const double *alpha,
			const double *A, int lda,
			const double *B, int ldb,
			const double *beta,
			double *C, int ldc) {
	int contadorX = 0, contadorY = 0;
	int contadorZ = 0;
	double result = 0;

	if ((transa == CUBLAS_OP_N) && (transb == CUBLAS_OP_N)) {
		result = 0;
		for(contadorZ=0; contadorZ<m; contadorZ++){
			for(contadorY=0; contadorY<n; contadorY++) {
				result = 0;
				for(contadorX=0;contadorX<k; contadorX++) {
					//result =  (A[contadorX + contadorY*k] * B[contadorX*n + contadorY]) + result;
					result =  (A[contadorX + contadorZ*k] * B[contadorX*n + contadorY]) + result;
					}
				C[contadorY + contadorZ*m] = alpha[0]*result + beta[0]*C[contadorY + contadorZ*m];
			}
		}
	}
	else if ((transa == CUBLAS_OP_N) && (transb == CUBLAS_OP_T)) {
		result = 0;
		for(contadorZ=0; contadorZ<m; contadorZ++){
			for(contadorY=0; contadorY<n; contadorY++) {
				result = 0;
				for(contadorX=0;contadorX<k; contadorX++) {
					//result =  (A[contadorX + contadorY*k] * B[contadorX*n + contadorY]) + result;
					result =  (A[contadorX + contadorZ*k] * B[contadorX + contadorY*n]) + result;
					}
				C[contadorY + contadorZ*m] = alpha[0]*result + beta[0]*C[contadorY + contadorZ*m];
			}
		}
	}
	else if ((transa == CUBLAS_OP_T) && (transb == CUBLAS_OP_N)) {
		result = 0;
		for(contadorZ=0; contadorZ<m; contadorZ++){
			for(contadorY=0; contadorY<n; contadorY++) {
				result = 0;
				for(contadorX=0;contadorX<k; contadorX++) {
					//result =  (A[contadorX + contadorY*k] * B[contadorX*n + contadorY]) + result;
					result =  (A[contadorX*k + contadorZ] * B[contadorX*n + contadorY]) + result;
					}
				C[contadorY + contadorZ*m] = alpha[0]*result + beta[0]*C[contadorY + contadorZ*m];
			}
		}
	}
	else if ((transa == CUBLAS_OP_T) && (transb == CUBLAS_OP_T)) {
		result = 0;
		for(contadorZ=0; contadorZ<m; contadorZ++){
			for(contadorY=0; contadorY<n; contadorY++) {
				result = 0;
				for(contadorX=0;contadorX<k; contadorX++) {
					//result =  (A[contadorX + contadorY*k] * B[contadorX*n + contadorY]) + result;
					result =  (A[contadorX*k + contadorZ] * B[contadorX + contadorY*n]) + result;
					}
				C[contadorY + contadorZ*m] = alpha[0]*result + beta[0]*C[contadorY + contadorZ*m];
			}
		}

	}
	return CUBLAS_STATUS_SUCCESS;

}
cublasStatus_t cublasSaxpy(cublasHandle_t handle, int n,
                           const float           *alpha,
                           const float           *x, int incx,
                           float                 *y, int incy) {
		int i = 0;
		int k, j;
		for(i=0;i < n; i++){
			k = 1+(i-1)*incx;
			j = 1+(i-1)*incy;
			y[j]= ((float)alpha[0])*x[k] + y[j];
		}
		return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasDaxpy(cublasHandle_t handle, int n,
                           const double          *alpha,
                           const double          *x, int incx,
                           double                *y, int incy) {
		int i = 0;
		int k, j;
		for(i=0;i < n; i++){
			k = 1+(i-1)*incx;
			j = 1+(i-1)*incy;
			y[j]= ((double)alpha[0])*x[k] + y[j];
		}
		return CUBLAS_STATUS_SUCCESS;
}


float sigmoidFunction(float u) {
	float retorno;
	retorno = (1/(1 + powf(2.718281,(u*(-1)))));
	return retorno;
}


void activeSigmoid(float* saida, int size) {
	int i;
	for(i = 0; i < size; i++) {
		saida[i] = sigmoidFunction(saida[i]);
	}
}

int signalChange(float v1, float v2) {
	if(v1 > 0 && v2 < 0)
		return 1;
	else if(v1 < 0 && v2 > 0)
		return 1;
	else
		return 0;
}

int distanceAbsoluteFunction(float v1, float v2, float d) {
	if (fabs(v1 - v2) > d)
		return 1;
	else
		return 0;
}

int valueChange(float v1, float v2, float d) {
	if((!signalChange(v1,v2) && distanceAbsoluteFunction(v1,v2,d)))
		return 1;
	else
		return 0;
}

int distanceChange(float* out1, float* out2, int size) {
}

void normalizef(float* image, int size) {
	int i = 0;
	for(i=0;i++;i<size) {
			image[i] = image[i]/255;
	}
}

void normalized(double* image, int size) {
	int i = 0;
	for(i=0;i++;i<size) {
			image[i] = image[i]/255;
	}
}
