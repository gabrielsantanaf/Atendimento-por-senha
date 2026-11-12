/*
 * F-05: testes da fila (Duplas A e B).
 * Compilar (na raiz do projeto):
 *     gcc -Wall -I. testes/teste_fila.c fila.c -o teste_fila
 */
#include <stdio.h>
#include "fila.h"

static int falhas = 0;

#define CHECA(cond, msg) \
    do { if (cond) printf("[OK]    %s\n", msg); \
         else { printf("[FALHA] %s\n", msg); falhas++; } } while (0)

int main(void) {
    Fila f;
    inicializar(&f);

    /* TODO (F-05): cobrir fila vazia, fila cheia, volta do indice para 0 e ordem FIFO */
    CHECA(desenfileirar(&f) == -1, "fila vazia retorna -1 ao desenfileirar");

    printf("\n%d falha(s)\n", falhas);
    return falhas != 0;
}
