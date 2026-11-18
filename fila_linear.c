#include "fila_linear.h"

void inicializar_linear(FilaLinear *f) {
    for (int i = 0; i < TAM; i++) f->senhas[i] = 0;
    f->inicio = 0;
    f->fim = 0;
    f->qtd = 0;
    f->proxima_senha = 1;
}

int enfileirar_linear(FilaLinear *f) {
    // Limitação da linear: fim == TAM significa "cheia",
    // mesmo que haja posições livres no começo do vetor.
    if (f->fim == TAM) return -1;
    int senha = f->proxima_senha++;
    f->senhas[f->fim] = senha;
    f->fim++;               // sem módulo: nunca volta para 0
    f->qtd++;
    return senha;
}

int desenfileirar_linear(FilaLinear *f) {
    if (f->qtd == 0) return -1;
    int senha = f->senhas[f->inicio];
    f->inicio++;            // sem módulo: posição liberada nunca mais é usada
    f->qtd--;
    return senha;
}