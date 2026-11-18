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

    /* teste 2 - fila */
    enfileirar(&f);
    enfileirar(&f);
    enfileirar(&f);
    enfileirar(&f);
    enfileirar(&f);
    
    /*teste 3 - indice circular */
    CHECA(f.qtd == TAM, "fila fica cheia após inserir 5 senhas");
    CHECA(enfileirar(&f) == -1, "fila cheia retorna -1 ao enfileirar");

    desenfileirar(&f);
    desenfileirar(&f);
    desenfileirar(&f);

    CHECA(enfileirar(&f) == 6,"insere senha 6 apos liberar espaco");

    CHECA(enfileirar(&f) == 7,"insere senha 7 apos liberar espaco");

    CHECA(enfileirar(&f) == 8,"insere senha 8 apos liberar espaco");

    CHECA(f.qtd == TAM,"fila volta a ficar cheia apos reutilizar posicoes");

    /*teste 4 - retirada de senha*/

    CHECA(desenfileirar(&f) == 4,"primeira senha chamada e a 4");

    CHECA(desenfileirar(&f) == 5,"segunda senha chamada e a 5");

    CHECA(desenfileirar(&f) == 6,"terceira senha chamada e a 6");

    CHECA(desenfileirar(&f) == 7,"quarta senha chamada e a 7");

    CHECA(desenfileirar(&f) == 8,"quinta senha chamada e a 8");

    CHECA(f.qtd == 0,"fila fica vazia apos retirar todas as senhas");

    /*teste 5 - posição da senha */

    inicializar(&f);

    enfileirar(&f);
    enfileirar(&f);
    enfileirar(&f);

    CHECA(posicao_da_senha(&f, 1) == 0,"senha 1 esta na primeira posicao");

    CHECA(posicao_da_senha(&f, 2) == 1,"senha 2 tem uma pessoa na frente");

    CHECA(posicao_da_senha(&f, 3) == 2,"senha 3 tem duas pessoas na frente");

    CHECA(posicao_da_senha(&f, 99) == -1,"senha inexistente retorna -1");

    printf("\n%d falha(s)\n", falhas);
    return falhas != 0;
}


