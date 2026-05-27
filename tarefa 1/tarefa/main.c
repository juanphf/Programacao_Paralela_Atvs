#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double get_time_diff(struct timespec start, struct timespec end)
{
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
};

int main (){
    int sizes[] = {256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
    int num_sizes = sizeof(sizes)/sizeof(sizes[0]);

    for (int s=0; s < num_sizes; s++) {
        int n = sizes[s];

        int (*A)[n] = malloc(sizeof(int[n][n]));
        int *X = malloc(n*sizeof(int));
        int *Y_row = calloc(n, sizeof(int));
        int *Y_col = calloc(n, sizeof(int));
        
        if (!A || !X || !Y_row || !Y_col){
            printf("Erro na alocação: %d\n", n);
            return 1;
        }
        for (int i = 0; i < n; i++){
            X[i] = rand() % 100;
            for (int j = 0; j<n; j++){
                A[i][j] =  rand() % 100;
            }
        }
        
        struct timespec start, end;
        double time_row, time_col;

        clock_gettime (CLOCK_MONOTONIC, &start);
        for (int i=0; i<n; i++){
            for (int j=0; j<n; j++){
                Y_row[i] += A[i][j]* X[j];
            }
        }
        clock_gettime (CLOCK_MONOTONIC, &end);
        time_row = get_time_diff(start, end);

        clock_gettime (CLOCK_MONOTONIC, &start);
        for (int j=0; j<n; j++){
            for (int i=0; i<n; i++){
                Y_col[i] += A[i][j]* X[j];
            }
        }
        clock_gettime (CLOCK_MONOTONIC, &end);
        time_col = get_time_diff(start, end);

        printf("%-10d | %-20.6f | %-20.6f | %-10.2f\n", n, time_row, time_col, time_col / time_row);

        free(A);
        free(X);
        free(Y_row);
        free(Y_col);       
    }
    return 0;
}