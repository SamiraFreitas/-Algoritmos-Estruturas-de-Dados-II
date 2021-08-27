
#include "funcionarios.h"
#include <stdlib.h>
#include <limits.h>

#include "particoes.h"


void classificacao_interna(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc) {
    rewind(arq); //posiciona cursor no inicio do arquivo


    int reg = 0;

    while (reg != nFunc) {
        //le o arquivo e coloca no vetor
        TFunc *v[M];
        int i = 0;
        while (!feof(arq)) {
            fseek(arq, (reg) * tamanho_registro(), SEEK_SET);
            v[i] = le_funcionario(arq);
       //     imprime_funcionario(v[i]);
            i++;
            reg++;
            if(i>=M) break;
        }

        //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
        if (i != M) {
            M = i;
        }

        //faz ordenacao
        for (int j = 1; j < M; j++) {
            TFunc *f = v[j];
            i = j - 1;
            while ((i >= 0) && (v[i]->cod > f->cod)) {
                v[i + 1] = v[i];
                i = i - 1;
            }
            v[i + 1] = f;
        }

        //cria arquivo de particao e faz gravacao
        char *nome_particao = nome_arquivos_saida->nome;
        nome_arquivos_saida = nome_arquivos_saida->prox;
        printf("\n%s\n", nome_particao);
        FILE *p;
        if ((p = fopen(nome_particao, "wb+")) == NULL) {
            printf("Erro criar arquivo de saida\n");
        } else {
            for (int i = 0; i < M; i++) {
                fseek(p, (i) * tamanho_registro(), SEEK_SET);
                salva_funcionario(v[i], p);
                imprime_funcionario(v[i]);
            }
            fclose(p);
        }
        for(int jj = 0; jj<M; jj++){
            free(v[jj]);
        }
    }
}

void selecao_natural(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc, int n){

    int reg = 0;
    //pos_menor=menor_pos(*arq, M, nFunc);
    //posicionando o cursor
    rewind(arq);
    //lendo o arquivo


    while (reg != nFunc) {
        //le o arquivo e coloca no vetor
        TFunc *v[M];
        int i = 0;
        while (!feof(arq)) {

            fseek(arq, (reg) * tamanho_registro(), SEEK_SET);
            v[i] = le_funcionario(arq);
            //imprime_funcionario(v[i]);


            reg++;
            //printf("1");

            if(i==M){
                //printf("2");
                //cria arquivo de particao e faz gravacao
                char *nome_particao = nome_arquivos_saida->nome;
                nome_arquivos_saida = nome_arquivos_saida->prox;
                printf("\n%s\n", nome_particao);
                FILE *p;
                if ((p = fopen(nome_particao, "wb+")) == NULL) {
                        printf("Erro criar arquivo de saida\n");
                    //printf("3");
                    }
                else {

                    //printf("\ni=%d\n",y);

                if (v[i]->cod>v[(i-1)]->cod){
                        for(int o=0;o<=M;o++){
                    fseek(p, (i) * tamanho_registro(), SEEK_SET);
                    salva_funcionario(v[(i+o)-1], p);
                    imprime_funcionario(v[(i+o)-1]);

                    //printf("5");

                }break;}
                  else if(v[i]->cod<v[(i)-1]->cod){
                        for(int o=0;o<=M;o++){

                    fseek(p, (i) * tamanho_registro(), SEEK_SET);
                    salva_funcionario(v[i-o], p);
                    imprime_funcionario(v[i-o]);

                    //printf("4");

                  }break;}
            }
            if(i>=M) break;

            //*********************************************************************************************
        }
        i++;
        //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
        if (i != M) {
            M = i;
            //printf("6");
        }

    }
    for(int jj = 0; jj<M; jj++){
            free(v[jj]);
        }
    }

}





