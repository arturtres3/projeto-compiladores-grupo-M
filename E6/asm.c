#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/tipos.h"
#include "include/asm.h"


void liberaASM(codASM* lista){

    if(lista == NULL)
        return;

    liberaASM(lista->prox);

    free(lista->end1);
    free(lista->end2);
    free(lista->dest);

    free(lista);
}


codASM* novoASM(opASM op, char* end1, char* end2, char* dest){

    codASM* novo = (codASM*)malloc(sizeof(codASM));

    novo->op = op;
    novo->end1 = copiaStr(end1);
    novo->end2 = copiaStr(end2);
    novo->dest = copiaStr(dest);

    novo->prox = NULL;

    return novo;
}


void appendASM(codASM** lista, codASM* novo){

    codASM* aux = *lista;

    if(aux != NULL){
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = novo;
    }else{
        *lista = novo;
    }

}


void adicionaASM(codASM** lista, opASM op, char* end1, char* end2, char* dest){

    codASM* codigo = novoASM(op, end1, end2, dest);

    appendASM(lista, codigo);

}


codASM* ultimoASM(codASM* lista){
    if(lista != NULL){
        while(lista->prox != NULL){
            lista = lista->prox;
        }
    }
    return lista;

}


int deslocASM(int reset){
    static int deslocamento = 8;

    int n = deslocamento;
    if(reset == 1){
        deslocamento = 8;
        return 0;
    }else{
        deslocamento = deslocamento + 8;
    }

    return n;
}


void imprimeASM(codASM* cod){

    char* opcode;
    int jump = 0;

    switch (cod->op) {

        case mov_to_mem:
            opcode = "movq";
            break;
        case mov_from_mem:
            opcode = "movq";
            break;
        case push_OP:
            opcode = "push";
            break;
        case pop_OP:
            opcode = "pop";
            break;
        case addq_OP:
            opcode = "addq";
            break;
        case imulq_OP:
            opcode = "imulq";
            break;
        case subq_OP:
            opcode = "subq";
            break;
        case idivq_OP:
            opcode = "idivq";
            break;
        case cmpq_OP:
            opcode = "cmpq";
            break;
        case jmp_OP:
            opcode = "jmp";
            jump = 1;
            break;
        case jl_OP:
            opcode = "jl";
            jump = 1;
            break;
        case jg_OP:
            opcode = "jg";
            jump = 1;
            break;
        case jle_OP:
            opcode = "jle";
            jump = 1;
            break;
        case jge_OP:
            opcode = "jge";
            jump = 1;
            break;


        case rotulo_ASM:
            // Skip
            break;
        default:
            printf("ERROR: Invalid operation.\n");
            exit(1);
            break;
    }

    if(cod->op == rotulo_ASM){
        printf(".%s:\n", cod->end1);
    }else{
        if(cod->end2 == NULL && cod->dest == NULL){
            if(jump == 1){
                printf("\t%s\t.%s\n", opcode, cod->end1); // pop rax
            }else{
                printf("\t%s\t%s\n", opcode, cod->end1); // pop rax
            }
            

        }else{

            if(cod->end2 != NULL){
                if(cod->op == mov_from_mem){
                    printf("\t%s\t%s%s, %s\n", opcode, cod->end1, cod->end2, cod->dest);// opcode desloc(reg), dest
                }else{
                    printf("\t%s\t%s, %s%s\n", opcode, cod->end1, cod->end2, cod->dest);// opcode desloc(reg), dest
                }
            }else{
                printf("\t%s\t%s, %s\n", opcode, cod->end1, cod->dest); // addq rdx, rax
            }
        }
    }

}


void exportaASM(codASM* programa){
    codASM* aux = programa;

    while(programa != NULL){

        imprimeASM(programa);

        programa = programa->prox;

    }

}

void limpaASM(codASM** cod){
    codASM* anterior = NULL;
    codASM* atual = *cod;
    codASM* proximo = *cod;

    if(*cod != NULL){
        while(atual != NULL){
            proximo = atual->prox;
            if(proximo != NULL && atual->op == push_OP && proximo->op == pop_OP && strcmp(proximo->end1, "%rax") == 0){
                anterior->prox = proximo->prox;
                proximo->prox = NULL;
                liberaASM(atual);
                atual = anterior->prox;
                proximo = anterior->prox;
            }else if(proximo != NULL && atual->op == push_OP && proximo->op == mov_to_mem && strcmp(proximo->end1, "%rax") == 0){
                anterior->prox = proximo;
                atual->prox = NULL;
                liberaASM(atual);
                atual = proximo;
            }else{
                anterior = atual;
                atual = proximo;
            }
        }
    }

}

