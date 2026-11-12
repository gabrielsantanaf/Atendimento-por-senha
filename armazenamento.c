#include <stdio.h>
#include "armazenamento.h"

/* A-01: gravar, fechar e reabrir devolve a fila identica; sem arquivo, cria fila nova */
void carregar_fila(Fila *f) {
    /* TODO (A-01): fopen(ARQUIVO_FILA, "rb") + fread(f, sizeof(Fila), 1, arq) */
    inicializar(f);
}

void salvar_fila(const Fila *f) {
    /* TODO (A-01): fopen(ARQUIVO_FILA, "wb") + fwrite(f, sizeof(Fila), 1, arq) */
}

/* A-02: segundo programa espera enquanto o primeiro tem a trava; fila.lock e apagado no fim */
void travar(void) {
    /* TODO (A-02): loop com fopen(ARQUIVO_TRAVA, "wx"); se falhar, esperar 100 ms
       (Sleep(100) no Windows / usleep(100000) no Linux) e tentar de novo */
}

void destravar(void) {
    /* TODO (A-02): remove(ARQUIVO_TRAVA) */
}
