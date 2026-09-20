#include <omp.h>
#include <stdio.h>

int fib(int n) {
  int i, j;
  if (n < 2)
    return n;
  else {
    // Create a new task for computing fib(n-1)
    #pragma omp task shared(i)
    i = fib(n - 1);
    
    // Create a new task for computing fib(n-2)
    #pragma omp task shared(j)
    j = fib(n - 2);
    
    // Wait for both tasks to complete before adding them
    #pragma omp taskwait
    return i + j;
  }
}

int main() {
    int n = 20; 
    int result;
    double tstart, tstop, tcalc;

    tstart = omp_get_wtime();

    // Create a team of threads
    #pragma omp parallel
    {
        // Only one thread should generate the initial task
        #pragma omp single
        {
            result = fib(n);
        }
    }

    tstop = omp_get_wtime();
    tcalc = (tstop - tstart) * 1000.0;

    printf("Fibonacci of %d is %d\n", n, result);
    printf("Time taken: %f ms\n", tcalc);

    return 0;
}
