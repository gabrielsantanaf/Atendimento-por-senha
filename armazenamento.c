/*
 * Camada de armazenamento (Dupla C): guarda a Fila em fila.dat e controla o
 * acesso exclusivo com fila.lock.
 *
 * Alvo: Linux (container Docker). Usa recursos POSIX:
 *   - fopen(..., "wx")  -> open(O_CREAT | O_EXCL): criar-se-nao-existir e atomico
 *   - rename()          -> troca de arquivo atomica (nunca existe fila.dat pela metade)
 *   - nanosleep()       -> espera de 100 ms entre tentativas da trava
 *   - getpid()          -> PID de quem esta com a trava, gravado dentro do fila.lock
 */
#define _POSIX_C_SOURCE 200809L

#if defined(_WIN32)
#error "armazenamento.c usa POSIX (Linux). Compile dentro do Docker: ./compilar.sh"
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "armazenamento.h"

#define ESPERA_MS     100   /* intervalo entre tentativas de pegar a trava */
#define AVISO_APOS_MS 3000  /* depois disso, avisa que a trava pode estar abandonada */

static int trava_ativa = 0; /* 1 enquanto ESTE processo tem o fila.lock */

static void esperar_ms(long ms) {
    struct timespec t;
    t.tv_sec = ms / 1000;
    t.tv_nsec = (ms % 1000) * 1000000L;
    /* nanosleep pode ser interrompido por um sinal; continua o tempo que faltou */
    while (nanosleep(&t, &t) == -1 && errno == EINTR) {
    }
}

/* Confere se os indices lidos do arquivo fazem sentido, para a fila.c
   nunca acessar o vetor fora dos limites. */
static int fila_valida(const Fila *f) {
    return f->qtd >= 0 && f->qtd <= TAM
        && f->inicio >= 0 && f->inicio < TAM
        && f->fim >= 0 && f->fim < TAM
        && f->proxima_senha >= 1;
}

/* A-01 */
void carregar_fila(Fila *f) {
    FILE *arq = fopen(ARQUIVO_FILA, "rb");

    if (arq == NULL) {
        if (errno != ENOENT) {               /* existe, mas nao deu para abrir */
            perror("carregar_fila: " ARQUIVO_FILA);
            exit(EXIT_FAILURE);
        }
        memset(f, 0, sizeof *f);             /* sem arquivo: fila nova */
        inicializar(f);
        return;
    }

    size_t lidos = fread(f, sizeof(Fila), 1, arq);
    int sobrou = (fgetc(arq) != EOF);        /* arquivo maior que a struct? */
    fclose(arq);

    if (lidos != 1 || sobrou || !fila_valida(f)) {
        fprintf(stderr,
                "carregar_fila: %s esta corrompido ou foi gravado com outro TAM.\n"
                "Apague o arquivo para comecar uma fila nova.\n", ARQUIVO_FILA);
        exit(EXIT_FAILURE);
    }
}

/* A-01: grava num arquivo temporario e depois troca com rename().
   Se o programa cair no meio do fwrite, o fila.dat antigo continua inteiro. */
void salvar_fila(const Fila *f) {
    char temp[64];
    snprintf(temp, sizeof temp, "%s.tmp.%ld", ARQUIVO_FILA, (long)getpid());

    FILE *arq = fopen(temp, "wb");
    if (arq == NULL) {
        perror("salvar_fila: abrir temporario");
        exit(EXIT_FAILURE);
    }

    int ok = (fwrite(f, sizeof(Fila), 1, arq) == 1);
    if (fclose(arq) != 0) {
        ok = 0;
    }
    if (!ok) {
        perror("salvar_fila: gravar temporario");
        remove(temp);
        exit(EXIT_FAILURE);
    }

    if (rename(temp, ARQUIVO_FILA) != 0) {
        perror("salvar_fila: rename");
        remove(temp);
        exit(EXIT_FAILURE);
    }
}

/* Se o programa sair com exit() segurando a trava (ex.: erro no carregar_fila),
   apaga o fila.lock para nao travar o outro programa para sempre. */
static void liberar_trava_na_saida(void) {
    if (trava_ativa) {
        destravar();
    }
}

/* A-02 */
void travar(void) {
    static int saida_registrada = 0;
    long esperado_ms = 0;
    int avisou = 0;

    if (!saida_registrada) {
        atexit(liberar_trava_na_saida);
        saida_registrada = 1;
    }

    for (;;) {
        /* "x": so cria se NAO existir. Criar e checar acontecem numa unica
           operacao do sistema, entao dois processos nunca conseguem juntos. */
        FILE *trava = fopen(ARQUIVO_TRAVA, "wx");

        if (trava != NULL) {
            fprintf(trava, "%ld\n", (long)getpid()); /* quem esta com a trava */
            fclose(trava);
            trava_ativa = 1;
            return;
        }

        if (errno != EEXIST) {               /* erro de verdade (permissao, disco...) */
            perror("travar: " ARQUIVO_TRAVA);
            exit(EXIT_FAILURE);
        }

        if (!avisou && esperado_ms >= AVISO_APOS_MS) {
            fprintf(stderr,
                    "Aguardando %s... Se nenhum programa estiver usando a fila, "
                    "apague o arquivo (rm %s).\n", ARQUIVO_TRAVA, ARQUIVO_TRAVA);
            avisou = 1;
        }

        esperar_ms(ESPERA_MS);
        esperado_ms += ESPERA_MS;
    }
}

/* A-02 */
void destravar(void) {
    trava_ativa = 0;
    if (remove(ARQUIVO_TRAVA) != 0) {
        perror("destravar: " ARQUIVO_TRAVA);
    }
}
