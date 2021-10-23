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
	enum_Natureza natureza; 	   	    // código definido no E2
	enum_Tipo tipo;
	int tamanho;
	valor_lexico valor;
	Parametro *lista_param;

	struct tabela_simbolos *prox;

}tabela_simbolos;

typedef struct pilha_tabela{
	tabela_simbolos *atual;         //tabela atual
	struct pilha_tabela *prox;      //primeiro da proxima

}pilha_tabela;

typedef struct lista_var{
	char* nome;         
	int tamanho;      
	int linha;
	int vetor; // 0 = nao vetor; 1 = vetor
	enum_Tipo tipo;
	valor_lexico valor;

	struct lista_var *prox;
}lista_var;

void liberaListaVar(lista_var* lista_var);

void liberaParams(Parametro* lista_par);

void liberaTabela(tabela_simbolos* tabela);

void liberaPilha(pilha_tabela* pilha);

void printTabela(tabela_simbolos* tabela);

void printPilha(pilha_tabela* pilha);

pilha_tabela* iniciaPilha(); 

lista_var* novoListaVar(lista_var* lista, char* nome, int tamanho, int linha, int vetor, valor_lexico valor, enum_Tipo tipo);

Parametro* novoParametro(Parametro *lista_par, enum_Tipo tipo);

tabela_simbolos* novaEntradaTabelaFunc(char* chave, int linha, enum_Natureza natureza, enum_Tipo tipo, valor_lexico valor, int tamanho, Parametro *lista_par);

tabela_simbolos* novaEntradaTabela(char* chave, int linha, enum_Natureza natureza, enum_Tipo tipo, valor_lexico valor, int tamanho);

tabela_simbolos* adicionaEntradaTabelaFunc(tabela_simbolos* escopo_atual, char* chave, int linha, enum_Natureza natureza, enum_Tipo tipo, valor_lexico valor, int tamanho, Parametro *lista_par);

tabela_simbolos* adicionaEntradaTabela(tabela_simbolos* escopo_atual, char* chave, int linha, enum_Natureza natureza, enum_Tipo tipo, valor_lexico valor, int tamanho);

tabela_simbolos* adicionaListaVar(tabela_simbolos* escopo_atual, lista_var* variaveis, enum_Tipo tipo);

pilha_tabela* novoEscopo(pilha_tabela* topo);

pilha_tabela* fechaEscopo(pilha_tabela* pilha);

tabela_simbolos* procuraTabela(char* chave, tabela_simbolos* tabela);

tabela_simbolos* encontraSimbolo(char* chave, pilha_tabela* pilha);

tabela_simbolos* foiDeclarado(char* chave, tabela_simbolos* tabela);

// list1 = params definidos da func; lista2 = argumentos na chamada
void comparaParams(Parametro* lista1, Parametro* lista2);

Parametro* copiaParametros(Parametro* lista_par);

void printParams(Parametro* lista_par);

void printListaVar(lista_var* lista);

void mensagemErro(int erro, int linha, void* ref1);


#endif