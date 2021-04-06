//author: Yufei Yang
#include <thread>
using namespace std;
// complex algorithm for evaluation

// Create a structure that will hold the execution parameters for each thread
struct MYPARAM{
	long i_start;
	long i_stop;
	};


void mul(struct MYPARAM *p,double *A, double *B, double *C, int N, int P){

	for (int i = p->i_start; i<p->i_stop; i++)
		for (int j = 0; j < N; j++)
		{
			C[i * N + j] = 0;
				for (int k = 0; k < N; k++)
				{
					C[i * N + j] += A[i * N + k] * B[k * N + j];
				}
		}
	if( (N>(P*(N/P)) && (p->i_stop==(P*(N/P))))){
		for (long i = p->i_stop ; i<N; i++){
			for (int j = 0; j < N; j++)
			{
				C[i * N + j] = 0;
					for (int k = 0; k < N; k++)
					{
						C[i * N + j] += A[i * N + k] * B[k * N + j];
					}
			}
	}
	}
}
void mull(struct MYPARAM *p,double *A, double *B, double *C, int N, int P ){
	double a1,a2,a3,a4,a5,a6,a7,a8;
	int k,j;
	for (int i = p->i_start; i<p->i_stop; i++) {
		
		for ( k = 0; k  < (N-7) ; k+=8) {
			
			a1 = A[i*N+k];
			a2 = A[i*N+k+1];
			a3 = A[i*N+k+2];
			a4 = A[i*N+k+3];
			a5 = A[i*N+k+4];
			a6 = A[i*N+k+5];
			a7 = A[i*N+k+6];
			a8 = A[i*N+k+7];
			for ( j = 0; j < N; j++) {
				C[i*N + j] += a1 * B[k*N + j];
				C[i*N + j] += a2 * B[(k+1)*N + j];
				C[i*N + j] += a3 * B[(k+2)*N + j];
				C[i*N + j] += a4 * B[(k+3)*N + j];
				C[i*N + j] += a5 * B[(k+4)*N + j];
				C[i*N + j] += a6 * B[(k+5)*N + j];
				C[i*N + j] += a7 * B[(k+6)*N + j];
				C[i*N + j] += a8 * B[(k+7)*N + j];
			}
		
		}
		if(N % 8) {
			do {
				a1 = A[i*N + k];
				for (int j = 0; j < N; j++) {
					C[i*N + j] += B[k*N + j] * a1;
				}
			}while(++k < N);
		}

	}

	if( (N>(P*(N/P)) && (p->i_stop==(P*(N/P))))){
		for (long i = p->i_stop ; i<N; i++){
			for ( k = 0; k  < (N-7) ; k+=8) {
			
			a1 = A[i*N+k];
			a2 = A[i*N+k+1];
			a3 = A[i*N+k+2];
			a4 = A[i*N+k+3];
			a5 = A[i*N+k+4];
			a6 = A[i*N+k+5];
			a7 = A[i*N+k+6];
			a8 = A[i*N+k+7];
			for ( j = 0; j < N; j++) {
				C[i*N + j] += a1 * B[k*N + j];
				C[i*N + j] += a2 * B[(k+1)*N + j];
				C[i*N + j] += a3 * B[(k+2)*N + j];
				C[i*N + j] += a4 * B[(k+3)*N + j];
				C[i*N + j] += a5 * B[(k+4)*N + j];
				C[i*N + j] += a6 * B[(k+5)*N + j];
				C[i*N + j] += a7 * B[(k+6)*N + j];
				C[i*N + j] += a8 * B[(k+7)*N + j];
			}
		
		}
		if(N % 8) {
			do {
				a1 = A[i*N + k];
				for (int j = 0; j < N; j++) {
					C[i*N + j] += B[k*N + j] * a1;
				}
			}while(++k < N);
			}
		}
	}
}

void matrix_mult( double *A, double *B, double *C, int N)
{	
	#define orithread  
	
	#ifdef original
	//original code 
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			C[i * N + j] = 0;
			for (int k = 0; k < N; k++)
			{
				C[i * N + j] += A[i * N + k] * B[k * N + j];
			}
		}
		return;
	#endif

		//number of threads
	int P = 20;
	thread t[P];
	struct MYPARAM *param = new struct MYPARAM[P];
	//STEP 2 PARTITION the job ref: pi_demo.cpp
   for (int i = 0; i < P; i++)
	{
		param[i].i_start = i * (N/P);
		param[i].i_stop = (i + 1) * (N/P);
	}



	#ifdef orithread
	//step 3 fire off
	for (int i = 0; i < P; i++)
	{
		t[i] = thread(mul, &param[i], A,B,C,N,P);
	}
	//step 4 join
	for (int i = 0; i < P; i++)
		t[i].join();
	#endif

	//reference Tuning the maxtrix multiply algorithm @ sg532@cornell.edu
	#ifdef everything
	//loop reording with loop unrolling 
	double a1,a2,a3,a4,a5,a6,a7,a8;
	int k,j;
	for (int i = 0; i < N; ++i) {
		
		for ( k = 0; k  < (N-7) ; k+=8) {
			
			a1 = A[i*N+k];
			a2 = A[i*N+k+1];
			a3 = A[i*N+k+2];
			a4 = A[i*N+k+3];
			a5 = A[i*N+k+4];
			a6 = A[i*N+k+5];
			a7 = A[i*N+k+6];
			a8 = A[i*N+k+7];
			for ( j = 0; j < N; j++) {
				C[i*N + j] += a1 * B[k*N + j];
				C[i*N + j] += a2 * B[(k+1)*N + j];
				C[i*N + j] += a3 * B[(k+2)*N + j];
				C[i*N + j] += a4 * B[(k+3)*N + j];
				C[i*N + j] += a5 * B[(k+4)*N + j];
				C[i*N + j] += a6 * B[(k+5)*N + j];
				C[i*N + j] += a7 * B[(k+6)*N + j];
				C[i*N + j] += a8 * B[(k+7)*N + j];
			}
		
		}
		if(N % 8) {
			do {
				a1 = A[i*N + k];
				for (int j = 0; j < N; j++) {
					C[i*N + j] += B[k*N + j] * a1;
				}
			}while(++k < N);
		}

	}
	#endif

	#ifdef everythingthread
		//loop reording with loop unrolling 
	//step 3 fire off
	for (int i = 0; i < P; i++)
	{
		t[i] = thread(mull, &param[i], A,B,C,N,P);
	}
	//step 4 join
	for (int i = 0; i < P; i++)
		t[i].join();

	#endif

	
}

