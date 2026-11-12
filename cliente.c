/*
 * Programa do cliente (Dupla D): gera senha e consulta posicao.
 * Compilar: gcc -Wall cliente.c fila.c armazenamento.c -o cliente
 */
#include <stdio.h>
#include "fila.h"
#include "armazenamento.h"

int main(void) {
    int opcao;

    do {
        printf("\n=== CLIENTE ===\n");
        printf("1 - Gerar senha\n");
        printf("2 - Consultar posicao\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) break;

        switch (opcao) {
        case 1:
            /* TODO (I-01, depois I-05): travar -> carregar -> enfileirar -> salvar -> destravar */
            printf("TODO: gerar senha\n");
            break;
        case 2:
            /* TODO (I-02, depois I-05): ler a senha e mostrar posicao_da_senha() */
            printf("TODO: consultar posicao\n");
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
