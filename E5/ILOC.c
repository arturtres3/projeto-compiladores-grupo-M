#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/tipos.h"
#include "include/ILOC.h"


char* geraReg(LISTA_PTR** lista){ 
    static int numeroReg;
    int n = numeroReg;
    numeroReg++;

    char* resultado = NULL;
    char* erre = "r";
    char* num = int_to_string(n);

    int length = snprintf( NULL, 0, "%s%s", erre, num);

    resultado = malloc( (length + 1)* sizeof(char));
    novoPTR(resultado, lista);

    strcpy(resultado, erre);
    strcat(resultado, num);
    free(num);

    //printf("\nlength %d\nsizeof %d\nstrlen %d\n", length, sizeof(resultado), strlen(resultado));

    return resultado;
}


char* geraLabel(LISTA_PTR** lista){
    static int numeroLabel = 1;
    int n = numeroLabel;
    numeroLabel++;

    char* resultado = NULL;
    char* elle = "L";
    char* num = int_to_string(n);

    int length = snprintf( NULL, 0, "%s%s", elle, num);

    resultado = malloc( (length + 1)* sizeof(char));
    novoPTR(resultado, lista);

    strcpy(resultado, elle);
    strcat(resultado, num);
    free(num);

    return resultado;
}


void fazRemendo(LISTA_PTR* lista, char* label){
    LISTA_PTR* aux = lista;
    while(aux != NULL){
        
        strcpy(aux->ptr, label);

        aux->ptr = NULL;
        
        aux = aux->prox;
    }

    liberaStructPTR(lista);

}


int deslocGlobal(){
    static int deslocamento;
    int n = deslocamento;
    deslocamento = deslocamento + 4;

    return n;
}


int deslocLocal(int reset){
    static int deslocamento;

    int n = deslocamento;
    if(reset == 1){
        deslocamento = 0;
        return 0;
    }else{
        deslocamento = deslocamento + 4;
    }

    return n;
}


void liberaILOC(codILOC* lista){

    if(lista == NULL)
        return;

    liberaILOC(lista->prox);

    free(lista->end1);
    free(lista->end2);
    free(lista->dest);

    free(lista);
    lista = NULL;
}


codILOC* novoILOC(opILOC op, char* end1, char* end2, char* dest){

    codILOC* novo = (codILOC*)malloc(sizeof(codILOC));

    novo->op = op;
    novo->end1 = copiaStr(end1);
    novo->end2 = copiaStr(end2);
    novo->dest = copiaStr(dest);

    novo->prox = NULL;

    return novo;
}


void appendCod(codILOC** lista, codILOC* novo){

    codILOC* aux = *lista;

    if(aux != NULL){
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = novo;
    }else{
        *lista = novo;
    }

}


void adicionaILOC(codILOC** lista, opILOC op, char* end1, char* end2, char* dest){

    codILOC* codigo = novoILOC(op, end1, end2, dest);

    appendCod(lista, codigo);

}


codILOC* ultimoILOC(codILOC* lista){
    if(lista != NULL){
        while(lista->prox != NULL){
            lista = lista->prox;
        }
    }
    return lista;

}


void declaraVarLocais(codILOC** lista, int num_vars){
    while(num_vars > 0){
        adicionaILOC(lista, addI_OP, "rsp", "4", "rsp");
        num_vars--;
    }
}


int contaILOC(codILOC* lista){
    int soma = 0;

    while(lista != NULL){
        soma++;
        lista = lista->prox;
    }

    return soma;
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
        case jumpI_OP:
            opcode = "jumpI";
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
        case halt_OP:
            opcode = "halt";
            break;
        case rotulo_OP:
            // Skip
            break;
        default:
            printf("ERROR: Invalid operation.\n");
            exit(1);
            break;
    }

    if(cod->op == halt_OP){
        printf("%s", cod->end1);
    }else if(cod->op == rotulo_OP){
        printf("%s: ", cod->end1);
    }else{
        if(cod->end2 == NULL && cod->dest == NULL){
            if(cod->op == jump_OP || cod->op == jumpI_OP){
                printf("%s -> %s\n", opcode, cod->end1);
            }else{
                printf("%s => %s\n", opcode, cod->end1);
            }
        }else{
            if(cod->end2 != NULL){
                if(cod->op == storeAI_OP){
                    printf("%s %s => %s, %s\n", opcode, cod->end1, cod->end2, cod->dest);

                }else if(cod->op == cbr_OP){
                    printf("%s %s -> %s, %s\n", opcode, cod->end1, cod->end2, cod->dest);

                }else if(cod->op >= cmp_EQ_OP && cod->op <= cmp_GE_OP){
                    printf("%s %s, %s -> %s\n", opcode, cod->end1, cod->end2, cod->dest);

                }else{
                    printf("%s %s, %s => %s\n", opcode, cod->end1, cod->end2, cod->dest);
                }
            }else{
                    printf("%s %s => %s\n", opcode, cod->end1, cod->dest);
            }
        }
    }

}


void exportaILOC(codILOC* programa){
    codILOC* aux = programa;
    int linhas = 0;

    while(aux != NULL){
        if(aux->op != rotulo_OP){
            linhas++;
        }
        aux = aux->prox;
    }
        

    printf("loadI 1024 => rfp\nloadI 1024 => rsp\n");
    printf("loadI %d => rbss\n", linhas + 6);
    printf("jumpI -> L0\n");


    while(programa != NULL){

        imprimeCod(programa);

        programa = programa->prox;

    }

    printf("halt");
}