#ifndef TABELA_HEADER
#define TABELA_HEADER

#include "tipos.h"
#include "valor_lexico.h"

// Guarda tipos esperados para params de funcoes
typedef struct Parametro{

	enum_Tipo tipo;
	struct Parametro *prox;

}Parametro;

// Entrada na tabela de simbolos
typedef struct tabela_simbolos{
    char *chave;
	int desloc;
	enum_Escopo escopo;
	char *label;		// se for funcao

	int num_linha;
	enum_Natureza natureza; 	   	    
	enum_Tipo tipo;
	int tamanho;
	valor_lexico valor;
	Parametro *lista_param;

	struct tabela_simbolos *prox;

}tabela_simbolos;

// Pilha de tabelas de simbolos
typedef struct pilha_tabela{
	tabela_simbolos *atual;         //tabela atual
	struct pilha_tabela *prox;      //primeiro da proxima

}pilha_tabela;


// Lista de variaveis a serem adicionadas a tabela
typedef struct lista_var{
	char* nome;   
	int desloc;      
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

// DEBUG
void printParams(Parametro* lista_par);

// DEBUG
void printListaVar(lista_var* lista);

// DEBUG
void printTabela(tabela_simbolos* tabela);

// DEBUG
void printPilha(pilha_tabela* pilha);


// Cria e aloca mem para escopo global
pilha_tabela* iniciaPilha(); 


// Adiciona entrada para lista de vars
lista_var* novoListaVar(lista_var* lista, char* nome, int tam, int linha, int vetor, valor_lexico valor, enum_Tipo tipo, int desloc);


// Adiciona entrada para lista de params
Parametro* novoParametro(Parametro *lista_par, enum_Tipo tipo);


// Cria entrada de funcao na tabela de simbolos
tabela_simbolos* novaEntradaTabelaFunc(char* chave, int linha, enum_Tipo tipo, valor_lexico valor, int tamanho, Parametro *lista_par, char* label);


// Cria entrada na tabela de simbolos
tabela_simbolos* novaEntradaTabela(char* chave, int linha, enum_Natureza natureza, enum_Tipo tipo, valor_lexico valor, int tamanho, int desloc, enum_Escopo escopo);


// Cria e adiciona entrada de funcao na tabela
tabela_simbolos* adicionaEntradaTabelaFunc(tabela_simbolos* escopo_atual, char* chave, int linha, enum_Tipo tipo, valor_lexico valor, int tamanho, Parametro *lista_par, char* label);


// Cria e adiciona entrada na tabela
tabela_simbolos* adicionaEntradaTabela(tabela_simbolos* escopo_atual, char* chave, int linha, enum_Natureza natureza, enum_Tipo tipo, valor_lexico valor, int tamanho, int desloc, enum_Escopo escopo);


// Cria e adiciona entradas para variaveis da lista
tabela_simbolos* adicionaListaVar(tabela_simbolos* escopo_atual, lista_var* variaveis, enum_Tipo tipo, enum_Escopo escopo);


// Abre escopo no topo da pilha PUSH
pilha_tabela* novoEscopo(pilha_tabela* topo);


// Fecha escopo atual POP
pilha_tabela* fechaEscopo(pilha_tabela* pilha);


// Encontra entrada na tabela pela chave, NULL senao
tabela_simbolos* procuraTabela(char* chave, tabela_simbolos* tabela);


// Procura simbolo em todo a pilha
tabela_simbolos* encontraSimbolo(char* chave, pilha_tabela* pilha);


// list1 = params definidos da func; lista2 = argumentos na chamada
int comparaParams(Parametro* lista1, Parametro* lista2);


// Confere tipos de argumentos passados 
void confereChamadaFunc(pilha_tabela* pilha, char* chave_func, Parametro* argumentos, int linha);


// Copia parametros para a entrada da funcao
Parametro* copiaParametros(Parametro* lista_par);


// Imprime mensagem e termina execucao com codigo passado
void mensagemErro(int erro, int linha, void* ref1);


// Confere se foi declarado, retorna tipo
enum_Tipo recuperaTipo(pilha_tabela* pilha, char* chave, int linha);


// Confere semantica IO
void verificaInputOutput(enum_Tipo tipo, char comando, int linha);


// Encontra a funcao do bloco que estamos (para verificar return)
tabela_simbolos* encontraUltimaFuncao(pilha_tabela* pilha);


// Confere semantica do comando return
void verificaReturn(pilha_tabela* pilha, enum_Tipo tipo, int linha);


// Confere atribuicoes e conversoes implicitas
void confereAtribuicao(enum_Tipo tipo_recebe, enum_Tipo tipo_recebido, int linha);


// Confere a natureza de um ident. natureza passada por parametro = natureza q deveria ser
void confereNatureza(pilha_tabela* pilha, char* chave, enum_Natureza natureza, int linha);


// Confere os tipos, e tambem arruma os tipos de nodos AST de inicializacao 
void confereInicializacao(pilha_tabela* pilha, void* nodo_in, enum_Tipo tipo, int linha);


// Define o tipo de expressoes binarias
enum_Tipo inferencia_tipo(enum_Tipo tipo1, enum_Tipo tipo2, int linha);


// Confere se o valor nao eh maior q 16
void confereShift(int valor, int linha);


// Retorna o deslocamento do simbolo na memoria
int recuperaDesloc(char* chave, pilha_tabela* pilha);


// Retorna se a var e global ou local 
char* recuperaEscopo(LISTA_PTR** lista_ptr,char* chave, pilha_tabela* pilha);


int quantidadeVarLocais(tabela_simbolos* atual);


int contaParams(Parametro* lista);

#endif