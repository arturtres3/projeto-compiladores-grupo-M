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
int comparaParams(Parametro* lista1, Parametro* lista2);

void confereChamadaFunc(pilha_tabela* pilha, char* chave_func, Parametro* argumentos, int linha);

Parametro* copiaParametros(Parametro* lista_par);

void printParams(Parametro* lista_par);

void printListaVar(lista_var* lista);

void mensagemErro(int erro, int linha, void* ref1);



enum_Tipo recuperaTipo(pilha_tabela* pilha, char* chave, int linha);

void verificaInputOutput(enum_Tipo tipo, char comando, int linha);

tabela_simbolos* encontraUltimaFuncao(pilha_tabela* pilha);

void verificaReturn(pilha_tabela* pilha, enum_Tipo tipo, int linha);

void confereAtribuicao(enum_Tipo tipo_recebe, enum_Tipo tipo_recebido, int linha);

void confereNatureza(pilha_tabela* pilha, char* chave, enum_Natureza natureza, int linha);

void confereInicializacao(pilha_tabela* pilha, void* nodo_in, enum_Tipo tipo, int linha);

enum_Tipo inferencia_tipo(enum_Tipo tipo1, enum_Tipo tipo2, int linha);

void confereShift(int valor, int linha);

#endif