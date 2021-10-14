#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symboltable.h"

void insere(SYMBOLTABLE *t, char *key,  char *natureza, char *tipo, int tamanho, void *arg, void *valor) {
   SYMBOLCEL *cp;
   cp = t->list;
   while (cp != NULL && strcmp( cp->key, key) != 0)
      cp = cp->link;
   if (cp == NULL) {
      cp = malloc( sizeof (SYMBOLCEL));
      cp->key = copyString( key );
      cp->link = t->list;
      t->list = cp;
   }
   cp->value = copyvalues();
}

char* copyKey( char *key) {
   char *newstr;
   newstr = malloc( strlen(key) + 1);
   if (newstr == NULL) exit( EXIT_FAILURE);
   strcpy( newstr, key);
   return newstr;
}

SYMBOLCEL *Lookup( SYMBOLTABLE *t, char *key) {
   SYMBOLCEL *cp;
   cp = t->list;
   while (cp != NULL && strcmp( cp->key, key) != 0)
      cp = cp->link;
   if (cp == NULL) return NULL;
   return cp;
}

SYMBOLTABLE *NewSymbolTable( void) {
   SYMBOLTABLE *t;
   t = malloc( sizeof (SYMBOLTABLE));
   t->list = NULL;
   return t;
}

void FreeSymbolTable( SYMBOLTABLE *t) {
   SYMBOLCEL *cp, *next;
   cp = t->list;
   while (cp != NULL) {
      next = cp->link;
      free( cp->key);
      free( cp);
      cp = next;
   }
   free(t);
}