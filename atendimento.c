#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NORMAL 999
#define MIN_NORMAL 1
#define MIN_PRIORITARIA 1000
#define MAX_PRIORITARIA 1999

typedef struct {
    int numero;
    int prioritaria;
} Ficha;

typedef struct {
    Ficha* fichas;
    int inicio;
    int fim;
    int capacidade;
} Fila;

void inicializar_fila(Fila* fila, int capacidade) {
    fila->fichas = (Ficha*) malloc(sizeof(Ficha) * capacidade);
    fila->capacidade = capacidade;
    fila->inicio = fila->fim = 0;
}

void destruir_fila(Fila* fila) {
    free(fila->fichas);
}

int tamanho_fila(Fila* fila) {
    return fila->fim - fila->inicio;
}

int cheia_fila(Fila* fila) {
    return tamanho_fila(fila) >= fila->capacidade;
}

int vazia_fila(Fila* fila) {
    return tamanho_fila(fila) == 0;
}

void inserir_ficha(Fila* fila, Ficha ficha) {
    if (cheia_fila(fila)) {
        printf("Erro: a fila esta cheia!\n");
        return;
    }
    fila->fichas[fila->fim] = ficha;
    fila->fim++;
}

Ficha remover_ficha(Fila* fila) {
    if (vazia_fila(fila)) {
        printf("Erro: a fila esta vazia!\n");
        Ficha ficha_vazia = {0, 0};
        return ficha_vazia;
    }
    Ficha ficha_removida = fila->fichas[fila->inicio];
    fila->inicio++;
    return ficha_removida;
}

void gerar_ficha_normal(Fila* fila, Ficha* ficha) {
    if (fila->fim >= MAX_NORMAL) {
        printf("Erro: nao e possivel gerar mais fichas normais!\n");
        return;
    }
    ficha->numero = fila->fim + MIN_NORMAL;
    ficha->prioritaria = 0;
    inserir_ficha(fila, *ficha);
}

void gerar_ficha_prioritaria(Fila* fila, Ficha* ficha) {
    if (fila->fim - fila->inicio >= MAX_PRIORITARIA - MIN_PRIORITARIA + 1) {
        printf("Erro: nao e possivel gerar mais fichas prioritarias!\n");
        return;
    }
    ficha->numero = MIN_PRIORITARIA + fila->fim - fila->inicio;
    ficha->prioritaria = 1;
    inserir_ficha(fila, *ficha);
}




int main() {
    srand(time(NULL));
    Fila fila_normal, fila_prioritaria;
    inicializar_fila(&fila_normal, MAX_NORMAL);
    inicializar_fila(&fila_prioritaria, MAX_PRIORITARIA - MIN_PRIORITARIA + 1);

    int opcao, num_atendimentos_normais = 0;
    do {
        printf("1 - Gerar Ficha Normal\n");
        printf("2 - Gerar Ficha Prioritaria\n");
        printf("3 - Atender\n");
        printf("4 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Ficha ficha;
                gerar_ficha_normal(&fila_normal, &ficha);
                printf("Ficha gerada: %03d\n", ficha.numero);
                break;
            }
            case 2: {
                Ficha ficha;
                gerar_ficha_prioritaria(&fila_prioritaria, &ficha);
                printf("Ficha gerada: %04d\n", ficha.numero);
                break;
            }
            case 3: {
                Ficha ficha_atendida;
                if (tamanho_fila(&fila_prioritaria) > 0) {
                    ficha_atendida = remover_ficha(&fila_prioritaria);
                    printf("Atendendo ficha prioritaria: %04d\n", ficha_atendida.numero);
                    num_atendimentos_normais = 0;
                } else if (tamanho_fila(&fila_normal) > 0) {
                    ficha_atendida = remover_ficha(&fila_normal);
                    printf("Atendendo ficha normal: %03d\n", ficha_atendida.numero);
                    num_atendimentos_normais++;
                    if (num_atendimentos_normais == 3) {
                        printf("Chamando ficha prioritaria!\n");
                        num_atendimentos_normais = 0;
                        if (tamanho_fila(&fila_prioritaria) > 0) {
                            ficha_atendida = remover_ficha(&fila_prioritaria);
                            printf("Atendendo ficha prioritaria: %04d\n", ficha_atendida.numero);
                        }
                    }
                } else {
                    printf("Nao ha fichas para atender!\n");
                }
                break;
            }
            case 4: {
                printf("Encerrando programa...\n");
                break;
            }
            default: {
                printf("Opcao invalida!\n");
                break;
            }
        }
    } while (opcao != 4);

    destruir_fila(&fila_normal);
    destruir_fila(&fila_prioritaria);

    return 0;
}

