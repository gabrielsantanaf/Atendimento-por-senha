
/*
 * Programa do cliente (Dupla D): gera senha e consulta posicao.
 * Compilar: gcc -Wall cliente.c fila.c armazenamento.c -o cliente
 */

#include <stdio.h>
#include "fila.h"
#include "armazenamento.h"

int main(void) {
    int opcao;
    Fila fila;

    do {
        printf("\n=== CLIENTE ===\n");
        printf("1 - Gerar senha\n");
        printf("2 - Consultar posicao\n");
        printf("0 - Sair\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida.\n");
            return 1;
        }

        switch (opcao) {
            case 1: {
                travar();
                carregar_fila(&fila);

                int senha = enfileirar(&fila);

                if (senha == -1) {
                    printf("Fila cheia! Nao foi possivel gerar senha.\n");
                } else {
                    salvar_fila(&fila);
                    printf("Sua senha e: %d\n", senha);
                }

                destravar();
                break;
            }

            case 2: {
                int senha;

                printf("Digite sua senha: ");

                if (scanf("%d", &senha) != 1) {
                    printf("Entrada invalida.\n");
                    break;
                }

                travar();
                carregar_fila(&fila);

                int posicao = posicao_da_senha(&fila, senha);

                destravar();

                if (posicao == -1) {
                    printf("Senha nao encontrada na fila.\n");
                } else {
                    printf("Existem %d pessoas na sua frente.\n", posicao);
                }

                break;
            }

            case 0:
                printf("Encerrando atendimento.\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    return 0;
}