#include "fila.h"

/* F-01: fila nova tem qtd = 0, inicio = 0, fim = 0, proxima_senha = 1 */
void inicializar(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->qtd = 0;
    f->proxima_senha = 1;
}


/* F-02: gera senhas em sequencia; fim avanca com (fim + 1) % TAM; -1 se cheia */
int enfileirar(Fila *f) {
    if (f->qtd == TAM) {
        return -1;
    }
    
    int senha_gerada = f->proxima_senha;
    f->senhas[f->fim] = senha_gerada;
    f->fim = (f->fim + 1) % TAM;
    f->qtd++;
    f->proxima_senha++;
    
    return senha_gerada;
}

/* F-03: sai sempre a senha mais antiga; inicio avanca com (inicio + 1) % TAM; -1 se vazia */
int desenfileirar(Fila *f) {
    /* TODO (F-03) */
    return -1;
}

/* F-04: senha 10 com 8 e 9 na frente retorna 2; senha inexistente retorna -1 */
int posicao_da_senha(Fila *f, int senha) {
    /* TODO (F-04) */
    return -1;
}
