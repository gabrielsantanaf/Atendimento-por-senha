/*
 * Testes da camada de armazenamento (Dupla C): A-01 e A-02.
 * Linux apenas (usa fork). Compilar e rodar dentro do Docker:
 *
 *     gcc -Wall -I. testes/teste_armazenamento.c fila.c armazenamento.c -o teste_armazenamento
 *     ./teste_armazenamento               # todos os testes
 *     ./teste_armazenamento --sem-trava   # demonstra a condicao de corrida (para a apresentacao)
 *
 * Os testes rodam numa pasta temporaria em /tmp, entao nao mexem no fila.dat do projeto.
 *
 * Atencao: processos filhos terminam com _exit(), nao exit(). O filho herda uma
 * copia do estado do pai; com exit() ele poderia apagar o fila.lock do pai.
 */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "fila.h"
#include "armazenamento.h"

static int falhas = 0;

#define CHECA(cond, msg) \
    do { if (cond) printf("[OK]    %s\n", msg); \
         else { printf("[FALHA] %s\n", msg); falhas++; } } while (0)

static long agora_ms(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000L + t.tv_nsec / 1000000L;
}

static void dormir_ms(long ms) {
    struct timespec t = { ms / 1000, (ms % 1000) * 1000000L };
    nanosleep(&t, NULL);
}

static int existe(const char *caminho) {
    return access(caminho, F_OK) == 0;
}

/* Espera o filho e devolve 1 se ele terminou com codigo 0. */
static int filho_ok(pid_t pid) {
    int status;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) && WEXITSTATUS(status) == 0;
}

/* Fila do exemplo do documento: [7 8 9 _ _], inicio 0, fim 3, qtd 3, proxima 10 */
static Fila fila_exemplo(void) {
    Fila f;
    memset(&f, 0, sizeof f);
    f.senhas[0] = 7;
    f.senhas[1] = 8;
    f.senhas[2] = 9;
    f.inicio = 0;
    f.fim = 3;
    f.qtd = 3;
    f.proxima_senha = 10;
    return f;
}

/* ---------------- A-01 ---------------- */

static void teste_sem_arquivo(void) {
    Fila f;
    remove(ARQUIVO_FILA);
    carregar_fila(&f);
    CHECA(f.qtd == 0 && f.inicio == 0 && f.fim == 0 && f.proxima_senha == 1,
          "A-01: sem fila.dat, carregar_fila devolve fila nova");
}

static void teste_ida_e_volta(void) {
    Fila original = fila_exemplo();
    salvar_fila(&original);

    /* "Fechar e reabrir": outro processo le o arquivo e compara byte a byte. */
    pid_t pid = fork();
    if (pid == 0) {
        Fila lida;
        carregar_fila(&lida);
        _exit(memcmp(&lida, &original, sizeof(Fila)) == 0 ? 0 : 1);
    }
    CHECA(filho_ok(pid), "A-01: outro processo carrega a fila identica a gravada");

    char temp[64];
    snprintf(temp, sizeof temp, "%s.tmp.%ld", ARQUIVO_FILA, (long)getpid());
    CHECA(!existe(temp), "A-01: salvar_fila nao deixa arquivo temporario para tras");
}

/* ---------------- A-02 ---------------- */

static void teste_cria_e_apaga_trava(void) {
    travar();
    CHECA(existe(ARQUIVO_TRAVA), "A-02: travar cria fila.lock");

    long pid_no_arquivo = 0;
    FILE *arq = fopen(ARQUIVO_TRAVA, "r");
    if (arq != NULL) {
        if (fscanf(arq, "%ld", &pid_no_arquivo) != 1) pid_no_arquivo = 0;
        fclose(arq);
    }
    CHECA(pid_no_arquivo == (long)getpid(), "A-02: fila.lock guarda o PID de quem travou");

    destravar();
    CHECA(!existe(ARQUIVO_TRAVA), "A-02: destravar apaga fila.lock");
}

static void teste_segundo_espera(void) {
    const long SEGURA_MS = 500;

    travar();                       /* o pai pega a trava primeiro */

    pid_t pid = fork();
    if (pid == 0) {
        long inicio = agora_ms();
        travar();                   /* tem que ficar esperando o pai */
        long esperou = agora_ms() - inicio;
        destravar();
        _exit(esperou >= SEGURA_MS - 100 ? 0 : 1);
    }

    dormir_ms(SEGURA_MS);
    destravar();                    /* libera: agora o filho consegue */

    CHECA(filho_ok(pid), "A-02: segundo processo espera enquanto o primeiro tem a trava");
    CHECA(!existe(ARQUIVO_TRAVA), "A-02: fila.lock apagado no fim");
}

/* ------------- Condicao de corrida ------------- */

#define PROCESSOS 4
#define VEZES     25

/* Cada processo soma 1 em proxima_senha VEZES vezes, lendo e gravando o arquivo.
   O atraso de 1 ms entre ler e gravar imita o tempo de processamento. */
static void trabalhador(int usar_trava) {
    for (int i = 0; i < VEZES; i++) {
        Fila f;
        if (usar_trava) travar();
        carregar_fila(&f);
        dormir_ms(1);
        f.proxima_senha++;
        salvar_fila(&f);
        if (usar_trava) destravar();
    }
    _exit(0);
}

/* Devolve o valor final de proxima_senha depois de PROCESSOS x VEZES somas. */
static int corrida(int usar_trava) {
    Fila f;
    memset(&f, 0, sizeof f);
    inicializar(&f);                /* proxima_senha = 1 */
    salvar_fila(&f);

    pid_t pids[PROCESSOS];
    for (int p = 0; p < PROCESSOS; p++) {
        pids[p] = fork();
        if (pids[p] == 0) trabalhador(usar_trava);
    }
    for (int p = 0; p < PROCESSOS; p++) filho_ok(pids[p]);

    carregar_fila(&f);
    return f.proxima_senha - 1;     /* quantas somas sobreviveram */
}

static void teste_sem_perda_com_trava(void) {
    int esperado = PROCESSOS * VEZES;
    int obtido = corrida(1);
    char msg[128];
    snprintf(msg, sizeof msg,
             "A-02: %d processos x %d gravacoes com trava = %d de %d (nenhuma perdida)",
             PROCESSOS, VEZES, obtido, esperado);
    CHECA(obtido == esperado, msg);
}

static void demonstra_sem_trava(void) {
    int esperado = PROCESSOS * VEZES;
    int com = corrida(1);
    int sem = corrida(0);
    printf("Com trava: %d de %d gravacoes\n", com, esperado);
    printf("Sem trava: %d de %d gravacoes  -> %d se perderam (condicao de corrida)\n",
           sem, esperado, esperado - sem);
}

/* ---------------- main ---------------- */

int main(int argc, char *argv[]) {
    char pasta[] = "/tmp/teste_armazenamento_XXXXXX";
    if (mkdtemp(pasta) == NULL || chdir(pasta) != 0) {
        perror("pasta temporaria");
        return 1;
    }

    if (argc > 1 && strcmp(argv[1], "--sem-trava") == 0) {
        demonstra_sem_trava();
    } else {
        teste_sem_arquivo();
        teste_ida_e_volta();
        teste_cria_e_apaga_trava();
        teste_segundo_espera();
        teste_sem_perda_com_trava();
        printf("\n%d falha(s)\n", falhas);
    }

    remove(ARQUIVO_FILA);
    remove(ARQUIVO_TRAVA);
    if (chdir("/") == 0) rmdir(pasta);
    return falhas != 0;
}
