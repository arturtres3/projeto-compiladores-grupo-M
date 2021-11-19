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
    jump_OP, jumpI_OP, cbr_OP,
    halt_OP
} opILOC;

// Informacao para uma linha de ILOC
typedef struct codILOC{
    opILOC op;
    char *end1;
    char *end2;
    char *dest;

	struct codILOC *prox;

}codILOC;


// Restorna string com o proximo registrador, salva str em lista_PTR
char* geraReg(LISTA_PTR** lista);


// Restorna string com a proxima label, salva str em lista_PTR, comeca em L1 (L0 e reservada para main)
char* geraLabel(LISTA_PTR** lista);


// Coloca o valor de label nos remendos apontados na lista
void fazRemendo(LISTA_PTR* lista, char* label);


// Mantem conta de desloc para variaveis global
int deslocGlobal();


// Mantem conta de desloc para var local. reset = 1 zera desloc
int deslocLocal(int reset);


int deslocASM(int reset);


void liberaILOC(codILOC* lista);


// Nova linha de ILOC
codILOC* novoILOC(opILOC op, char* end1, char* end2, char* dest);


// Coloca linha de ILOC no final da lista
void appendCod(codILOC** lista, codILOC* novo);


// Cria e faz append na lista
void adicionaILOC(codILOC** lista, opILOC op, char* end1, char* end2, char* dest);


// Retorna o ultimo valor da lista, a ultima instrucao inserida
codILOC* ultimoILOC(codILOC* lista);


// Adiciona 4 ao topo da pilha para reservar espaco para as vars locais
void declaraVarLocais(codILOC** lista, int num_vars);


// Quantas instrucoes estao na lista
int contaILOC(codILOC* lista);


// Imprime instrucao ILOC formatada
void imprimeCod(codILOC* cod);


// Imprime programa ILOC gerado
void exportaILOC(codILOC* programa);


#endif