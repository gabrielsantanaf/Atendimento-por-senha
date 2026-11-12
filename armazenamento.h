#ifndef ARMAZENAMENTO_H
#define ARMAZENAMENTO_H

/*
 * Leitura/gravacao de fila.dat e trava com fila.lock (Dupla C).
 * Nao mexe na logica da fila.
 *
 * Padrao de uso em toda acao:
 *     travar();
 *     carregar_fila(&f);
 *     ... enfileirar / desenfileirar / consultar ...
 *     salvar_fila(&f);
 *     destravar();
 *
 * CONTRATO: nao mudar estas assinaturas sem combinar com o time.
 */

#include "fila.h"

#define ARQUIVO_FILA  "fila.dat"
#define ARQUIVO_TRAVA "fila.lock"

void carregar_fila(Fila *f);       /* fread da struct inteira; sem arquivo, inicializa fila nova */
void salvar_fila(const Fila *f);   /* fwrite da struct inteira */

void travar(void);                 /* cria fila.lock com fopen(..., "wx"); se ja existe, espera 100 ms e tenta de novo */
void destravar(void);              /* remove(fila.lock) */

#endif
