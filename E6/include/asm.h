#ifndef ASM_HEADER
#define ASM_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"

typedef enum {
    mov_to_mem, mov_from_mem,
    push_OP, pop_OP,
    addq_OP, imulq_OP,
    subq_OP, idivq_OP,
    cmpq_OP,
    jmp_OP, jl_OP, jg_OP,
    jle_OP, jge_OP,
    je_OP, jne_OP,
    call_OP,
    declara_global,
    abre_funcao,
    fecha_funcao,
    rotulo_ASM,
} opASM;

typedef struct codASM{
    opASM op;
    char *end1;
    char *end2;
    char *dest;

	struct codASM *prox;

}codASM;


void liberaASM(codASM* lista);


codASM* novoASM(opASM op, char* end1, char* end2, char* dest);


void appendASM(codASM** lista, codASM* novo);

void appendInicioASM(codASM** lista, codASM* novo);


void adicionaASM(codASM** lista, opASM op, char* end1, char* end2, char* dest);

void adicionaInicioASM(codASM **lista, opASM op, char *end1, char *end2, char *dest);


codASM* ultimoASM(codASM* lista);





void imprimeASM(codASM* cod);


void exportaASM(codASM* programa);


void limpaASM(codASM** cod);


#endif
