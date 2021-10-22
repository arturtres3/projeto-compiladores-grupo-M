#ifndef TABELA_HEADER
#define TABELA_HEADER

#include "tipos.h"
#include "valor_lexico.h"



typedef struct Parametro{

	enum_Tipo tipo;
	struct Parametro *prox;

}Parametro;

typedef struct tabela_simbolos{
    char *chave;

	int num_linha;
	int natureza; 	   	    // código definido no E2
	enum_Tipo tipo;
	int tamanho;
	union Valores valor;
	Parametro *lista_param;

	struct tabela_simbolos *prox;

}tabela_simbolos;

typedef struct pilha_tabela{
	tabela_simbolos *atual;         //tabela atual
	struct pilha_tabela *prox;      //primeiro da proxima

}pilha_tabela;




void liberaParams(Parametro* lista_par);

void liberaTabela(tabela_simbolos* tabela);

void liberaPilha(pilha_tabela* pilha);

void printTabela(tabela_simbolos* tabela);

void printPilha(pilha_tabela* pilha);

pilha_tabela* iniciaPilha(); // var global

Parametro* novoParametro(Parametro *lista_par, enum_Tipo tipo);

tabela_simbolos* novaEntradaTabelaFunc(char* chave, int linha, int natureza, enum_Tipo tipo, union Valores valor, int tamanho, Parametro *lista_par);

tabela_simbolos* novaEntradaTabela(char* chave, int linha, int natureza, enum_Tipo tipo, union Valores valor, int tamanho);

tabela_simbolos* adicionaEntradaTabelaFunc(tabela_simbolos* escopo_atual, char* chave, int linha, int natureza, enum_Tipo tipo, union Valores valor, int tamanho, Parametro *lista_par);

tabela_simbolos* adicionaEntradaTabela(tabela_simbolos* escopo_atual, char* chave, int linha, int natureza, enum_Tipo tipo, union Valores valor, int tamanho);

pilha_tabela* novoEscopo(pilha_tabela* topo);

pilha_tabela* fechaEscopo(pilha_tabela* pilha);

tabela_simbolos* procuraTabela(char* chave, tabela_simbolos* tabela);

tabela_simbolos* encontraSimbolo(char* chave, pilha_tabela* pilha);

// list1 = params definidos da func; lista2 = argumentos na chamada
void comparaParams(Parametro* lista1, Parametro* lista2);

void printParams(Parametro* lista_par);


void kill_me();

//extern void* pilha; // var global


#endif