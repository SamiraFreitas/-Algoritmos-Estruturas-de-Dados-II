#include <stdio.h>
#include <stdlib.h>
#include "arvore_vencedores.h"
#include "funcionarios.h"
#include "pilha.h"



TNo_vencedor *cria_arvore_vencedores_inicial(TPilha **pilha, int *vetTop, int num_p, TNo_vencedor **backup_p){
    int i;
    TFila fila;
    FFVazia(&fila);
    TItem x;
    for(i = 0; i < num_p; i++){
        TFunc *aux= (TFunc *) malloc(sizeof(TFunc));
        aux = pop(pilha[i], 0, &vetTop[i]);
        x.no = cria_folha_arvore_vencedora(aux, i);
        Enfileira(&fila, x);
        backup_p[i] = x.no;
    }
    imprime_fila(fila);
    TNo_vencedor *raiz = (TNo_vencedor*) malloc(sizeof(TNo_vencedor));
    while(fila.Frente->Prox->Prox!=NULL){
        TItem *aux1 = (TItem*) malloc(sizeof(TItem));
        TItem *aux2 = (TItem*) malloc(sizeof(TItem));
        TItem *aux3 = (TItem*) malloc(sizeof(TItem));
        Desenfileira(&fila, aux1);
        Desenfileira(&fila, aux2);
        if(aux1->no->vencedor->cod < aux2->no->vencedor->cod){
            aux3->no = cria_pai_arvore_vencedora(aux1->no->vencedor, aux1->no->posicao, aux1->no, aux2->no);
            Enfileira(&fila, *aux3);
        }
        else{
            aux3->no = cria_pai_arvore_vencedora(aux2->no->vencedor, aux2->no->posicao, aux2->no, aux1->no);
            Enfileira(&fila, *aux3);
        }
        if(fila.Frente->Prox->Prox == NULL){
            raiz= aux3->no;
        }
    }
    return raiz;
}

TNo_vencedor *cria_folha_arvore_vencedora(TFunc *func, int posicao){
    TNo_vencedor *no = (TNo_vencedor*)malloc(sizeof(TNo_vencedor));
    no->vencedor = func;
    no->posicao = posicao;
    no->pai = NULL;
    no->esq = NULL;
    no->dir = NULL;
    return no;
}

TNo_vencedor *cria_pai_arvore_vencedora(TFunc *pai, int posicao, TNo_vencedor *esq, TNo_vencedor *dir){
    TNo_vencedor *no = (TNo_vencedor*)malloc(sizeof(TNo_vencedor));
    no->vencedor = pai;
    no->posicao = posicao;
    no->pai = NULL;
    no->dir = dir;
    no->esq = esq;
    esq->pai = no;
    dir->pai = no;
    return no;
}

void reorganiza_arvore(TNo_vencedor *raiz, TPilha **pilha, int *vetTop, int num_p, TNo_vencedor **backup_endereco_p){

    TFunc *aux = (TFunc*)malloc(sizeof(TFunc));
    TNo_vencedor *no, *no2;
    if(vetTop[raiz->posicao]>=0){
        aux = pop(pilha[raiz->posicao], 0, &vetTop[raiz->posicao]);
        no = backup_endereco_p[raiz->posicao];
        *(no->vencedor) = *(aux);
        no->posicao = raiz->posicao;
        ordena_arvore_vencedora(raiz, no);
    }
    else{
        no2 = backup_endereco_p[raiz->posicao];
        no2 = no2->pai;
        int a = raiz->posicao+1;
        no2->vencedor = backup_endereco_p[raiz->posicao+1]->vencedor;
        no2->posicao = backup_endereco_p[raiz->posicao+1]->posicao;
        if(no2->dir == backup_endereco_p[raiz->posicao]){
            no2->dir = NULL;
        }
        if(no2->esq == backup_endereco_p[raiz->posicao]){
            no2->esq = NULL;
        }
        ordena_arvore_vencedora(raiz, no2);

    }
}

void ordena_arvore_vencedora(TNo_vencedor* raiz, TNo_vencedor *pos){
    TNo_vencedor *aux;
    aux = pos->pai;
    while(aux!=raiz){
        if(aux->dir->vencedor ==NULL && aux->esq->vencedor==NULL){
            aux = NULL;
        }

        else if (aux->dir->vencedor ==NULL){
            *(aux->vencedor) = *(aux->esq->vencedor);
            aux->posicao = aux->esq->posicao;
        }
        else if (aux->esq->vencedor == NULL){
            *(aux->vencedor) = *(aux->dir->vencedor);
            aux->posicao = aux->esq->posicao;
        }
        else{
            if(aux->esq->vencedor > aux->dir->vencedor){
                *(aux->vencedor) = *(aux->dir->vencedor);
                aux->posicao = aux->dir->posicao;
            }
            else{
                *(aux->vencedor) = *(aux->esq->vencedor);
                aux->posicao = aux->esq->posicao;
            }
        }
        aux = aux->pai;
    }
}





void FFVazia(TFila *Fila){
    Fila->Frente = (TApontador) malloc(sizeof(TCelula));
    Fila->Tras = Fila->Frente;
    Fila->Frente->Prox = NULL;
}

int FVazia(TFila Fila){
    return(Fila.Frente == Fila.Tras);
}

void Enfileira(TFila *Fila, TItem x){
    Fila->Tras->Prox= (TApontador)malloc(sizeof(TCelula));
    Fila->Tras = Fila->Tras->Prox;
    Fila->Tras->Item = x;
    Fila->Tras->Prox = NULL;
}

void Desenfileira(TFila *Fila, TItem *item){
    TApontador q;
    if(FVazia(*Fila)){
        return;
    }
    q = Fila->Frente;
    Fila->Frente = Fila->Frente->Prox;
    *item = Fila->Frente->Item;
    free(q);
}
void imprime_fila(TFila fila){
    TApontador aux;
    aux = fila.Frente->Prox;
    while(aux != NULL){
        printf("%d/", aux->Item.no->vencedor->cod);
        aux = aux->Prox;
    }
}

void imprime_arvore_vencedora(TNo_vencedor *no){
    if(no == NULL){
        return;
    }
    imprime_arvore_vencedora(no->esq);
    printf("%d,", no->vencedor->cod);
    imprime_arvore_vencedora(no->dir);
}
