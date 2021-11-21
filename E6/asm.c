#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/tipos.h"
#include "include/asm.h"

void liberaASM(codASM *lista){

    if (lista == NULL)
        return;

    liberaASM(lista->prox);

    free(lista->end1);
    free(lista->end2);
    free(lista->dest);

    free(lista);
}

codASM *novoASM(opASM op, char *end1, char *end2, char *dest){

    codASM *novo = (codASM *)malloc(sizeof(codASM));

    novo->op = op;
    novo->end1 = copiaStr(end1);
    novo->end2 = copiaStr(end2);
    novo->dest = copiaStr(dest);

    novo->prox = NULL;

    return novo;
}

void appendASM(codASM **lista, codASM *novo){

    codASM *aux = *lista;

    if (aux != NULL)
    {
        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }
        aux->prox = novo;
    }
    else
    {
        *lista = novo;
    }
}


void appendInicioASM(codASM** lista, codASM* novo){
    codASM* aux = novo;

    if(aux != NULL){
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = *lista;
        *lista = novo;
    }else{
        *lista = novo;
    }
}


void adicionaASM(codASM **lista, opASM op, char *end1, char *end2, char *dest){

    codASM *codigo = novoASM(op, end1, end2, dest);

    appendASM(lista, codigo);
}

void adicionaInicioASM(codASM **lista, opASM op, char *end1, char *end2, char *dest){

    codASM *codigo = novoASM(op, end1, end2, dest);

    appendInicioASM(lista, codigo);
}

codASM *ultimoASM(codASM *lista){
    if (lista != NULL)
    {
        while (lista->prox != NULL)
        {
            lista = lista->prox;
        }
    }
    return lista;
}

void cabecalhoFunc(char *nome_func, char *label_abre_func){
    printf("\t.globl\t%s\n", nome_func);
    printf("\t.type\t%s, @function\n", nome_func);
    printf("%s:\n", nome_func);
    printf(".%s:\n", label_abre_func);
    printf("\t.cfi_startproc\n");
    printf("\tendbr64\n");
    printf("\tpushq\t%%rbp\n");
    printf("\t.cfi_def_cfa_offset 16\n");
    printf("\t.cfi_offset 6, -16\n");
    printf("\tmovq\t%%rsp, %%rbp\n");
    printf("\t.cfi_def_cfa_register 6\n");
}

void rodapeFunc(char *nome_func, char *label_fecha_func){
    printf("\tleave\n");
    //printf("\tpopq\t%%rbp\n");
    printf("\t.cfi_def_cfa 7, 8\n");
    printf("\tret\n");
    printf("\t.cfi_endproc\n");
    printf(".%s:\n", label_fecha_func);
    printf("\t.size	%s, .-%s\n", nome_func, nome_func);
}

void rodapeProg(){ 
    printf("\t.ident\t\"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0\"\n");
    printf("\t.section\t.note.GNU-stack,\"\",@progbits\n");
    printf("\t.section\t.note.gnu.property,\"a\"\n");
    printf("\t.align 8\n");
    printf("\t.long\t1f - 0f\n");
    printf("\t.long\t4f - 1f\n");
    printf("\t.long\t5\n");
    printf("0:\n");
    printf("\t.string\t\"GNU\"\n");
    printf("1:\n");
    printf("\t.align 8\n");
    printf("\t.long\t0xc0000002\n");
    printf("\t.long\t3f - 2f\n");
    printf("2:\n");
    printf("\t.long\t0x3\n");
    printf("3:\n");
    printf("\t.align 8\n");
    printf("4:\n");

}

void imprimeASM(codASM *cod){

    char *opcode;
    int jump = 0;

    switch (cod->op)
    {

    case mov_to_mem:
        opcode = "movq";
        break;
    case mov_from_mem:
        opcode = "movq";
        break;
    case push_OP:
        opcode = "pushq";
        break;
    case pop_OP:
        opcode = "popq";
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
    case je_OP:
        opcode = "je";
        jump = 1;
        break;
    case jne_OP:
        opcode = "jne";
        jump = 1;
        break;
    case call_OP:
        opcode = "call";
        break;
    case declara_global:
        opcode = ".comm";
        break;
    case abre_funcao:
        // Skip
        break;
    case fecha_funcao:
        // Skip
        break;

    case rotulo_ASM:
        // Skip
        break;
    default:
        printf("ERROR: Invalid operation.\n");
        exit(1);
        break;
    }

    if (cod->op == rotulo_ASM)
    {
        printf(".%s:\n", cod->end1);
    }
    else if (cod->op == abre_funcao)
    {
        cabecalhoFunc(cod->end1, cod->end2);
    }
    else if (cod->op == fecha_funcao)
    {
        rodapeFunc(cod->end1, cod->end2);
    }
    else if (cod->op == declara_global)
    {
        printf("\t%s\t%s,8,8\n", opcode, cod->end1); // .comm   a,8,8
    }
    else
    {
        if (cod->end2 == NULL && cod->dest == NULL)
        {
            if (jump == 1)
            {
                printf("\t%s\t\t.%s\n", opcode, cod->end1); // pop rax
            }
            else
            {
                if(strlen(opcode) > 3){
                    printf("\t%s\t%s\n", opcode, cod->end1); // pop rax
                } else {
                    printf("\t%s\t\t%s\n", opcode, cod->end1); // pop rax
                }
            }
        }
        else
        {

            if (cod->end2 != NULL)
            {
                if (cod->op == mov_from_mem)
                {
                    printf("\t%s\t%s%s, %s\n", opcode, cod->end1, cod->end2, cod->dest); // opcode desloc(reg), dest
                }
                else
                {
                    printf("\t%s\t%s, %s%s\n", opcode, cod->end1, cod->end2, cod->dest); // opcode desloc(reg), dest
                }
            }
            else
            {
                printf("\t%s\t%s, %s\n", opcode, cod->end1, cod->dest); // addq rdx, rax
            }
        }
    }
}

void exportaASM(codASM *programa){
    codASM *aux = programa;

    printf("\t.text\n");

    while (programa != NULL)
    {

        imprimeASM(programa);

        programa = programa->prox;
    }

    rodapeProg();
}

void limpaASM(codASM **cod){
    codASM *anterior = NULL;
    codASM *atual = *cod;
    codASM *proximo = *cod;

    if (*cod != NULL)
    {
        while (atual != NULL)
        {
            proximo = atual->prox;
            if (proximo != NULL && atual->op == push_OP && proximo->op == pop_OP && strcmp(proximo->end1, "%rax") == 0)
            {
                anterior->prox = proximo->prox;
                proximo->prox = NULL;
                liberaASM(atual);
                atual = anterior->prox;
                proximo = anterior->prox;
            }
            else
            {
                anterior = atual;
                atual = proximo;
            }
        }
    }
}
