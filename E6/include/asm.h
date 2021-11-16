#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"

typedef enum {
    mov_to_mem, mov_from_mem,
    push_OP, pop_OP,
    addq_OP, imulq_OP,
    rotulo_OP,
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


void adicionaASM(codASM** lista, opASM op, char* end1, char* end2, char* dest);


int deslocASM(int reset);


void imprimeASM(codASM* cod);


void exportaASM(codASM* programa);
