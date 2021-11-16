#ifndef AST_HEADER
#define AST_HEADER

#include "tipos.h"
#include "ILOC.h"
#include "asm.h"

// Nodo da AST
typedef struct AST {
  char *label;

  char *local;        // registrador cod ILOC
  codILOC* codigo;
  LISTA_PTR* l_false;
  LISTA_PTR* l_true;

  codASM* ASM;        

  enum_Tipo tipo;
  struct AST *prim_filho;
  struct AST *prim_irmao;
} AST;

extern AST* lista[10]; // var global

// Cria nodo AST
AST* novoNodo(char *data, enum_Tipo tipo);


// Atribui lista de filhos a nodo pai
void adicionaFilhos(AST* pai, AST* lista_filhos[], int num_filhos);


// Adiciona novo filho no final da lista
void appendFilho(AST* pai, AST* novo_filho);


// Cria nodo pai e adiciona filhos
AST* cria_e_adiciona(char *data, AST** lista_filhos, int num_filhos, enum_Tipo tipo); 


// Imprime %p do pai e todos os filhos
void printIrmaos(AST* node, AST* pai);


// Chama printIrmaos para toda arvore
void printEnderecos(AST* node);


// Imprime label para cada nodo
void printValEnderecos(AST* node);


// Imprime arvore DEBUG
void printPreorder(AST* node);


// Procura label na arvore, retorna nodo
AST* encontraNodo(AST* node, char *label);


//  Retorna ultimo filho do nodo
AST* ultimoFilho(AST* node);


// Retorna 1 se raiz eh pai de node, 0 senao
int ehPai(AST* raiz, AST* node);


// Procura o pai de node na arvore
AST* encontraPai(AST* raiz, AST* node);


// Retira nodo da arvore
void removeNodo(AST* raiz, AST* node);


// Atribui nova label para nodo
void alteraNodo(AST* node, char* novo_valor);


// Atribui novo tipo para nodo
void alteraTipoNodo(AST* node, enum_Tipo tipo);


// Imprime arvore nas specs da E3
void exporta(AST* arvore);


// Libera memoria alocada para a arvore
void libera(AST* node);


#endif
