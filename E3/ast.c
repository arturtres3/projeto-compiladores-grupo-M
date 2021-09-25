#include <stdio.h>
#include <string.h>
#include <stdlib.h>

AST* novoNodo(char *data){
    AST* node = (AST*)malloc(sizeof(AST));
	node->a = malloc(strlen(data) * sizeof(char));
    strcpy(node->label, data)
    node->prim_filho = NULL;
    node->prim_irmao = NULL;

    return node;
}

void adicionaFilhos(AST* pai, AST* lista_filhos[], int num_filhos){
    int i = 0;

    if(pai == NULL || lista_filhos == NULL)
        return;

	//todos os filhos da lista viram irmaos
    for(i = 0; i < num_filhos-1; i++){
        lista_filhos[i]->prim_irmao = lista_filhos[i+1];
	}

    pai->prim_filho = lista_filhos[0];

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

    printf("%p = [label=\"%s\"]\n", node, node->a);

    printValEnderecos(node->prim_filho);

    printValEnderecos(node->prim_irmao);

}

void libera(AST* node){
    if (node == NULL)
        return;

    libera(node->prim_filho);
    libera(node->prim_irmao);

	free(node->a);
    free(node);
}


