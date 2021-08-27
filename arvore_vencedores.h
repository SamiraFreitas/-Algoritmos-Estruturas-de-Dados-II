#include <stdio.h>
#include "funcionarios.h"
#include "pilha.h"


typedef struct No_vencedor{
    TFunc *vencedor;
    int posicao;
    struct No_vencedor *pai;
    struct No_vencedor *esq;
    struct No_vencedor *dir;
}TNo_vencedor;

typedef struct TCelula *TApontador;
typedef struct TipoItem{
    TNo_vencedor *no;
}TItem;

typedef struct TCelula{

    TItem Item;
    TApontador Prox;
}TCelula;

typedef struct TFila{
    TApontador Frente, Tras;
}TFila;

TNo_vencedor *cria_arvore_vencedores_inicial(TPilha **pilha, int *vetTop, int num_p, TNo_vencedor **backup_p);
TNo_vencedor *cria_folha_arvore_vencedora(TFunc *func, int posicao);
TNo_vencedor *cria_pai_arvore_vencedora(TFunc *pai, int posicao, TNo_vencedor *esq, TNo_vencedor *dir);
void imprime_arvore_vencedora(TNo_vencedor *no);
void reorganiza_arvore(TNo_vencedor *raiz, TPilha **pilha, int *vetTop, int num_p, TNo_vencedor **backup_endereco_p);
void ordena_arvore_vencedora(TNo_vencedor *raiz, TNo_vencedor *pos);


void FFVazia(TFila *Fila);
int FVazia(TFila Fila);
void Enfileira(TFila *Fila, TItem x);
void Desenfileira(TFila *Fila, TItem *item);
void imprime_fila(TFila fila);
