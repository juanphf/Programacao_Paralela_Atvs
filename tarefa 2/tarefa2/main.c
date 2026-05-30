#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO 100000000

double calcular_tempo(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main() {
    double *vetor = (double *)malloc(TAMANHO * sizeof(double));
    if (vetor == NULL) {
        return 1;
    }

    struct timespec start, end;
    double tempo;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < TAMANHO; i++) {
        vetor[i] = i * 2.5;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    tempo = calcular_tempo(start, end);
    printf("Tempo do Laco 1 (Inicializacao): %f segundos\n", tempo);

    double soma_dependente = 0.0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < TAMANHO; i++) {
        soma_dependente += vetor[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    tempo = calcular_tempo(start, end);
    printf("Tempo do Laco 2 (Com dependencia): %f segundos (Soma: %.2f)\n", tempo, soma_dependente);

    double s1 = 0.0, s2 = 0.0, s3 = 0.0, s4 = 0.0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < TAMANHO; i += 4) {
        s1 += vetor[i];
        s2 += vetor[i + 1];
        s3 += vetor[i + 2];
        s4 += vetor[i + 3];
    }
    double soma_independente = s1 + s2 + s3 + s4;
    clock_gettime(CLOCK_MONOTONIC, &end);
    tempo = calcular_tempo(start, end);
    printf("Tempo do Laco 3 (Sem dependencia): %f segundos (Soma: %.2f)\n", tempo, soma_independente);

    free(vetor);
    return 0;
}