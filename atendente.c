/*
 * Programa do atendente (Duplas D e B): chama a proxima senha, mostra a fila
 * e roda a demonstracao fila linear x circular.
 * Compilar: gcc -Wall atendente.c fila.c armazenamento.c -o atendente
 */
#include <stdio.h>
#include <stdlib.h>
#include "fila.h"
#include "armazenamento.h"

static void pausar_e_limpar(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    printf("\nPressione ENTER para voltar ao menu...");
    getchar();

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* D-01 / D-02 / D-03: demonstracao em memoria, NAO mexe em fila.dat.
   Roteiro (TAM = 5): enfileira 1..5, desenfileira 3x, tenta enfileirar 6, chama a proxima. */
static void demonstracao(void) {
    /* TODO (Dupla B) */
    printf("TODO: demonstracao linear x circular\n");
}

int main(void) {
    int opcao;

    do {
        printf("\n=== ATENDENTE ===\n");
        printf("1 - Chamar proxima\n");
        printf("2 - Ver fila\n");
        printf("3 - Demonstracao linear x circular\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) break;

        switch (opcao) {
        case 1:
        {
            /* TODO (I-03, depois I-05): travar -> carregar -> desenfileirar -> salvar -> destravar */
            Fila f;
            int senha_chamada;

            travar();
            carregar_fila(&f);

            senha_chamada = desenfileirar(&f);

            if (senha_chamada != -1)
            {
                printf("\nSenha chamada: %d\n", senha_chamada);
                salvar_fila(&f);
            }
            else
            {
                printf("\nFila vazia! Nenhuma senha para chamar.\n");
            }

            destravar();
            pausar_e_limpar();
            break;
           
        }

        case 2:
            /* TODO (I-04): mostrar vetor, inicio, fim e qtd */
            Fila f;

            travar();
            carregar_fila(&f);
            destravar();

            printf("\n=== ESTADO ATUAL DA FILA ===\n");
            printf("Inicio: %d | Fim: %d | Quantidade: %d\n", f.inicio, f.fim, f.qtd);

            if (f.qtd == 0) {
                printf("Fila vazia no momento.\n");
            } else {
                printf("Vetor: [ ");
                for (int i = 0; i < TAM; i++) {
                    printf("%d ", f.senhas[i]);
                }
                printf("]\n");
            }

            pausar_e_limpar();
            break;

        case 3:
            demonstracao();
            pausar_e_limpar();
            break;
        case 0:
            break;
        default:
            printf("\nOpcao invalida.\n");
            pausar_e_limpar();
        }
    } while (opcao != 0);

    return 0;
}
