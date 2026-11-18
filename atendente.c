/*
 * Programa do atendente (Duplas D e B): chama a proxima senha, mostra a fila
 * e roda a demonstracao fila linear x circular.
 * Compilar: gcc -Wall atendente.c fila.c armazenamento.c -o atendente
 */
#include <stdio.h>
#include "fila.h"
#include "armazenamento.h"

/* D-01 / D-02 / D-03: demonstracao em memoria, NAO mexe em fila.dat.
   Roteiro (TAM = 5): enfileira 1..5, desenfileira 3x, tenta enfileirar 6, chama a proxima. */
static void demonstracao(void) {
    /* TODO (Dupla B) */
    printf("TODO: demonstracao linear x circular\n");
}
    /* I-04: mostra o estado atual da fila */
    static void mostrar_fila(Fila *f) {
        printf("\n=== ESTADO DA FILA ===\n");

        printf("\nVetor:\n");

        for (int i = 0; i < TAM; i++) {
            int ocupado = 0;

            for (int j = 0; j < f->qtd; j++) {
                int indice = (f->inicio + j) % TAM;

                if (indice == i) {
                    ocupado = 1;
                    break;
                }
            }

            if (ocupado) {
                printf("[%d] = %d\n", i, f->senhas[i]);
            } else {
                printf("[%d] = vazio\n", i);
            }
        }

    printf("\nInicio: %d\n", f->inicio);
    printf("Fim: %d\n", f->fim);
    printf("Quantidade: %d\n", f->qtd);
}
int main(void) {
    int opcao;
    Fila fila;
    inicializar(&fila);
 
    /* PREENCHIMENTO DA FILA */
    enfileirar(&fila);
    enfileirar(&fila);
    enfileirar(&fila);  

    do {
        printf("\n=== ATENDENTE ===\n");
        printf("1 - Chamar proxima\n");
        printf("2 - Ver fila\n");
        printf("3 - Demonstracao linear x circular\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) break;

        switch (opcao) {
        case 1:{
            /* TODO (I-03, depois I-05): travar -> carregar -> desenfileirar -> salvar -> destravar */
            int senha = desenfileirar(&fila);

            if (senha == -1) {
                printf("Nao existem senhas na fila.\n");
            } else {
                printf("Senha chamada: %d\n", senha);
            }

            break;
        }
        case 2:
            mostrar_fila(&fila);
            break;
        case 3:
            demonstracao();
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
