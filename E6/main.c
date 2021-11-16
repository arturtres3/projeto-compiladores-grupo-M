// Grupo M
// Eduardo Henrique Ferreira do Nascimento (00260846)
// Artur Tres do Amaral (00287682)

#include <stdio.h>
#include <stdlib.h>
#include "include/tabela.h"
#include "include/ILOC.h"
#include "include/asm.h"
extern int yyparse(void);
extern int yylex_destroy(void);

lista_var* lista_variaveis = NULL;
Parametro* lista_parametros = NULL;
pilha_tabela *pilha = NULL;
codILOC* temp_ILOC;
tabela_simbolos* tempSimbolo = NULL;
void* tempAST = NULL;

codILOC* lista_ILOC = NULL;
codASM* lista_ASM = NULL;

void *arvore = NULL;
void exporta (void *arvore);
void libera (void *arvore);

int main (int argc, char **argv)
{
  pilha = iniciaPilha();
  int ret = yyparse();
  //exporta (arvore);       // Imprime AST
  //exportaILOC(lista_ILOC);  // Imprime programa ILOC
  exportaASM(lista_ASM);

  libera(arvore);
  liberaILOC(lista_ILOC);
  liberaASM(lista_ASM);
  arvore = NULL;

  yylex_destroy();
  return ret;
}
