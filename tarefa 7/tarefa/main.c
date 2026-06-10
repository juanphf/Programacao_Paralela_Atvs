#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

struct Node {
    char nome_arquivo[50];
    struct Node* next;
};

void add_node(struct Node** head, const char* nome) {
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    strcpy(new_node->nome_arquivo, nome);
    new_node->next = *head;
    *head = new_node;
}

int main() {
    struct Node* head = NULL;
    
    add_node(&head, "Relatorio_Financeiro.pdf");
    add_node(&head, "Foto_Formatura.png");
    add_node(&head, "Planilha_Vendas.xlsx");
    add_node(&head, "Apresentacao.pptx");

    printf("Sem restricao (Geracao Duplicada)\n");
    #pragma omp parallel
    {
        struct Node* current = head;
        while (current != NULL) {
            #pragma omp task firstprivate(current)
            {
                printf("Thread %d processou: %s\n", omp_get_thread_num(), current->nome_arquivo);
            }
            current = current->next;
        }
    }

    printf("\nCom restricao (Geracao Correta)\n");
    #pragma omp parallel
    {
        #pragma omp single
        {
            struct Node* current = head;
            while (current != NULL) {
                #pragma omp task firstprivate(current)
                {
                    printf("Thread %d processou: %s\n", omp_get_thread_num(), current->nome_arquivo);
                }
                current = current->next;
            }
        }
    }

    struct Node* current = head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}