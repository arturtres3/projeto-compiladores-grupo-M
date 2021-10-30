#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/tipos.h"
#include "include/ILOC.h"


char* geraReg(){
    static int numeroReg;
    int n = numeroReg;
    numeroReg++;

    char* resultado = NULL;
    char* erre = "r";
    char* num = int_to_string(n);

    int length = snprintf( NULL, 0, "%s%s", erre, num);

    resultado = malloc( (length + 1)* sizeof(char));

    strcpy(resultado, erre);
    strcat(resultado, num);
    free(num);

    //printf("\nlength %d\nsizeof %d\nstrlen %d\n", length, sizeof(resultado), strlen(resultado));

    return resultado;
}


char* copiaEnd(char* str){ //strdup quebra se passar nulo
    char* saida;

    if(str != NULL){
        saida = strdup(str);
    }else{
        saida = NULL;
    }


    return saida;
}


void liberaILOC(codILOC* lista){

    if(lista == NULL)
        return;

    liberaILOC(lista->prox);

    free(lista->end1);
    free(lista->end2);
    free(lista->dest);

    free(lista);
}


codILOC* novoILOC(opILOC op, char* end1, char* end2, char* dest){

    codILOC* novo = (codILOC*)malloc(sizeof(codILOC));

    novo->op = op;
    novo->end1 = copiaEnd(end1);
    novo->end2 = copiaEnd(end2);
    novo->dest = copiaEnd(dest);

    novo->prox = NULL;

    return novo;
}


codILOC* appendCod(codILOC* lista, codILOC* novo){

    codILOC* aux = lista;

    if(aux != NULL){
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = novo;
        return lista;
    }

    return novo;

}


void imprimeCod(codILOC* cod){

    char* opcode;

    switch (cod->op) {
        case loadI_OP:
            opcode = "loadI";
            break;
        case loadAI_OP:
            opcode = "loadAI";
            break;
        case i2i_OP:
            opcode = "i2i";
            break;
        case storeAI_OP:
            opcode = "storeAI";
            break;
        case jump_OP:
            opcode = "jump";
            break;
        case cbr_OP:
            opcode = "cbr";
            break;
        case add_OP:
            opcode = "add";
            break;
        case sub_OP:
            opcode = "sub";
            break;
        case mult_OP:
            opcode = "mult";
            break;
        case div_OP:
            opcode = "div";
            break;
        case addI_OP:
            opcode = "addI";
            break;
        case subI_OP:
            opcode = "subI";
            break;
        case multI_OP:
            opcode = "multI";
            break;
        case divI_OP:
            opcode = "divI";
            break;
        case and_OP:
            opcode = "and";
            break;
        case or_OP:
            opcode = "or";
            break;
        case cmp_LE_OP:
            opcode = "cmp_LE";
            break;
        case cmp_LT_OP:
            opcode = "cmp_LT";
            break;
        case cmp_GE_OP:
            opcode = "cmp_GE";
            break;
        case cmp_GT_OP:
            opcode = "cmp_GT";
            break;
        case cmp_EQ_OP:
            opcode = "cmp_EQ";
            break;
        case cmp_NE_OP:
            opcode = "cmp_NE";
            break;
        case rotulo_OP:
            // Skip
            break;
        default:
            printf("ERROR: Invalid operation.\n");
            exit(1);
            break;
    }

    if(cod->op == rotulo_OP){
        printf("\n%s: ", cod->end1);
    }else{
        if(cod->end2 == NULL && cod->dest == NULL){
            printf("%s => %s\n", opcode, cod->end1);
        }else{
            if(cod->end2 != NULL){
                printf("%s %s, %s => %s\n", opcode, cod->end1, cod->end2, cod->dest);
            }else{
                printf("%s %s => %s\n", opcode, cod->end1, cod->dest);
            }
        }
    }

}