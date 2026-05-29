#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

// Função para estimar Pi usando o método de Monte Carlo (Serial)
void estimativa_pi_serial(long long num_pontos) {
    long long pontos_no_circulo = 0;
    double inicio = omp_get_wtime();

    unsigned int seed = (unsigned int)time(NULL);

    for (long long i = 0; i < num_pontos; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            pontos_no_circulo++;
        }
    }

    double pi_estimado = 4.0 * pontos_no_circulo / num_pontos;
    double fim = omp_get_wtime();
    printf("Serial: Pi estimado = %f, Tempo = %f segundos\n", pi_estimado, fim - inicio);
}

// Versão paralela com condição de corrida
void estimativa_pi_corrida(long long num_pontos) {
    long long pontos_no_circulo = 0;
    double inicio = omp_get_wtime();

    #pragma omp parallel for
    for (long long i = 0; i < num_pontos; i++) {
        unsigned int seed = (unsigned int)time(NULL) + omp_get_thread_num();
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            pontos_no_circulo++; // Condição de corrida
        }
    }

    double pi_estimado = 4.0 * pontos_no_circulo / num_pontos;
    double fim = omp_get_wtime();
    printf("Condição de Corrida: Pi estimado = %f, Tempo = %f segundos\n", pi_estimado, fim - inicio);
}

// Versão corrigida com #pragma omp critical
void estimativa_pi_critical(long long num_pontos) {
    long long pontos_no_circulo = 0;
    double inicio = omp_get_wtime();

    #pragma omp parallel for
    for (long long i = 0; i < num_pontos; i++) {
        unsigned int seed = (unsigned int)time(NULL) + omp_get_thread_num();
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            #pragma omp critical
            {
                pontos_no_circulo++;
            }
        }
    }

    double pi_estimado = 4.0 * pontos_no_circulo / num_pontos;
    double fim = omp_get_wtime();
    printf("Critical: Pi estimado = %f, Tempo = %f segundos\n", pi_estimado, fim - inicio);
}

// Versão com parallel, for e cláusulas de escopo
void estimativa_pi_escopo(long long num_pontos) {
    long long pontos_no_circulo = 0;
    int var_privada; // Exemplo de variável para cláusulas
    double inicio = omp_get_wtime();

    #pragma omp parallel default(none) private(var_privada) shared(num_pontos, pontos_no_circulo)
    {
        var_privada = 0; // Cada thread tem sua própria cópia
        unsigned int seed = (unsigned int)time(NULL) + omp_get_thread_num();

        #pragma omp for
        for (long long i = 0; i < num_pontos; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;
            if (x * x + y * y <= 1.0) {
                #pragma omp atomic
                pontos_no_circulo++;
            }
        }
    }

    double pi_estimado = 4.0 * pontos_no_circulo / num_pontos;
    double fim = omp_get_wtime();
    printf("Escopo (com atomic): Pi estimado = %f, Tempo = %f segundos\n", pi_estimado, fim - inicio);
}


int main() {
    long long num_pontos = 10000000; // 10 milhões de pontos

    printf("Estimando Pi com %lld pontos.\n\n", num_pontos);

    estimativa_pi_serial(num_pontos);
    estimativa_pi_corrida(num_pontos);
    estimativa_pi_critical(num_pontos);
    estimativa_pi_escopo(num_pontos);

    printf("\nNota: A versão 'Condição de Corrida' provavelmente mostrará um resultado incorreto.\n");

    return 0;
}
