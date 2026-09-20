#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000000
#define STRIP_SIZE 256

int main() {
    float *A = (float*)malloc(N * sizeof(float));
    float *B = (float*)malloc(N * sizeof(float));
    float *C = (float*)malloc(N * sizeof(float));

    for (int i = 0; i < N; i++) {
        A[i] = 1.5f;
        B[i] = 2.0f;
    }

    double tstart, tstop, tcalc;
    tstart = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < N; i += STRIP_SIZE) {
        
        // Calculate the end limit to avoid the && condition in the loop header
        int end = (i + STRIP_SIZE < N) ? (i + STRIP_SIZE) : N;
        
        #pragma omp simd
        for (int j = i; j < end; j++) {
            C[j] = A[j] * B[j];
        }
    }

    tstop = omp_get_wtime();
    tcalc = (tstop - tstart) * 1000.0;

    printf("Calculation complete. First element of C is: %f\n", C[0]);
    printf("Time taken: %f ms\n", tcalc);

    free(A);
    free(B);
    free(C);
    return 0;
}
