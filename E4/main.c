// Grupo M
// Eduardo Henrique Ferreira do Nascimento (00260846)
// Artur Tres do Amaral (00287682)
//arquivo disponibilizado no moodle pelo professor para realização do trabalho
/*
  Função principal para realização da E3.
  Não modifique este arquivo.
*/
#include <stdio.h>
extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
void exporta (void *arvore);
void libera (void *arvore);

int main (int argc, char **argv)
{
  int ret = yyparse();
  exporta (arvore);
  libera(arvore);
  arvore = NULL;
  yylex_destroy();
  return ret;
}
