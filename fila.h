#ifndef FILA_H
#define FILA_H

/*
 * Logica da fila circular (Dupla A).
 * Nao le arquivo e nao imprime nada: so vetor, indices e operador modulo.
 *
 * CONTRATO: nao mudar estas assinaturas sem combinar com o time.
 */

#define TAM 5

typedef struct {
    int senhas[TAM];
    int inicio;        /* indice da proxima senha a ser chamada */
    int fim;           /* indice onde entra a proxima senha gerada */
    int qtd;           /* quantas senhas estao esperando */
    int proxima_senha; /* contador para gerar numeros novos (1, 2, 3...) */
} Fila;

void inicializar(Fila *f);
int  enfileirar(Fila *f);                  /* gera senha nova; retorna o numero ou -1 se cheia */
int  desenfileirar(Fila *f);               /* retorna a senha chamada ou -1 se vazia */
int  posicao_da_senha(Fila *f, int senha); /* quantas pessoas estao na frente; -1 se nao existe */

#endif
