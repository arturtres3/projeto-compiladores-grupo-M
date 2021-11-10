// Grupo M
// Eduardo Henrique Ferreira do Nascimento (00260846)
// Artur Tres do Amaral (00287682)

#include <stdio.h>
#include <stdlib.h>
#include "include/tabela.h"
#include "include/ILOC.h"
extern int yyparse(void);
extern int yylex_destroy(void);

lista_var* lista_variaveis = NULL;
Parametro* lista_parametros = NULL;
pilha_tabela *pilha = NULL;
codILOC* lista_ILOC;
codILOC* temp_ILOC;
tabela_simbolos* tempSimbolo = NULL;
void* tempAST = NULL;

void *arvore = NULL;
void exporta (void *arvore);
void libera (void *arvore);

int main (int argc, char **argv)
{
  pilha = iniciaPilha();
  int ret = yyparse();
  //exporta (arvore);       // Imprime AST
  exportaILOC(lista_ILOC);  // Imprime programa ILOC

  libera(arvore);
  liberaILOC(lista_ILOC);
  arvore = NULL;

  yylex_destroy();
  return ret;
}
