#ifndef AST_HEADER
#define AST_HEADER

typedef struct AST {
  char *label;
  struct AST *prim_filho;
  struct AST *prim_irmao;
} AST;

AST* novoNodo(char *data);

void adicionaFilhos(AST* pai, AST* lista_filhos[], int num_filhos);

AST* cria_e_adiciona(char *data, AST** lista_filhos, int num_filhos);

void printIrmaos(AST* node, AST* pai);
	
void printEnderecos(AST* node);

void printValEnderecos(AST* node);
	
void libera(AST* node);


#endif
