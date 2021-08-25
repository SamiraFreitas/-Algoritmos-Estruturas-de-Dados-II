#include "intercalacao.h"
#include "funcionarios.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "arvore_binaria.h"

void intercalacao_basico(char *nome_arquivo_saida, int num_p, Lista *nome_particoes)
{

    int fim = 0; //variavel que controla fim do procedimento
    FILE *out; //declara ponteiro para arquivo

    //abre arquivo de saida para escrita
    if((out = fopen(nome_arquivo_saida, "wb")) == NULL)
    {
        printf("Erro ao abrir arquivo de saida\n");
    }
    else
    {
        //cria vetor de particoes
        TVet v[num_p];

        //abre arquivos das particoes, colocando variavel de arquivo no campo f do vetor
        //e primeiro funcionario do arquivo no campo func do vetor
        for (int i=0; i < num_p; i++)
        {
            v[i].f = fopen(nome_particoes->nome, "rb");
            v[i].aux_p = 0;
            if (v[i].f != NULL)
            {
                fseek(v[i].f, v[i].aux_p * tamanho_registro(), SEEK_SET);
                TFunc *f = le_funcionario(v[i].f);
                if (f == NULL)
                {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posi??o do vetor
                    v[i].func = funcionario(INT_MAX, "","","",0);
                }
                else
                {
                    //conseguiu ler funcionario, coloca na posi??o atual do vetor
                    v[i].func = f;
                }
            }
            else
            {
                fim = 1;
            }
            nome_particoes = nome_particoes->prox;
        }

        int aux = 0;
        while (!(fim))   //conseguiu abrir todos os arquivos
        {
            int menor = INT_MAX;
            int pos_menor;
            //encontra o funcionario com menor chave no vetor
            for(int i = 0; i < num_p; i++)
            {
                if(v[i].func->cod < menor)
                {
                    menor = v[i].func->cod;
                    pos_menor = i;
                }
            }
            if (menor == INT_MAX)
            {
                fim = 1; //terminou processamento
            }
            else
            {
                //salva funcionario no arquivo de saída
                fseek(out, aux * tamanho_registro(), SEEK_SET);
                salva_funcionario(v[pos_menor].func, out);
                //atualiza posição pos_menor do vetor com pr?ximo funcionario do arquivo
                v[pos_menor].aux_p++;
                fseek(v[pos_menor].f, v[pos_menor].aux_p * tamanho_registro(), SEEK_SET);
                TFunc *f = le_funcionario(v[pos_menor].f);
                aux++;
                if (f == NULL)
                {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posiçao do vetor
                    v[pos_menor].func = funcionario(INT_MAX, "", "", "",0.0);
                }
                else
                {
                    v[pos_menor].func = f;
                }

            }
        }

        //fecha arquivos das partiÇões de entrada
        for(int i = 0; i < num_p; i++)
        {
            fclose(v[i].f);
            //    free(v[i].func);
        }
        //fecha arquivo de saída
        fclose(out);
    }
}


