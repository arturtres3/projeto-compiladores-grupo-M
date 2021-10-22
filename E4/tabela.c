#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tabela.h"

void liberaParams(Parametro* lista_par){
    if(lista_par == NULL)
        return;

    liberaParams(lista_par->prox);

    free(lista_par);
    lista_par = NULL;
}

void liberaTabela(tabela_simbolos* tabela){

    if(tabela == NULL)
        return;

    liberaTabela(tabela->prox);

    if(tabela->lista_param != NULL)
        liberaParams(tabela->lista_param);

    free(tabela->chave);
    free(tabela);

}

void liberaPilha(pilha_tabela* pilha){
    if(pilha == NULL)
        return;

    liberaPilha(pilha->prox);
    liberaTabela(pilha->atual);

    free(pilha);
}

void printTabela(tabela_simbolos* tabela){

    if(tabela == NULL)
        return;

    printf("%s, ", tabela->chave);

    printTabela(tabela->prox);

}

void printPilha(pilha_tabela* pilha){
    if(pilha == NULL)
        return;

    printf("escopo: \n");
    printTabela(pilha->atual);
    printf("\n\n");

    printPilha(pilha->prox);

}

pilha_tabela* iniciaPilha(){
    pilha_tabela* novo = (pilha_tabela*)malloc(sizeof(pilha_tabela));

    novo->atual = (tabela_simbolos*)malloc(sizeof(tabela_simbolos));

    novo->atual = NULL;

    novo->prox = NULL;

    return novo;
}

Parametro* novoParametro(Parametro *lista_par, enum_Tipo tipo){

    Parametro* novo = (Parametro*)malloc(sizeof(Parametro));

    novo->tipo = tipo;
    novo->prox = NULL;

    if(lista_par == NULL)
        return novo;

    Parametro* aux = lista_par;
    while(aux->prox != NULL)
            aux = aux->prox;

    aux->prox = novo;

    return lista_par;

}

tabela_simbolos* novaEntradaTabelaFunc(char* chave, int linha, int natureza, enum_Tipo tipo, union Valores valor, int tamanho, Parametro *lista_par){
    tabela_simbolos* nova_tabela = (tabela_simbolos*)malloc(sizeof(tabela_simbolos));

    nova_tabela->chave = strdup(chave);
    nova_tabela->num_linha = linha;
    nova_tabela->natureza = natureza;
    nova_tabela->tipo = tipo;
    nova_tabela->tamanho = tamanho * bytes_por_tipo(tipo);
    nova_tabela->valor = valor;

    nova_tabela->lista_param = lista_par;

    nova_tabela->prox = NULL;

    return  nova_tabela;
}

tabela_simbolos* novaEntradaTabela(char* chave, int linha, int natureza, enum_Tipo tipo, union Valores valor, int tamanho){

    tabela_simbolos* nova_tabela = (tabela_simbolos*)malloc(sizeof(tabela_simbolos));

    nova_tabela->chave = strdup(chave);
    nova_tabela->num_linha = linha;
    nova_tabela->natureza = natureza;
    nova_tabela->tipo = tipo;
    nova_tabela->tamanho = tamanho * bytes_por_tipo(tipo);
    nova_tabela->valor = valor;

    nova_tabela->lista_param = NULL;

    nova_tabela->prox = NULL;

    return  nova_tabela;

}

tabela_simbolos* adicionaEntradaTabelaFunc(tabela_simbolos* escopo_atual, char* chave, int linha, int natureza, enum_Tipo tipo, union Valores valor, int tamanho, Parametro *lista_par){

    tabela_simbolos* nova = novaEntradaTabelaFunc(chave, linha, natureza, tipo, valor, tamanho, lista_par);

    if(escopo_atual == NULL){
        return nova;

    }else{
        tabela_simbolos* aux = escopo_atual;

        while(aux->prox != NULL)
            aux = aux->prox;

        aux->prox = nova;

        return escopo_atual;
    }

}

tabela_simbolos* adicionaEntradaTabela(tabela_simbolos* escopo_atual, char* chave, int linha, int natureza, enum_Tipo tipo, union Valores valor, int tamanho){

    tabela_simbolos* nova = novaEntradaTabela(chave, linha, natureza, tipo, valor, tamanho);

    if(escopo_atual == NULL){
        return nova;

    }else{
        tabela_simbolos* aux = escopo_atual;

        while(aux->prox != NULL)
            aux = aux->prox;

        aux->prox = nova;

        return escopo_atual;
    }

}

pilha_tabela* novoEscopo(pilha_tabela* topo){

    pilha_tabela* novo = (pilha_tabela*)malloc(sizeof(pilha_tabela));

    novo->atual = (tabela_simbolos*)malloc(sizeof(tabela_simbolos));
    novo->atual = NULL;
    novo->prox = topo;

    return novo;

}

pilha_tabela* fechaEscopo(pilha_tabela* pilha){

    liberaTabela(pilha->atual);

    return pilha->prox;

}

tabela_simbolos* procuraTabela(char* chave, tabela_simbolos* tabela){

    while(tabela != NULL){
        if(strcmp(tabela->chave, chave) == 0)
            return tabela;
        tabela = tabela->prox;
    }

    return NULL;

}

tabela_simbolos* encontraSimbolo(char* chave, pilha_tabela* pilha){

    if(pilha == NULL)
        return NULL;

    tabela_simbolos* resultado = procuraTabela(chave, pilha->atual);

    if(resultado != NULL)
        return resultado;

    encontraSimbolo(chave, pilha->prox);

}

void comparaParams(Parametro* lista1, Parametro* lista2){ // list1 = params definidos da func; lista2 = argumentos na chamada
    if(lista1 == NULL && lista2 == NULL){
        printf("OK");
        return;
    }

    if(lista1 == NULL && lista2 != NULL){
        printf("Excess Args");
        return;
    }

    if(lista1 != NULL && lista2 == NULL){
        printf("Missing Args");
        return;
    }

    if(lista1->tipo == lista2->tipo){
        comparaParams(lista1->prox, lista2->prox);
    }else{
        printf("incompativel");
        return;
    }



}

void printParams(Parametro* lista_par){
    if(lista_par == NULL)
        return;

    printf("%d", lista_par->tipo);

    printParams(lista_par->prox);
}


void kill_me(){
    printf("kill me    ");
}