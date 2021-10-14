#ifndef SYMBOLTABLE_HEADER
#define SYMBOLTABLE_HEADER

typedef struct SYMBOLCEL {
  char *key;
  char *natureza;
  char *tipo;
  int tamanho;
  void *arg;
  void *valor;
  struct SYMBOLCEL *link;
} SYMBOLCEL;

typedef struct SYMBOLTABLE {
  struct SYMBOLCEL *list;
} SYMBOLTABLE;

void insere(SYMBOLTABLE *t, char *key,  char *natureza, char *tipo, int tamanho, void *arg, void *valor);
char* copyKey( char *key);
SYMBOLCEL *Lookup( SYMBOLTABLE *t, char *key);
SYMBOLTABLE *NewSymbolTable( void);
void FreeSymbolTable( SYMBOLTABLE *t);
#endif
