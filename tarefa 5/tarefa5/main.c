#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int is_prime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void run_sequential(int n) {
    int count = 0;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 2; i <= n; i++) {
        if (is_prime(i)) {
            count++;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Sequencial: \t\t%d primos \t(%f segundos)\n", count, time_taken);
}

void run_parallel_naive(int n, int threads) {
    int count = 0;
    omp_set_num_threads(threads);
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    #pragma omp parallel for
    for (int i = 2; i <= n; i++) {
        if (is_prime(i)) {
            count++;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Paralelo (Ingenuo): \t%d primos \t(%f segundos)\n", count, time_taken);
}

void run_parallel_fixed(int n, int threads) {
    int count = 0;
    omp_set_num_threads(threads);
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    #pragma omp parallel for reduction(+:count) schedule(dynamic)
    for (int i = 2; i <= n; i++) {
        if (is_prime(i)) {
            count++;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Paralelo (Corrigido): \t%d primos \t(%f segundos)\n", count, time_taken);
}

int main(int argc, char *argv[]) {
    int threads = 4;
    int n = 5000000;

    if (argc > 1) {
        threads = atoi(argv[1]);
    }
    
    printf("Buscando primos de 2 ate %d usando %d threads...\n\n", n, threads);

    run_sequential(n);
    run_parallel_naive(n, threads);
    run_parallel_fixed(n, threads);

    return 0;
}