#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"

AST* novoNodo(char *data){
    AST* node = (AST*)malloc(sizeof(AST));
	node->label = strdup(data);
    node->prim_filho = NULL;
    node->prim_irmao = NULL;

    return node;
}

void adicionaFilhos(AST* pai, AST* lista_filhos[], int num_filhos){
    int i = 0;

    if(pai == NULL)
        return;

	//todos os filhos da lista viram irmaos (se num_filhos<2 nao entra no for)
    for(i = 0; i < num_filhos-1; i++){
        lista_filhos[i]->prim_irmao = lista_filhos[i+1];
	}

    pai->prim_filho = lista_filhos[0];

}

void appendFilho(AST* pai, AST* novo_filho){
    if(pai == NULL || novo_filho == NULL)
        return;

    AST* filho = pai->prim_filho;

    while(filho != NULL && filho->prim_irmao != NULL)
        filho = filho->prim_irmao;

    filho->prim_irmao = novo_filho;
}

AST* cria_e_adiciona(char *data, AST** lista_filhos, int num_filhos){

    AST* node = novoNodo(data);

    adicionaFilhos(node, lista_filhos, num_filhos);

    return node;
}

void printIrmaos(AST* node, AST* pai){

    if (node == NULL)
        return;

    printf("%p, %p\n", pai, node);

    printIrmaos(node->prim_irmao, pai);
}

void printEnderecos(AST* node){

    if (node == NULL)
        return;

    printIrmaos(node->prim_filho, node);

    printEnderecos(node->prim_filho);
    printEnderecos(node->prim_irmao);
}

void printValEnderecos(AST* node){

    if (node == NULL)
        return;

    printf("%p [label=\"%s\"]\n", node, node->label);

    printValEnderecos(node->prim_filho);

    printValEnderecos(node->prim_irmao);

}

void printPreorder(AST* node){
    if (node == NULL)
        return;

    printf("%s ", node->label);

    printPreorder(node->prim_filho);

    printPreorder(node->prim_irmao);
}

void exporta(AST* arvore){
	printEnderecos(arvore);
	printValEnderecos(arvore);
}

void libera(AST* node){
    if (node == NULL)
        return;

    libera(node->prim_filho);
    libera(node->prim_irmao);

	free(node->label);
    free(node);
}
