#ifndef AST_HEADER
#define AST_HEADER

#include "tipos.h"

typedef struct AST {
  char *label;
  enum_Tipo tipo;
  struct AST *prim_filho;
  struct AST *prim_irmao;
} AST;

extern AST* lista[10]; // var global

AST* novoNodo(char *data, enum_Tipo tipo); // TIPO

void adicionaFilhos(AST* pai, AST* lista_filhos[], int num_filhos);

void appendFilho(AST* pai, AST* novo_filho);

AST* cria_e_adiciona(char *data, AST** lista_filhos, int num_filhos, enum_Tipo tipo); // TIPO

void printIrmaos(AST* node, AST* pai);

void printEnderecos(AST* node);

void printValEnderecos(AST* node);

void printPreorder(AST* node);

AST* encontraNodo(AST* node, char *label);

int ehPai(AST* raiz, AST* node);

AST* encontraPai(AST* raiz, AST* node);

void removeNodo(AST* raiz, AST* node);

void alteraNodo(AST* node, char* novo_valor);

void exporta(AST* arvore);

void libera(AST* node);


#endif
