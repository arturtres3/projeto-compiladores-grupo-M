#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tabela.h"
#include "errors.h"

void liberaListaVar(lista_var* lista){

    if(lista == NULL)
        return;

    liberaListaVar(lista->prox);

    free(lista->nome);
    free(lista);

}

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

tabela_simbolos* foiDeclarado(char* chave, tabela_simbolos* tabela){

    while(tabela != NULL){
        if(strcmp(tabela->chave, chave) == 0)
            return tabela;
        tabela = tabela->prox;
    }

    return NULL;

}

lista_var* novoListaVar(lista_var* lista, char* nome, int tamanho, int linha, int vetor, valor_lexico valor, enum_Tipo tipo){

    lista_var* nova = (lista_var*)malloc(sizeof(lista_var));

    nova->nome = strdup(nome);
    nova->tamanho = tamanho;
    nova->linha = linha;
    nova->vetor = vetor;
    nova->valor = valor;
    nova->prox = NULL;
    nova->tipo = tipo;

    if(lista == NULL)  
        return nova;

    lista_var* aux = lista;
    while(aux->prox != NULL)
            aux = aux->prox;

    aux->prox = nova;

    return lista;
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

tabela_simbolos* novaEntradaTabelaFunc(char* chave, int linha, enum_Natureza natureza, enum_Tipo tipo, valor_lexico valor, int tamanho, Parametro *lista_par){
    tabela_simbolos* nova_tabela = (tabela_simbolos*)malloc(sizeof(tabela_simbolos));

    nova_tabela->chave = strdup(chave);
    nova_tabela->num_linha = linha;
    nova_tabela->natureza = natureza;
    nova_tabela->tipo = tipo;
    nova_tabela->tamanho = tamanho * bytes_por_tipo(tipo);
    nova_tabela->valor = valor;

    nova_tabela->lista_param = copiaParametros(lista_par);

    nova_tabela->prox = NULL;

    return  nova_tabela;
}

tabela_simbolos* novaEntradaTabela(char* chave, int linha, enum_Natureza natureza, enum_Tipo tipo, valor_lexico valor, int tamanho){

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

tabela_simbolos* adicionaEntradaTabelaFunc(tabela_simbolos* escopo_atual, char* chave, int linha, enum_Natureza natureza, enum_Tipo tipo, valor_lexico valor, int tamanho, Parametro *lista_par){

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

tabela_simbolos* adicionaEntradaTabela(tabela_simbolos* escopo_atual, char* chave, int linha, enum_Natureza natureza, enum_Tipo tipo, valor_lexico valor, int tamanho){

    tabela_simbolos* declarado = foiDeclarado(chave, escopo_atual);
    if(declarado != NULL){
        mensagemErro(ERR_DECLARED, linha, declarado);
    }

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

tabela_simbolos* adicionaListaVar(tabela_simbolos* escopo_atual, lista_var* variaveis, enum_Tipo tipo){

    //tabela_simbolos* saida = NULL;
    while(variaveis != NULL){
        enum_Natureza natur;
        if(variaveis->vetor == 1){
            natur = VETOR;
        }else{
            natur =  VAR;
        }

        if(variaveis->tipo == TIPO_NA){
            escopo_atual = adicionaEntradaTabela(escopo_atual, variaveis->nome, variaveis->linha, natur, tipo, variaveis->valor, variaveis->tamanho);
        }else{
            escopo_atual = adicionaEntradaTabela(escopo_atual, variaveis->nome, variaveis->linha, natur, variaveis->tipo, variaveis->valor, variaveis->tamanho);
        }
        variaveis = variaveis->prox;
        
    }

    return escopo_atual;

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

    pilha_tabela* aux = pilha->prox;

    free(pilha);

    return aux;

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

Parametro* copiaParametros(Parametro* lista_par){
    Parametro* nova_lista = NULL;
    Parametro* aux = lista_par;

    while(aux != NULL){
        nova_lista = novoParametro(nova_lista, aux->tipo);
        aux = aux->prox;
    }

    return nova_lista;
}

void printParams(Parametro* lista_par){
    if(lista_par == NULL)
        return;

    printf("%d", lista_par->tipo);

    printParams(lista_par->prox);
}

void printListaVar(lista_var* lista){
    if(lista == NULL)
        return;

    printf("%s, ", lista->nome);

    printListaVar(lista->prox);
}

void mensagemErro(int erro, int linha, void* ref1){

    printf("[ERRO] Erro semantico encontrado na linha: %d\n", linha);

    switch (erro){
        case ERR_UNDECLARED:
			printf("[ERRO] Variavel nao declarada.\n");  
            break;
        case ERR_DECLARED:
            printf("[ERRO] Variavel ja declarada.\n");
            printf("[ERRO] Primeira declaracao na linha: %d\n", ((tabela_simbolos *)ref1)->num_linha);
            break;
        case ERR_VARIABLE:
            printf("[ERRO] Variavel nao pode ser usada como vetor ou funcao.\n");
            break;
        case ERR_VECTOR:
            printf("[ERRO] Vetor nao pode ser usado como variavel ou funcao.\n");
            break;
        case ERR_FUNCTION:
            printf("[ERRO] Funcao nao pode ser usada como variavel ou vetor.\n");
            break;
        case ERR_WRONG_TYPE:
			printf("[ERRO] Atribuicao de tipo incompativel.\n");
            break;
        case ERR_STRING_TO_X:
            printf("[ERRO] Strings nao podem ser convertidas para outro tipo.\n");
            break;
        case ERR_CHAR_TO_X:
            printf("[ERRO] Characteres nao podem ser convertidos para outro tipo.\n");
            break;
        case ERR_MISSING_ARGS:
            printf("[ERRO] Chamada de funcao com numero insuficiente de argumentos.\n");
            break;
        case ERR_EXCESS_ARGS:
            printf("[ERRO] Chamada de funcao com numero excessivo de argumentos.\n");
            break;
        case ERR_WRONG_TYPE_ARGS:
            printf("[ERRO] Argumento com tipo incompativel na chamada de funcao.\n");
            break;
        case ERR_WRONG_PAR_INPUT:
            printf("[ERRO] Comando input aceita apenas identificadores dos tipos int ou float.\n");
            break;
        case ERR_WRONG_PAR_OUTPUT:
            printf("[ERRO] Comando output aceita apenas identificadores ou literais dos tipos int ou float.\n");
            break;
        case ERR_WRONG_PAR_RETURN:
            printf("[ERRO] Retorno incompativel com o tipo declarado da funcao.\n");
            break;
        case ERR_WRONG_PAR_SHIFT:
            printf("[ERRO] Comando de shift nao aceita numeros maiores que 16.\n");
            break;
        default:
            
            break;
        }

        printf("Terminando execucao com codigo de status: %d \n\n", erro);

        exit(erro);

}
