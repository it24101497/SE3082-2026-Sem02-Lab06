#include <omp.h>
#include <stdio.h>

#define NPOINTS 1000
#define MAXITER 2000

int main() {
    int numoutside = 0;
    double area, error;
    double tstart, tstop, tcalc;

    tstart = omp_get_wtime();

    // 1. Start parallel region before the main loop
    #pragma omp parallel default(none) reduction(+:numoutside)
    {
        double c_r, c_i, z_r, z_i, temp;
        int iter;

        // 2. Distribute the outermost loop across available threads
        #pragma omp for schedule(static)
        for (int i = 0; i < NPOINTS; i++) {
            for (int j = 0; j < NPOINTS; j++) {
                c_r = -2.0 + (2.5 * (double)i / (double)NPOINTS);
                c_i = 1.125 * (double)j / (double)NPOINTS;
                z_r = c_r;
                z_i = c_i;

                // Iteration z = z^2 + c
                for (iter = 0; iter < MAXITER; iter++) {
                    temp = (z_r * z_r) - (z_i * z_i) + c_r;
                    z_i = (z_r * z_i * 2.0) + c_i;
                    z_r = temp;
                    
                    // Threshold condition |z| > 2 (which is z_r^2 + z_i^2 > 4)
                    if ((z_r * z_r + z_i * z_i) > 4.0) {
                        numoutside++;
                        break;
                    }
                }
            }
        }
    }

    tstop = omp_get_wtime();
    tcalc = (tstop - tstart) * 1000.0;

    // Estimate the area
    area = 2.0 * 2.5 * 1.125 * (double)(NPOINTS * NPOINTS - numoutside) / (double)(NPOINTS * NPOINTS);
    error = area / (double)NPOINTS;

    printf("Area of Mandelbrot set = %f +/- %f\n", area, error);
    printf("Time taken: %f ms\n", tcalc);

    return 0;
}
