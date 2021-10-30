#ifndef TIPOS_ILOC
#define TIPOS_ILOC

typedef enum {
    add_OP, sub_OP, mult_OP, div_OP,
    addI_OP, subI_OP, multI_OP, divI_OP,
    and_OP, or_OP,
    cmp_EQ_OP, cmp_NE_OP,
    cmp_LT_OP, cmp_GT_OP,
    cmp_LE_OP, cmp_GE_OP,

    loadI_OP, loadAI_OP, i2i_OP,
    storeAI_OP,
    rotulo_OP,
    jump_OP, cbr_OP
} opILOC;

typedef struct codILOC{
    opILOC op;
    char *end1;
    char *end2;
    char *dest;

	struct codILOC *prox;

}codILOC;


char* geraReg();

char* copiaEnd(char* str);

void liberaILOC(codILOC* lista);

codILOC* novoILOC(opILOC op, char* end1, char* end2, char* dest);

codILOC* appendCod(codILOC* lista, codILOC* novo);

void imprimeCod(codILOC* cod);


#endif