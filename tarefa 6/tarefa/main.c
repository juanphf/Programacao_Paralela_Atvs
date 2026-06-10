#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#ifdef _WIN32
int rand_r(unsigned int *seedp) {
    *seedp = *seedp * 1103515245 + 12345;
    return (unsigned int)(*seedp / 65536) % 32768;
}
#endif

int main() {
    long total_pontos = 10000000;
    long pontos_circulo;
    double pi;
    struct timespec start, end;
    double tempo_gasto;

    pontos_circulo = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    #pragma omp parallel for
    for (long i = 0; i < total_pontos; i++) {
        unsigned int seed = i;
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        
        if (x * x + y * y <= 1.0) {
            pontos_circulo++;
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    tempo_gasto = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    pi = 4.0 * pontos_circulo / total_pontos;
    printf("Metodo 1 (Incorreto - Condicao de Corrida): %f | Tempo: %f s\n", pi, tempo_gasto);

    pontos_circulo = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    #pragma omp parallel for
    for (long i = 0; i < total_pontos; i++) {
        unsigned int seed = i;
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        
        if (x * x + y * y <= 1.0) {
            #pragma omp critical
            {
                pontos_circulo++;
            }
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    tempo_gasto = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    pi = 4.0 * pontos_circulo / total_pontos;
    printf("Metodo 2 (Corrigido com Critical lento): %f | Tempo: %f s\n", pi, tempo_gasto);

    pontos_circulo = 0;
    long i;
    long last_i = 0;
    unsigned int seed_base = 12345;
    double x, y;

    clock_gettime(CLOCK_MONOTONIC, &start);

    #pragma omp parallel default(none) shared(total_pontos, pontos_circulo, last_i) firstprivate(seed_base) private(x, y, i)
    {
        long pontos_locais = 0;

        #pragma omp for lastprivate(last_i)
        for (i = 0; i < total_pontos; i++) {
            unsigned int seed = seed_base + i;
            x = (double)rand_r(&seed) / RAND_MAX;
            y = (double)rand_r(&seed) / RAND_MAX;

            if (x * x + y * y <= 1.0) {
                pontos_locais++;
            }
            last_i = i;
        }

        #pragma omp critical
        {
            pontos_circulo += pontos_locais;
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    tempo_gasto = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    pi = 4.0 * pontos_circulo / total_pontos;
    printf("Metodo 3 (Reestruturado com Clausulas): %f | Tempo: %f s\n", pi, tempo_gasto);
    printf("Ultima iteracao processada (lastprivate): %ld\n", last_i);

    return 0;
}