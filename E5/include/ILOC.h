#ifndef TIPOS_ILOC
#define TIPOS_ILOC

// Todos os comandos em ILOC
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

// Informacao para uma linha de ILOC
typedef struct codILOC{
    opILOC op;
    char *end1;
    char *end2;
    char *dest;

	struct codILOC *prox;

}codILOC;


// Restorna string com o proximo registrador
char* geraReg();


// strdup(), que aceita NULL
char* copiaEnd(char* str);


void liberaILOC(codILOC* lista);


// Nova linha de ILOC
codILOC* novoILOC(opILOC op, char* end1, char* end2, char* dest);


// Coloca linha de ILOC no final da lista
codILOC* appendCod(codILOC* lista, codILOC* novo);


// Imprime ILOC formatado
void imprimeCod(codILOC* cod);


#endif