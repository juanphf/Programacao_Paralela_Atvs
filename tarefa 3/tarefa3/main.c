#include <stdio.h>
#include <math.h>
#include <time.h>


int main(){
    long long N = 10000000000; // 10 Bi
    long long checkpoint = 1000;

    double pi_real = 3.141592653589793;
    double pi_aprox = 0.0;
    int sinal = 1;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for(long long i=0; i<N; i++){
        pi_aprox += sinal * (4.0/ (2.0*i + 1.0));
        sinal *= -1;

        if(i+1 == checkpoint) {
            clock_gettime(CLOCK_MONOTONIC, &end);

            double tempo_gasto = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
            double erro = fabs(pi_real - pi_aprox);

            printf("Iteracoes: %11lld | Pi: %.10f  | Erro: %.10f  | Tempo: %f s\n", checkpoint, pi_aprox, erro, tempo_gasto);

            checkpoint *=10;
        }
    }
    return 0;

}