void intercalacao_arvore_de_vencedores(TPilha **pilha, int *vetTop, char *nome_arquivo_saida, int num_p){
    FILE *out; // declara ponteiro para arquivo

    // abre arquivo de saida para escrita

    out = fopen(nome_arquivo_saida, "wb");
    TNoA *arvore = (TNoA *) malloc(sizeof(TNoA));
    inserir_arvore_binaria_RAIZ(arvore, INT_MAX, 0);
    TNoA *vetNo[num_p];
    TFunc funcionarios[num_p * ((*vetTop) + 1)];
    int vetorOrdem[num_p * (*vetTop) + 1];
    int lista_registros[2][10];

    for (int i = 0; i < num_p; i+=2){
        TNoA *aux = (TNoA *) malloc(sizeof(TNoA));
        TFunc *func = (TFunc *) malloc(sizeof(TFunc));
        pilha[i]->p = (*vetTop);
        func = pop(pilha[i], 0, &pilha[i]->p);
        funcionarios[func->cod-1] = (*func);
        aux->esq = criaNo_arvore_binaria(func->cod, i);
        lista_registros[0][i] = func->cod;
        pilha[i+1]->p = (*vetTop);
        func = pop(pilha[i+1], 0, &pilha[i+1]->p);
        funcionarios[func->cod-1] = (*func);
        aux->dir = criaNo_arvore_binaria(func->cod, i+1);
        lista_registros[0][i+1] = func->cod;
        escreve_raiz(aux);
        vetNo[(i/2)] = aux;
    }

    if (num_p % 2 != 0){
        TNoA *aux = (TNoA *) malloc(sizeof(TNoA));
        TFunc *func = (TFunc *) malloc(sizeof(TFunc));

        pilha[num_p]->p = (*vetTop);
        func = pop(pilha[num_p], 0, &pilha[num_p]->p);
        funcionarios[func->cod-1] = (*func);
        lista_registros[0][num_p] = func->cod;

        aux->esq = criaNo_arvore_binaria(func->cod, num_p);

        vetNo[(num_p/2) - 1] = aux;
    }

    int inteiro = (num_p/2);
    int j = 0;

    for (int i = (num_p/2); i < num_p; i++){
        TNoA *aux = (TNoA *) malloc(sizeof(TNoA));
        if (inteiro - j > 1){
            aux->esq = vetNo[j];
            aux->dir = vetNo[j+1];
            escreve_raiz(aux);
            vetNo[i] = aux;
        }
        else{
            aux->info = vetNo[j]->info;
            aux->rrn = vetNo[j]->rrn;
            aux->dir = vetNo[j];
            vetNo[i] = aux;
            inteiro = i + 1;
            j--;
        }
        j+=2;
    }
    arvore->esq = vetNo[num_p - 2];
    arvore->dir = vetNo[num_p - 1];
    escreve_raiz(arvore);

    for(int i = 0; i < 10 ; i++){
        TFunc *func = (TFunc *) malloc(sizeof(TFunc));
        func = pop(pilha[i], 0, &pilha[i]->p);
        funcionarios[func->cod-1] = (*func);
        lista_registros[1][i] = func->cod;
    }

    while (arvore->info != INT_MAX){
        int k;

        fseek(out, (arvore->info-1) * tamanho_registro(), SEEK_SET);
        salva_funcionario(&funcionarios[arvore->info-1], out);
        for (k = 0; k < num_p; k++){
            if(lista_registros[0][k] == arvore->info){
                break;
            }
        }
        lista_registros[0][k] = lista_registros[1][k];
        lista_registros[1][k] = INT_MAX;

        TNoA *aux = (TNoA *) malloc(sizeof(TNoA));

        if(arvore->info == arvore->esq->info){
            aux = arvore->esq;
        }
        else{
            aux = arvore->dir;
        }

        while(!(busca_arvore_binaria(arvore, vetorOrdem[j])) || (aux->esq != NULL && aux->dir != NULL)){
            if (aux->esq != NULL && aux->dir != NULL){
                if(aux->info == aux->esq->info){
                    if(lista_registros[0][k] < aux->dir->info){
                        aux->info = lista_registros[0][k];
                    }
                    else{
                        aux->info = aux->dir->info;
                    }
                    aux = aux->esq;
                }
                else{
                    if(lista_registros[0][k] < aux->esq->info){
                        aux->info = lista_registros[0][k];
                    }
                    else{
                        aux->info = aux->esq->info;
                    }
                    aux = aux->dir;
                }
            }
            else if (aux->esq != NULL){
                aux->info = lista_registros[0][k];
                aux = aux->esq;
            }
            else if (aux->dir != NULL){
                aux->info = lista_registros[0][k];
                aux = aux->dir;
            }
            else{
                break;
            }
        }
        escreve_raiz(arvore);
        aux->info = lista_registros[0][k];
        ordena_arvore(arvore);
    }

    fclose(out);

}
