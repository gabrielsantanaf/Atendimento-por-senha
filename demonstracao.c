#include <stdio.h>
#include "demonstracao.h"
#include "fila.h"
#include "fila_linear.h"

static void pausar(void) {
    int c;
    printf("\n  [Enter para continuar]");
    fflush(stdout);
    while ((c = getchar()) != '\n' && c != EOF) { }
    printf("\n");
}

/* Mostra só as posições ocupadas (de inicio, qtd posições, com módulo).
   As demais aparecem como _ mesmo que guardem lixo de senhas já chamadas. */
static void imprimir_vetor(const int senhas[], int inicio, int qtd) {
    printf("    [");
    for (int i = 0; i < TAM; i++) {
        int ocupada = 0;
        for (int k = 0; k < qtd; k++) {
            if ((inicio + k) % TAM == i) { ocupada = 1; break; }
        }
        if (ocupada) printf(" %d", senhas[i]);
        else         printf(" _");
    }
    printf(" ]");
}

static void mostrar_linear(const FilaLinear *f) {
    printf("  Linear   ");
    imprimir_vetor(f->senhas, f->inicio, f->qtd);
    printf("  inicio=%d fim=%d qtd=%d\n", f->inicio, f->fim, f->qtd);
}

static void mostrar_circular(const Fila *f) {
    printf("  Circular ");
    imprimir_vetor(f->senhas, f->inicio, f->qtd);
    printf("  inicio=%d fim=%d qtd=%d\n", f->inicio, f->fim, f->qtd);
}

static void mostrar_ambas(const FilaLinear *l, const Fila *c) {
    mostrar_linear(l);
    mostrar_circular(c);
}

static void titulo(const char *texto) {
    printf("==============================================================\n");
    printf(" %s\n", texto);
    printf("==============================================================\n");
}

void demonstracao(void) {
    FilaLinear lin;
    Fila circ;
    inicializar_linear(&lin);
    inicializar(&circ);

    printf("\n");
    titulo("DEMONSTRACAO: FILA LINEAR x FILA CIRCULAR (TAM = 5)");
    printf("  As duas filas comecam vazias. Vamos acompanhar o vetor e os\n"
           "  indices a cada passo. '_' = posicao livre.\n\n");
    mostrar_ambas(&lin, &circ);
    pausar();

    /* Passo 1: enfileira 5 senhas */
    titulo("Passo 1: enfileirar 5 senhas (1 a 5)");
    for (int i = 0; i < TAM; i++) {
        int sl = enfileirar_linear(&lin);
        int sc = enfileirar(&circ);
        printf("  Entrou a senha %d (linear) e %d (circular)\n", sl, sc);
    }
    printf("\n");
    mostrar_ambas(&lin, &circ);
    printf("\n  Linear: fim = %d (passou da ultima posicao do vetor).\n", lin.fim);
    printf("  Circular: fim = (4 + 1) %% %d = %d. O modulo faz o indice\n"
           "  voltar para 0.\n", TAM, circ.fim);
    pausar();

    /* Passo 2: desenfileira 3 vezes */
    titulo("Passo 2: chamar 3 senhas (FIFO)");
    for (int i = 0; i < 3; i++) {
        int sl = desenfileirar_linear(&lin);
        int sc = desenfileirar(&circ);
        printf("  Saiu a senha %d (linear) e %d (circular)\n", sl, sc);
    }
    printf("\n");
    mostrar_ambas(&lin, &circ);
    printf("\n  FIFO: nas duas filas saiu primeiro quem chegou primeiro (1, 2, 3).\n"
           "  Agora ha 3 posicoes livres no comeco do vetor.\n");
    pausar();

    /* Passo 3: tenta enfileirar 6 */
    titulo("Passo 3: tentar enfileirar a senha 6");
    int sl = enfileirar_linear(&lin);
    int sc = enfileirar(&circ);
    if (sl == -1) {
        printf("  Linear:   \"Fila cheia!\" (fim = %d ja passou do vetor,\n"
               "            mesmo com 3 posicoes livres)\n", lin.fim);
    } else {
        printf("  Linear:   entrou a senha %d\n", sl);
    }
    if (sc == -1) {
        printf("  Circular: \"Fila cheia!\"\n");
    } else {
        printf("  Circular: entrou a senha %d na posicao 0, que foi reaproveitada\n", sc);
    }
    printf("\n");
    mostrar_ambas(&lin, &circ);
    printf("\n  Limitacao da linear: os indices so andam para frente, entao as\n"
           "  posicoes liberadas no comeco nunca mais sao usadas.\n"
           "  Circular: (fim + 1) %% %d reaproveita o espaco.\n", TAM);
    pausar();

    /* Passo 4: chama a próxima */
    titulo("Passo 4: chamar a proxima senha");
    sl = desenfileirar_linear(&lin);
    sc = desenfileirar(&circ);
    printf("  Saiu a senha %d (linear) e %d (circular)\n\n", sl, sc);
    mostrar_ambas(&lin, &circ);
    printf("\n  FIFO de novo: a senha mais antiga (4) sai primeiro nas duas.\n"
           "  Na circular, inicio == fim pode significar fila vazia OU cheia;\n"
           "  o contador qtd (aqui qtd=%d) desfaz a duvida.\n", circ.qtd);
    pausar();

    titulo("Fim da demonstracao");
}