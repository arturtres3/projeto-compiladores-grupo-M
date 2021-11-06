#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/ast.h"
#include "include/tipos.h"

AST* novoNodo(char *data, enum_Tipo tipo){
    AST* node = (AST*)malloc(sizeof(AST));
	node->label = strdup(data);

    node->local = NULL;
    node->codigo = NULL;
    node->l_false = NULL;
    node->l_true = NULL;

    node->tipo = tipo;
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
    if(pai == NULL)
        return;

    AST* filho = pai->prim_filho;

    while(filho != NULL && filho->prim_irmao != NULL)
        filho = filho->prim_irmao;

    if(filho != NULL){
        filho->prim_irmao = novo_filho;
    }else{
        pai->prim_filho = novo_filho;
    }
}

AST* cria_e_adiciona(char *data, AST** lista_filhos, int num_filhos, enum_Tipo tipo){

    AST* node = novoNodo(data, tipo);

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

    //printf("%p [label=\"%s\"] tipo: %c\n", node, node->label, charDoTipo(node->tipo));
    
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

AST* encontraNodo(AST* node, char *label){
    if (node == NULL)
        return NULL;

    if(strcmp(node->label, label) == 0)
        return node;

    AST* filhos = encontraNodo(node->prim_filho, label);
    if(filhos != NULL)
        return filhos;

    AST* irmaos = encontraNodo(node->prim_irmao, label);
    return irmaos;
}

AST* ultimoFilho(AST* node){
    AST* aux = node;

    if(aux != NULL){
        if(aux->prim_filho != NULL){
            aux = aux->prim_filho;
            while(aux != NULL && aux->prim_irmao != NULL){
                aux = aux->prim_irmao;
            }

            return aux;
        }
    }

    return NULL;
}

int ehPai(AST* raiz, AST* node){ //retorna 1 se raiz eh pai de node, 0 senao
    if (node == NULL || raiz == NULL)
        return 0;

    AST* percorre = raiz->prim_filho;
    while(percorre != NULL && percorre->prim_irmao != NULL){
        if(percorre == node)
            return 1;
        percorre = percorre->prim_irmao;
    }

    return 0;
}

AST* encontraPai(AST* raiz, AST* node){//retona ponteiro para pai do node se estiver em raiz, senao retorna nulo
    if (raiz == NULL)
        return NULL;

    if(ehPai(raiz, node)){
        return raiz;
    }

    AST* filhos = encontraPai(raiz->prim_filho, node);
    if(filhos != NULL)
        return filhos;

    AST* irmaos = encontraPai(raiz->prim_irmao, node);
    return irmaos;


}

void removeNodo(AST* raiz, AST* node){
    AST* pai = encontraPai(raiz, node);

    if(pai == NULL)
        return;

    AST* temp = pai->prim_filho;
    AST* irmao_anterior = NULL;
    if(pai->prim_filho == node){
        pai->prim_filho = temp->prim_irmao;
        libera(temp);
    }else{
        while(temp != node){
            temp = temp->prim_irmao;
            irmao_anterior = temp;
        }
        irmao_anterior->prim_irmao = temp->prim_irmao;
        libera(temp);
    }

}

void alteraNodo(AST* node, char* novo_valor){
    free(node->label);
    node->label = strdup(novo_valor);
}

void alteraTipoNodo(AST* node, enum_Tipo tipo){
    node->tipo = tipo;
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
