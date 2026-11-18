#ifndef FILA_LINEAR_H
#define FILA_LINEAR_H

#include "fila.h"   // reaproveita o TAM

/* Fila linear em memória (D-01). Usada só na demonstração.
   Os índices só andam para frente: nunca voltam para 0. */
typedef struct {
    int senhas[TAM];
    int inicio;
    int fim;
    int qtd;
    int proxima_senha;
} FilaLinear;

void inicializar_linear(FilaLinear *f);
int enfileirar_linear(FilaLinear *f);     // retorna a senha ou -1 se fim == TAM
int desenfileirar_linear(FilaLinear *f);  // retorna a senha chamada ou -1 se vazia

#endif