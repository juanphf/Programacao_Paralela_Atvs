#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#define SIZE 50000000

void run_memory_bound(double *a, double *b, double *c, int threads) {
    omp_set_num_threads(threads);
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++) {
        c[i] = a[i] + b[i];
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                        (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Memory-bound (%d threads): %f segundos\n", threads, time_taken);
}

void run_compute_bound(double *a, double *c, int threads) {
    omp_set_num_threads(threads);
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++) {
        double temp = a[i];
        for (int j = 0; j < 50; j++) {
            temp = sin(temp) + cos(temp) * 0.5;
        }
        c[i] = temp;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                        (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Compute-bound (%d threads): %f segundos\n", threads, time_taken);
}

int main(int argc, char *argv[]) {
    int threads = 1;
    if (argc > 1) {
        threads = atoi(argv[1]);
    }

    double *a = (double *)malloc(SIZE * sizeof(double));
    double *b = (double *)malloc(SIZE * sizeof(double));
    double *c = (double *)malloc(SIZE * sizeof(double));

    for (int i = 0; i < SIZE; i++) {
        a[i] = i * 0.1;
        b[i] = i * 0.2;
    }

    run_memory_bound(a, b, c, threads);
    run_compute_bound(a, c, threads);

    free(a);
    free(b);
    free(c);

    return 0;
}