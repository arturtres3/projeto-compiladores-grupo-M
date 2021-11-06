%{
	// Grupo M
	// Eduardo Henrique Ferreira do Nascimento (00260846)
	// Artur Tres do Amaral (00287682)
	#include <stdio.h>
	#include <string.h>
	#define YYERROR_VERBOSE 1
	extern void *arvore;

	int yylex(void);
	int yyerror (char const *s);
	int get_line_number(void);

	char* temp = NULL; //variavel para colocar literais em strings
	char* temp1 = NULL; 
	char* temp2 = NULL; 
	char* rotulo = NULL;
	char* rotulo1 = NULL;
	char* rotulo2 = NULL;
%}

%code requires{
	#include "include/valor_lexico.h"
	#include "include/ast.h"
	#include "include/tabela.h"
	#include "include/tipos.h"
	#include "include/ILOC.h"
}

%code{
	extern LISTA_PTR* lista_ptr;
	AST* lista[10]; //lista de filhos
	extern pilha_tabela *pilha;

	extern AST* tempAST;
	extern tabela_simbolos* tempSimbolo;
	extern codILOC* temp_ILOC;
	extern codILOC* lista_ILOC;
	extern lista_var* lista_variaveis;
	extern Parametro* lista_parametros;
	int num_linha; 
}

%union{
	enum_Tipo tipo_semantico;
	//lista_var* variaveis;

	struct valor_lexico valor_lexico;
	AST* nodo;
}

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_STRING
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_DO
%token TK_PR_CONST
%token TK_PR_STATIC
%token TK_PR_FOREACH
%token TK_PR_FOR
%token TK_PR_SWITCH
%token TK_PR_CASE
%token TK_PR_BREAK
%token TK_PR_CONTINUE
%token TK_PR_CLASS
%token TK_PR_PRIVATE
%token TK_PR_PUBLIC
%token TK_PR_PROTECTED
%token TK_PR_END
%token TK_PR_DEFAULT
%token <valor_lexico> TK_PR_RETURN
%token <valor_lexico> TK_PR_INPUT
%token <valor_lexico> TK_PR_OUTPUT
%token <valor_lexico> TK_OC_LE
%token <valor_lexico> TK_OC_GE
%token <valor_lexico> TK_OC_EQ
%token <valor_lexico> TK_OC_NE
%token <valor_lexico> TK_OC_AND
%token <valor_lexico> TK_OC_OR
%token <valor_lexico> TK_OC_SL
%token <valor_lexico> TK_OC_SR
%token <valor_lexico> TK_LIT_INT
%token <valor_lexico> TK_LIT_FLOAT
%token <valor_lexico> TK_LIT_FALSE
%token <valor_lexico> TK_LIT_TRUE
%token <valor_lexico> TK_LIT_CHAR
%token <valor_lexico> TK_LIT_STRING
%token <valor_lexico> TK_IDENTIFICADOR
%token <valor_lexico> '+'	
%token <valor_lexico> '-'		
%token <valor_lexico> '*'		
%token <valor_lexico> '/'		
%token <valor_lexico> '<'		
%token <valor_lexico> '>'		
%token <valor_lexico> '|'		
%token <valor_lexico> '%'		
%token <valor_lexico> '^'		
%token <valor_lexico> '&'
%token <valor_lexico> '='
%token TOKEN_ERRO

%type <nodo> programa
%type <nodo> lista_var_global_func
%type <nodo> func
%type <valor_lexico> cabecalho
%type <nodo> bloco
%type <nodo> comandos
%type <nodo> comando_simples
%type <nodo> declaracao
%type <nodo> atribuicao
%type <nodo> entrada_saida
%type <nodo> chamada_funcao
%type <nodo> comand_shift
%type <nodo> return
%type <nodo> break
%type <nodo> continue
%type <nodo> controle_fluxo
%type <nodo> inicializacao
%type <nodo> nomes_l
%type <nodo> argumentos
%type <nodo> mais_argumentos
%type <nodo> if
%type <nodo> for
%type <nodo> while
%type <nodo> expressao
%type <nodo> vetor
%type <nodo> literal
%type <nodo> literal_nao_expr
%type <nodo> shift
%type <nodo> identificador
%type <nodo> int
%type <nodo> float
%type <nodo> true
%type <nodo> false
%type <nodo> string
%type <nodo> char

%type <tipo_semantico> tipo

//%type <variaveis> nomes_g

//ternarios
%left TERNARIO

//binarios
%left TK_OC_OR
%left TK_OC_AND
%left '|'
%left '^'
%left '&'
%left TK_OC_EQ TK_OC_NE
%left '>' '<' TK_OC_GE TK_OC_LE
%left TK_OC_SL TK_OC_SR
%left '+' '-'
%left '*' '/' '%'

//unarios
%left '!' '?'
%left UNARIO_L //usado para precedencia de +, -
%right '#'
%right UNARIO_R //usado para precedencia de &, *


%left '(' ')' //parenteses tem a maior precedencia



%start programa

%%

programa:	
		lista_var_global_func			
		{
			$$ = $1; //printPilha(pilha);  
			arvore = $$;  //printPTR(lista_ptr);
			if($$ != NULL){ lista_ILOC = $$->codigo; }
			
			liberaPTR(lista_ptr); liberaPilha(pilha);
			liberaListaVar(lista_variaveis); liberaParams(lista_parametros);
		}
			;

lista_var_global_func:
		var_global lista_var_global_func	
		{
			$$ = $2;
		}

		| func lista_var_global_func		
		{
			$$ = $1; 
			if($2 != NULL){
				appendFilho($$, $2);
				appendCod(&($$->codigo), $2->codigo);
			}
		}

		|	/* PROD. VAZIA */							
		{
			$$ = NULL;
		}
		;

/* -------   Variaveis globais    ------- */

var_global: 	
		static tipo TK_IDENTIFICADOR nomes_g ';'				
		{
			lista_variaveis = novoListaVar(lista_variaveis, $3.valor.cad_char, 1, $3.num_linha, 0, $3, TIPO_NA);
			inverteListaVar(&lista_variaveis);
			pilha->atual = adicionaListaVar(pilha->atual, lista_variaveis, $2, GLOBAL, 0); 
			liberaListaVar(lista_variaveis); lista_variaveis = NULL;
		}

		| static tipo TK_IDENTIFICADOR '[' TK_LIT_INT ']' nomes_g ';'	
		{
			lista_variaveis = novoListaVar(lista_variaveis, $3.valor.cad_char, $5.valor.i, $3.num_linha, 1, $3, TIPO_NA);
			pilha->atual = adicionaListaVar(pilha->atual, lista_variaveis, $2, GLOBAL, 0); 
			liberaListaVar(lista_variaveis); lista_variaveis = NULL;
		}
		; 

nomes_g: 	
		',' TK_IDENTIFICADOR nomes_g					
		{
			lista_variaveis = novoListaVar(lista_variaveis, $2.valor.cad_char, 1, $2.num_linha, 0, $2, TIPO_NA);
		}
		
		| ',' TK_IDENTIFICADOR '[' TK_LIT_INT ']' nomes_g	
		{
			lista_variaveis = novoListaVar(lista_variaveis, $2.valor.cad_char, $4.valor.i, $2.num_linha, 1, $2, TIPO_NA);
		}

		|	/* PROD. VAZIA */
		;

/* -------   Funcoes   ------- */

func: 		
		cabecalho bloco		
		{
			lista[0] = $2;
			$$ = cria_e_adiciona($1.valor.cad_char, lista, 1, recuperaTipo(pilha, $1.valor.cad_char, $1.num_linha));

			tempSimbolo = encontraSimbolo($1.valor.cad_char, pilha);
			adicionaILOC(&($$->codigo), rotulo_OP, tempSimbolo->label, NULL, NULL);

			if(strcmp(tempSimbolo->chave, "main") != 0){
				adicionaILOC(&($$->codigo), i2i_OP, "rsp", NULL, "rfp");
				adicionaILOC(&($$->codigo), addI_OP, "rsp", "16", "rsp");
			}
			
			declaraVarLocais(&($$->codigo), quantidadeVarLocais(pilha->atual));
			
			if($2 != NULL){
				appendCod(&($$->codigo), $2->codigo);
			}

			if(strcmp(tempSimbolo->chave, "main") != 0){
				temp = geraReg(&lista_ptr); temp1 = geraReg(&lista_ptr); temp2 = geraReg(&lista_ptr);

				adicionaILOC(&($$->codigo), loadAI_OP, "rfp", "0", temp);
				adicionaILOC(&($$->codigo), loadAI_OP, "rfp", "4", temp1);
				adicionaILOC(&($$->codigo), loadAI_OP, "rfp", "8", temp2);
				adicionaILOC(&($$->codigo), i2i_OP, temp1, NULL, "rsp");
				adicionaILOC(&($$->codigo), i2i_OP, temp2, NULL, "rfp");
				adicionaILOC(&($$->codigo), jump_OP, temp, NULL, NULL);

				temp = NULL; temp1 = NULL; temp2 = NULL;
			}
			tempSimbolo = NULL;
			pilha = fechaEscopo(pilha);
			deslocLocal(1);
		}
		;

cabecalho:	
		static tipo TK_IDENTIFICADOR '(' parametros ')' 
		{
			$$ = $3; 
			if(strcmp($3.valor.cad_char, "main") == 0){
				temp = "L0";
			}else{
				temp = geraLabel(&lista_ptr);
			}
			pilha->atual = adicionaEntradaTabelaFunc(pilha->atual, $3.valor.cad_char, $3.num_linha, $2, $3, 1, lista_parametros, temp);
			liberaParams(lista_parametros); lista_parametros = NULL;

			//printPilha(pilha);
		}
		;

parametros:	
		const tipo TK_IDENTIFICADOR mais_parametros 	
		{
			lista_parametros = novoParametro(lista_parametros, $2);
			lista_variaveis = novoListaVar(lista_variaveis, $3.valor.cad_char, 1, $3.num_linha, 0, $3, $2);
		}

		|	/* PROD. VAZIA */
		;

mais_parametros:
		',' const tipo TK_IDENTIFICADOR mais_parametros		
		{
			lista_parametros = novoParametro(lista_parametros, $3);
			lista_variaveis = novoListaVar(lista_variaveis, $4.valor.cad_char, 1, $4.num_linha, 0, $4, $3);
		}

		|	/* PROD. VAZIA */
		;

/* -------   Bloco de Comandos   ------- */

abre_bloco:	
		'{' 	
		{
			pilha = novoEscopo(pilha);
			inverteListaVar(&lista_variaveis);
			tempSimbolo =  encontraUltimaFuncao(pilha);
			if(strcmp(tempSimbolo->chave,"main") == 0){
				pilha->atual = adicionaListaVar(pilha->atual, lista_variaveis, TIPO_NA, LOCAL, 1);
			}else{
				pilha->atual = adicionaListaVar(pilha->atual, lista_variaveis, TIPO_NA, LOCAL, 0);
			}
			
			liberaListaVar(lista_variaveis); lista_variaveis = NULL;
			tempSimbolo = NULL;
		}
		;

fecha_bloco:	
		'}'		
		{
			//printPilha(pilha);
		}
		;

bloco: 	
		abre_bloco comandos fecha_bloco		
		{
			$$ = $2;
		}
		;


comandos: 	
		comando_simples ';' comandos	
		{
			if($1 != NULL){
				$$ = $1; 
				appendFilho($$, $3);

				if($3 != NULL){
					appendCod(&($$->codigo), $3->codigo);
				}

			}else{
				$$ = $3;
			}
			
		}	

		|	/* PROD. VAZIA */							
		{
			$$ = NULL;
		}
		;

comando_simples:
		declaracao			{$$ = $1;}
		| atribuicao		{$$ = $1;}
		| entrada_saida		{$$ = $1;}
		| chamada_funcao	{$$ = $1;}
		| comand_shift		{$$ = $1;}
		| return			{$$ = $1;}
		| break				{$$ = $1;}
		| continue			{$$ = $1;}
		| controle_fluxo	{$$ = $1;}
		| bloco				{$$ = $1;}
		;

//  1. Declaracao Variavel local
declaracao: 	
		static const tipo TK_IDENTIFICADOR nomes_l		
		{
			$$ = $5; lista_variaveis = novoListaVar(lista_variaveis, $4.valor.cad_char, 1, $4.num_linha, 0, $4, TIPO_NA);
			inverteListaVar(&lista_variaveis);
			tempSimbolo =  encontraUltimaFuncao(pilha);
			if(strcmp(tempSimbolo->chave,"main") == 0){
				pilha->atual = adicionaListaVar(pilha->atual, lista_variaveis, $3, LOCAL, 1);
			}else{
				pilha->atual = adicionaListaVar(pilha->atual, lista_variaveis, $3, LOCAL, 0);
			}
			
			liberaListaVar(lista_variaveis); lista_variaveis = NULL;
			confereInicializacao(pilha, $$, $3, $4.num_linha);

			tempSimbolo = NULL;
		}

		| static const tipo inicializacao nomes_l		
		{
			$$ = $4; appendFilho($$, $5);
			pilha->atual = adicionaListaVar(pilha->atual, lista_variaveis, $3, LOCAL, 0);
			liberaListaVar(lista_variaveis); lista_variaveis = NULL;
			confereInicializacao(pilha, $$, $3, num_linha);
		}
		;

inicializacao:	
		TK_IDENTIFICADOR TK_OC_LE identificador			
		{
			lista[0] = novoNodo($1.valor.cad_char, TIPO_NA); lista[1] = $3;
			$$ = cria_e_adiciona("<=", lista, 2, TIPO_NA); num_linha = $1.num_linha;
			lista_variaveis = novoListaVar(lista_variaveis, $1.valor.cad_char, 1, $1.num_linha, 0, $1, TIPO_NA);
		}

		| TK_IDENTIFICADOR TK_OC_LE literal_nao_expr  	
		{
			lista[0] = novoNodo($1.valor.cad_char, TIPO_NA) ; lista[1] = $3;
			$$ = cria_e_adiciona("<=", lista, 2, TIPO_NA); num_linha = $1.num_linha;
			lista_variaveis = novoListaVar(lista_variaveis, $1.valor.cad_char, 1, $1.num_linha, 0, $1, TIPO_NA);
		}
		;

nomes_l: 		
		',' TK_IDENTIFICADOR nomes_l	
		{
			$$ = $3;
			lista_variaveis = novoListaVar(lista_variaveis, $2.valor.cad_char, 1, $2.num_linha, 0, $2, TIPO_NA);

		}

		| ',' inicializacao nomes_l		
		{
			$$ = $2; appendFilho($$, $3);
		}

		|	/* PROD. VAZIA */					
		{
			$$ = NULL;
		}
		;

//  2. Comando de Atribuicao
atribuicao: 	
		TK_IDENTIFICADOR '=' expressao	
		{
			lista[0] = novoNodo($1.valor.cad_char, recuperaTipo(pilha, $1.valor.cad_char, $1.num_linha)); 
			confereNatureza(pilha, $1.valor.cad_char, VAR, $1.num_linha);
			lista[1] = $3;
			$$ = cria_e_adiciona("=", lista, 2, lista[0]->tipo);
			confereAtribuicao(lista[0]->tipo, $3->tipo, $2.num_linha);

			
			temp = int_to_string(recuperaDesloc($1.valor.cad_char, pilha));
			appendCod(&($$->codigo), $3->codigo);
			adicionaILOC(&($$->codigo), storeAI_OP, $3->local, recuperaEscopo(&lista_ptr, $1.valor.cad_char, pilha), temp);
			free(temp); temp = NULL;
		}

		| vetor '=' expressao		
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona("=", lista, 2, $1->tipo);
			confereAtribuicao($1->tipo, $3->tipo, $2.num_linha);
		}
		;

//  3. Comando de Entrada e Saida
entrada_saida:	
		TK_PR_INPUT identificador	
		{
			lista[0] = $2;
			$$ = cria_e_adiciona("input", lista, 1, $2->tipo );
			verificaInputOutput($2->tipo, 'i', $1.num_linha);
		}
		
		| TK_PR_OUTPUT identificador		
		{
			lista[0] = $2;
			$$ = cria_e_adiciona("output", lista, 1, $2->tipo );
			verificaInputOutput($2->tipo, 'o', $1.num_linha);
		}

		| TK_PR_OUTPUT literal_nao_expr		
		{
			lista[0] = $2;
			$$ = cria_e_adiciona("output", lista, 1, $2->tipo ); 
			verificaInputOutput($2->tipo, 'o', $1.num_linha);
		}
		;

//  4. Chamada de Funcao
chamada_funcao: 
		TK_IDENTIFICADOR '(' argumentos ')'	
		{
			lista[0] = $3;
			temp = label_chamada($1.valor.cad_char);
			$$ = cria_e_adiciona(temp, lista, 1, recuperaTipo(pilha, $1.valor.cad_char, $1.num_linha)); 

			confereChamadaFunc(pilha, $1.valor.cad_char, lista_parametros, $1.num_linha);
			confereNatureza(pilha, $1.valor.cad_char, FUNC, $1.num_linha);
			liberaParams(lista_parametros); lista_parametros = NULL;
			free(temp); temp = NULL; 

			/*if($3 != NULL){
				appendCod(&($$->codigo), $3->codigo);
			}*/


			tempSimbolo = encontraSimbolo($1.valor.cad_char, pilha);
			int num_params = contaParams(tempSimbolo->lista_param);
			int contador = 0;
			tempAST = $3;
			
			while(tempAST != NULL && num_params > 0){	
				contador = contador + 1 + contaILOC(tempAST->codigo); // store na pilha + codigo para avaliar param 
				tempAST = ultimoFilho(tempAST);
				num_params--;
			}

			temp = int_to_string(5 + contador); // calcula end. retorno
			temp1 = geraReg(&lista_ptr);

			adicionaILOC(&($$->codigo), addI_OP, "rpc", temp, temp1);
			adicionaILOC(&($$->codigo), storeAI_OP, temp1, "rsp", "0");
			adicionaILOC(&($$->codigo), storeAI_OP, "rsp", "rsp", "4");
			adicionaILOC(&($$->codigo), storeAI_OP, "rfp", "rsp", "8");
			// rsp, 12 e reservado para o retorno da funcao

			num_params = contaParams(tempSimbolo->lista_param);
			contador = 16;
			tempAST = $3;
			
			while(tempAST != NULL && num_params > 0){				
				temp2 = int_to_string(contador);
				appendCod(&($$->codigo), tempAST->codigo); 								// codigo do parametro
				adicionaILOC(&($$->codigo), storeAI_OP, tempAST->local, "rsp", temp2); 	// empilha como var local da funcao chamada

				tempAST = ultimoFilho(tempAST); 
				contador = contador + 4;
				num_params--;
				free(temp2); temp2 = NULL;
			}

			adicionaILOC(&($$->codigo), jumpI_OP, tempSimbolo->label, NULL, NULL);

			$$->local = geraReg(&lista_ptr);
			adicionaILOC(&($$->codigo), loadAI_OP, "rsp", "12", $$->local); // recupera retorno

			free(temp); temp = NULL;
			temp1 = NULL;
			tempAST = NULL;
			tempSimbolo = NULL;
		}
		;

argumentos:	
		expressao mais_argumentos	
		{
			$$ = $1;
			if($2 != NULL){
				appendFilho($$, $2);
				//appendCod(&($$->codigo), $2->codigo);
			}
			lista_parametros = novoParametro(lista_parametros, $1->tipo);
		}

		|	/* PROD. VAZIA */							
		{
			$$ = NULL;
		}
		;

mais_argumentos: 
		',' expressao mais_argumentos	
		{
			$$ = $2;
			if($3 != NULL){
				appendFilho($$, $3);
				//appendCod(&($$->codigo), $3->codigo);
			}
			lista_parametros = novoParametro(lista_parametros, $2->tipo);
		}

		|	/* PROD. VAZIA */							
		{
			$$ = NULL;
		}
		;


//  5. Comandos de Shift
comand_shift:	
		identificador shift TK_LIT_INT	
		{
			temp = int_to_string($3.valor.i);
			lista[0] = $1; lista[1] = novoNodo(temp, TIPO_INT);
			free(temp); temp = NULL;
			$$ = $2; adicionaFilhos($$, lista, 2);
			$$->tipo = $1->tipo; 
			confereShift($3.valor.i, $3.num_linha);
		}

		| vetor shift TK_LIT_INT		
		{
			temp = int_to_string($3.valor.i);
			lista[0] = $1; lista[1] = novoNodo(temp, TIPO_INT);
			$$ = $2; adicionaFilhos($$, lista, 2);
			$$->tipo = $1->tipo;
			confereShift($3.valor.i, $3.num_linha);
		}
		;

//  6. Comando de Retorno, Break e Continue
return:	
		TK_PR_RETURN expressao	
		{
			lista[0] = $2;
			$$ = cria_e_adiciona("return", lista, 1, $2->tipo); 
			verificaReturn(pilha, $2->tipo, $1.num_linha);

			appendCod(&($$->codigo), $2->codigo);

			tempSimbolo =  encontraUltimaFuncao(pilha);
			if(strcmp(tempSimbolo->chave,"main") != 0){
				adicionaILOC(&($$->codigo), storeAI_OP, $2->local, "rfp", "12");
			}else{
				adicionaILOC(&($$->codigo), halt_OP, NULL, NULL, NULL);
			}

			tempSimbolo = NULL;
		}
		;

break: 		
		TK_PR_BREAK				
		{
			$$ = novoNodo("break", TIPO_NA);
		}
		;

continue:
		TK_PR_CONTINUE		
		{
			$$ = novoNodo("continue", TIPO_NA);
		}
		;

//  7. Comandos de Controle de Fluxo
controle_fluxo:
		if 			{$$ = $1;}
		| for 		{$$ = $1;}
		| while 	{$$ = $1;}
		;

if: 	
		TK_PR_IF '(' expressao ')' '{' comandos '}'				
		{
			lista[0] = $3; lista[1] = $6;
			$$ = cria_e_adiciona("if", lista, 2, TIPO_NA);

			rotulo = geraLabel(&lista_ptr);
			fazRemendo($3->l_true, rotulo);
			
			rotulo1 = geraLabel(&lista_ptr);
			fazRemendo($3->l_false, rotulo1);

			appendCod(&($$->codigo), $3->codigo);
			adicionaILOC(&($$->codigo), rotulo_OP, rotulo, NULL, NULL);
			if($6 != NULL){
				appendCod(&($$->codigo), $6->codigo);
			}
			adicionaILOC(&($$->codigo), rotulo_OP, rotulo1, NULL, NULL);

			rotulo = NULL; // free acontece em liberaILOC()
			rotulo1 = NULL;

		}

		| TK_PR_IF '(' expressao ')' '{' comandos '}' TK_PR_ELSE '{' comandos '}'	
		{
			lista[0] = $3; lista[1] = $6; lista[2] = $10;
			$$ = cria_e_adiciona("if", lista, 3, TIPO_NA);

			rotulo = geraLabel(&lista_ptr);
			fazRemendo($3->l_true, rotulo);
			
			rotulo1 = geraLabel(&lista_ptr);
			fazRemendo($3->l_false, rotulo1);
			
			rotulo2 = geraLabel(&lista_ptr);
			
			appendCod(&($$->codigo), $3->codigo);		// cod bool
			adicionaILOC(&($$->codigo), rotulo_OP, rotulo, NULL, NULL);
			if($6 != NULL){
				appendCod(&($$->codigo), $6->codigo); 	// cod if
			}
			adicionaILOC(&($$->codigo), jumpI_OP, rotulo2, NULL, NULL); 	//jump out
			adicionaILOC(&($$->codigo), rotulo_OP, rotulo1, NULL, NULL);
			if($10 != NULL){
				appendCod(&($$->codigo), $10->codigo); 	// cod else
			}
			adicionaILOC(&($$->codigo), jumpI_OP, rotulo2, NULL, NULL); 	//jump out
			adicionaILOC(&($$->codigo), rotulo_OP, rotulo2, NULL, NULL);	// out

			rotulo = NULL; rotulo1 = NULL; rotulo2 = NULL;
		}
		;

for:	
		TK_PR_FOR '(' atribuicao ':' expressao ':' atribuicao ')' '{' comandos '}'
		{
			lista[0] = $3; lista[1] = $5;
			lista[2] = $7; lista[3] = $10;
			$$ = cria_e_adiciona("for", lista, 4, TIPO_NA);

			rotulo = geraLabel(&lista_ptr);

			rotulo1 = geraLabel(&lista_ptr);
			fazRemendo($5->l_true, rotulo1);
			
			rotulo2 = geraLabel(&lista_ptr);
			fazRemendo($5->l_false, rotulo2);

			appendCod(&($$->codigo), $3->codigo); // atribuicao inicial
			adicionaILOC(&($$->codigo), rotulo_OP, rotulo, NULL, NULL); 	// avaliacao
			appendCod(&($$->codigo), $5->codigo); // cod bool
			adicionaILOC(&($$->codigo), rotulo_OP, rotulo1, NULL, NULL);	// true
			if($10 != NULL){
				appendCod(&($$->codigo), $10->codigo); 	// cod
			}
			appendCod(&($$->codigo), $7->codigo); // atribuicao repete
			adicionaILOC(&($$->codigo), jumpI_OP, rotulo, NULL, NULL);	// re-avalia
			adicionaILOC(&($$->codigo), rotulo_OP, rotulo2, NULL, NULL);	// false

			rotulo = NULL; rotulo1 = NULL; rotulo2 = NULL;

		}
		;

while:	
		TK_PR_WHILE '(' expressao ')' TK_PR_DO '{' comandos '}'	
		{
			lista[0] = $3; lista[1] = $7;
			$$ = cria_e_adiciona("while", lista, 2, TIPO_NA);

			rotulo = geraLabel(&lista_ptr);

			rotulo1 = geraLabel(&lista_ptr);
			fazRemendo($3->l_true, rotulo1);
			
			rotulo2 = geraLabel(&lista_ptr);
			fazRemendo($3->l_false, rotulo2);

			adicionaILOC(&($$->codigo), rotulo_OP, rotulo, NULL, NULL); 	// re-avalia
			appendCod(&($$->codigo), $3->codigo);		// cod bool
			adicionaILOC(&($$->codigo), rotulo_OP, rotulo1, NULL, NULL);	// true
			if($7 != NULL){
				appendCod(&($$->codigo), $7->codigo); 	// cod
			}
			adicionaILOC(&($$->codigo), jumpI_OP, rotulo, NULL, NULL);
			adicionaILOC(&($$->codigo), rotulo_OP, rotulo2, NULL, NULL);	// false

			rotulo = NULL; rotulo1 = NULL; rotulo2 = NULL;
		}
		;



/* -------   Expressoes   ------- */

expressao: 	
		identificador					{$$ = $1;}
		| literal						{$$ = $1;}
		| chamada_funcao				{$$ = $1;}
		| vetor							{$$ = $1;}
		| '+' expressao %prec UNARIO_L	
		{
			lista[0] = $2;
			$$ = cria_e_adiciona("+", lista, 1, $2->tipo); 

			appendCod(&($$->codigo), $2->codigo);
		}

		| '-' expressao %prec UNARIO_L	
		{
			lista[0] = $2;
			$$ = cria_e_adiciona("-", lista, 1, $2->tipo); 

			appendCod(&($$->codigo), $2->codigo);

			temp = geraReg(&lista_ptr);
			$$->local = geraReg(&lista_ptr);
			adicionaILOC(&($$->codigo), loadI_OP, "0", NULL, temp);
			adicionaILOC(&($$->codigo), sub_OP, temp, $2->local, $$->local); 

			temp = NULL;
		}

		| '!' expressao					
		{
			lista[0] = $2;
			$$ = cria_e_adiciona("!", lista, 1, $2->tipo); 

			appendCod(&($$->codigo), $2->codigo);
			$$->l_false = $2->l_true;
			$$->l_true = $2->l_false;
		}

		| '&' expressao %prec UNARIO_R	{lista[0] = $2;
										$$ = cria_e_adiciona("&", lista, 1, $2->tipo); }
		| '*' expressao %prec UNARIO_R	{lista[0] = $2;
										$$ = cria_e_adiciona("*", lista, 1, $2->tipo); }
		| '?' expressao					{lista[0] = $2;
										$$ = cria_e_adiciona("?", lista, 1, $2->tipo); }
		| '#' expressao					{lista[0] = $2;
										$$ = cria_e_adiciona("#", lista, 1, $2->tipo); }										
    	| expressao '+' expressao		
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona("+", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha)); 
			$$->local = geraReg(&lista_ptr);
			appendCod(&($$->codigo), $1->codigo);
			appendCod(&($$->codigo), $3->codigo);
			adicionaILOC(&($$->codigo), add_OP, $1->local, $3->local, $$->local);
		}
		
    	| expressao '-' expressao		
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona("-", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha));
			$$->local = geraReg(&lista_ptr);
			appendCod(&($$->codigo), $1->codigo);
			appendCod(&($$->codigo), $3->codigo);
			adicionaILOC(&($$->codigo), sub_OP, $1->local, $3->local, $$->local); 
		}

    	| expressao '*' expressao		
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona("*", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha)); 
			$$->local = geraReg(&lista_ptr);
			appendCod(&($$->codigo), $1->codigo);
			appendCod(&($$->codigo), $3->codigo);
			adicionaILOC(&($$->codigo), mult_OP, $1->local, $3->local, $$->local);
		}

   		| expressao '/' expressao		
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona("/", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha)); 
			$$->local = geraReg(&lista_ptr);
			appendCod(&($$->codigo), $1->codigo);
			appendCod(&($$->codigo), $3->codigo);
			adicionaILOC(&($$->codigo), div_OP, $1->local, $3->local, $$->local);
		}

  		| expressao '<' expressao		 
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona("<", lista, 2, TIPO_BOOL);

			temp = geraReg(&lista_ptr);
			appendCod(&($$->codigo), $1->codigo);
			appendCod(&($$->codigo), $3->codigo);
			adicionaILOC(&($$->codigo), cmp_LT_OP, $1->local, $3->local, temp);
			adicionaILOC(&($$->codigo), cbr_OP, temp, "**", "**"); // remendos

			temp_ILOC = ultimoILOC($$->codigo);
			
			novoPTR(temp_ILOC->end2, &($$->l_true) );
			novoPTR(temp_ILOC->dest, &($$->l_false) );

			temp_ILOC = NULL;

			temp = NULL;

		}

 		| expressao '>' expressao		
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona(">", lista, 2, TIPO_BOOL); 
			
			temp = geraReg(&lista_ptr);
			appendCod(&($$->codigo), $1->codigo);
			appendCod(&($$->codigo), $3->codigo);
			adicionaILOC(&($$->codigo), cmp_GT_OP, $1->local, $3->local, temp);
			adicionaILOC(&($$->codigo), cbr_OP, temp, "**", "**"); // remendos

			temp_ILOC = ultimoILOC($$->codigo);
			
			novoPTR(temp_ILOC->end2, &($$->l_true) );
			novoPTR(temp_ILOC->dest, &($$->l_false) );

			temp_ILOC = NULL;

			temp = NULL;

		}

   		| expressao '|' expressao		
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona("|", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha)); 
		}

		| expressao '%' expressao		
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona("%", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha)); 
		}

    	| expressao '^' expressao		
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona("^", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha)); 
		}

    	| expressao '&' expressao		
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona("&", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha)); 
		}
    	| expressao TK_OC_LE expressao	
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona("<=", lista, 2, TIPO_BOOL); 
			
			temp = geraReg(&lista_ptr);
			appendCod(&($$->codigo), $1->codigo);
			appendCod(&($$->codigo), $3->codigo);
			adicionaILOC(&($$->codigo), cmp_LE_OP, $1->local, $3->local, temp);
			adicionaILOC(&($$->codigo), cbr_OP, temp, "**", "**"); // remendos

			temp_ILOC = ultimoILOC($$->codigo);
			
			novoPTR(temp_ILOC->end2, &($$->l_true) );
			novoPTR(temp_ILOC->dest, &($$->l_false) );

			temp_ILOC = NULL;

			temp = NULL;
		}

    	| expressao TK_OC_GE expressao	
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona(">=", lista, 2, TIPO_BOOL); 
			
			temp = geraReg(&lista_ptr);
			appendCod(&($$->codigo), $1->codigo);
			appendCod(&($$->codigo), $3->codigo);
			adicionaILOC(&($$->codigo), cmp_GE_OP, $1->local, $3->local, temp);
			adicionaILOC(&($$->codigo), cbr_OP, temp, "**", "**"); // remendos

			temp_ILOC = ultimoILOC($$->codigo);
			
			novoPTR(temp_ILOC->end2, &($$->l_true) );
			novoPTR(temp_ILOC->dest, &($$->l_false) );

			temp_ILOC = NULL;

			temp = NULL;
		}

    	| expressao TK_OC_EQ expressao	
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona("==", lista, 2, TIPO_BOOL); 
			
			temp = geraReg(&lista_ptr);
			appendCod(&($$->codigo), $1->codigo);
			appendCod(&($$->codigo), $3->codigo);
			adicionaILOC(&($$->codigo), cmp_EQ_OP, $1->local, $3->local, temp);
			adicionaILOC(&($$->codigo), cbr_OP, temp, "**", "**"); // remendos

			temp_ILOC = ultimoILOC($$->codigo);
			
			novoPTR(temp_ILOC->end2, &($$->l_true) );
			novoPTR(temp_ILOC->dest, &($$->l_false) );

			temp_ILOC = NULL;

			temp = NULL;
		}

    	| expressao TK_OC_NE expressao	
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona("!=", lista, 2, TIPO_BOOL); 
			
			temp = geraReg(&lista_ptr);
			appendCod(&($$->codigo), $1->codigo);
			appendCod(&($$->codigo), $3->codigo);
			adicionaILOC(&($$->codigo), cmp_NE_OP, $1->local, $3->local, temp);
			adicionaILOC(&($$->codigo), cbr_OP, temp, "**", "**"); // remendos

			temp_ILOC = ultimoILOC($$->codigo);
			
			novoPTR(temp_ILOC->end2, &($$->l_true) );
			novoPTR(temp_ILOC->dest, &($$->l_false) );

			temp_ILOC = NULL;

			temp = NULL;
		}

    	| expressao TK_OC_AND expressao	
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona("&&", lista, 2, TIPO_BOOL); 
			
			temp = geraLabel(&lista_ptr);
			fazRemendo($1->l_true, temp);

			$$->l_true = $3->l_true;
			$$->l_false = concatLista($1->l_false, $3->l_false);

			appendCod(&($$->codigo), $1->codigo);
			adicionaILOC(&($$->codigo), rotulo_OP, temp, NULL, NULL);
			appendCod(&($$->codigo), $3->codigo);

			temp = NULL;
		}

    	| expressao TK_OC_OR expressao	
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona("||", lista, 2, TIPO_BOOL); 
			
			temp = geraLabel(&lista_ptr);
			fazRemendo($1->l_false, temp);

			$$->l_false = $3->l_false;
			$$->l_true = concatLista($1->l_true, $3->l_true);

			appendCod(&($$->codigo), $1->codigo);
			adicionaILOC(&($$->codigo), rotulo_OP, temp, NULL, NULL);
			appendCod(&($$->codigo), $3->codigo);

			temp = NULL;

		}

    	| '(' expressao ')'				{$$ = $2;}
    	| expressao '?' expressao ':' expressao %prec TERNARIO
		{
			lista[0] = $1; lista[1] = $3; lista[2] = $5;
			$$ = cria_e_adiciona("?:", lista, 3, TIPO_NA); /* Nao sei que tipo deve ser aqui */
		}
    	;





/* -------   Gerais   ------- */

vetor:		
		TK_IDENTIFICADOR '[' expressao ']'	
		{
			lista[0] = novoNodo($1.valor.cad_char, recuperaTipo(pilha, $1.valor.cad_char, $1.num_linha)); lista[1] = $3;
			$$ = cria_e_adiciona("[]", lista, 2, recuperaTipo(pilha, $1.valor.cad_char, $1.num_linha));
			confereNatureza(pilha, $1.valor.cad_char, VETOR, $1.num_linha);
		}
		;

tipo: 		
		TK_PR_INT 		{$$ = TIPO_INT;}
		| TK_PR_FLOAT 	{$$ = TIPO_FLOAT;}
		| TK_PR_CHAR 	{$$ = TIPO_CHAR;}
		| TK_PR_BOOL 	{$$ = TIPO_BOOL;}
		| TK_PR_STRING	{$$ = TIPO_STRING;}
		;

literal:	
		int 			{$$ = $1;}
		| float 		{$$ = $1;}
		| false 		{$$ = $1;}
		| true 			{$$ = $1;}
		;

literal_nao_expr: 	
		literal 		{$$ = $1;}
		| string 		{$$ = $1;}
		| char 			{$$ = $1;}
		;

int:  
		TK_LIT_INT 	
		{
			temp = int_to_string($1.valor.i);
			pilha->atual = adicionaEntradaTabela(pilha->atual, temp, $1.num_linha, LIT, TIPO_INT, $1, 1, 9999, GLOBAL);
			$$ = novoNodo(temp, TIPO_INT);
			
			$$->local = geraReg(&lista_ptr);
			adicionaILOC(&($$->codigo), loadI_OP, temp, NULL, $$->local);

			free(temp); temp = NULL;
		}
	  	;

float: 
		TK_LIT_FLOAT	
		{
			temp = float_to_string($1.valor.f);
			pilha->atual = adicionaEntradaTabela(pilha->atual, temp, $1.num_linha, LIT, TIPO_FLOAT, $1, 1, 9999, GLOBAL);
			$$ = novoNodo(temp, TIPO_FLOAT);
			free(temp); temp = NULL;
		}
	   ;

false: 	
		TK_LIT_FALSE	
		{
			$$ = novoNodo("false", TIPO_BOOL);
			pilha->atual = adicionaEntradaTabela(pilha->atual, "false", $1.num_linha, LIT, TIPO_BOOL, $1, 1, 9999, GLOBAL);
		}
		;

true: 	
		TK_LIT_TRUE		
		{
			$$ = novoNodo("true", TIPO_BOOL);
			pilha->atual = adicionaEntradaTabela(pilha->atual, "true", $1.num_linha, LIT, TIPO_BOOL, $1, 1, 9999, GLOBAL);
		}
		;

string:	
		TK_LIT_STRING	
		{
			$$ = novoNodo($1.valor.str, TIPO_STRING);
			pilha->atual = adicionaEntradaTabela(pilha->atual, $1.valor.str, $1.num_linha, LIT, TIPO_STRING, $1, strlen($1.valor.str), 9999, GLOBAL);
		}
		;

char:	
		TK_LIT_CHAR		
		{
			temp = char_to_string($1.valor.c);
			pilha->atual = adicionaEntradaTabela(pilha->atual, temp, $1.num_linha, LIT, TIPO_CHAR, $1, 1, 9999, GLOBAL);
			$$ = novoNodo(temp, TIPO_CHAR);
			free(temp); temp = NULL;
		}
		;

shift:		
		TK_OC_SL	
		{
			$$ = novoNodo("<<", TIPO_NA); /* tipo atribuido depois */ 
		}

		| TK_OC_SR		
		{
			$$ = novoNodo(">>", TIPO_NA);  
		} 
		;

identificador: 	
		TK_IDENTIFICADOR	
		{
			$$ = novoNodo($1.valor.cad_char, recuperaTipo(pilha, $1.valor.cad_char, $1.num_linha));
			confereNatureza(pilha, $1.valor.cad_char, VAR, $1.num_linha);

			temp = int_to_string(recuperaDesloc($1.valor.cad_char, pilha));
			$$->local = geraReg(&lista_ptr);
			adicionaILOC(&($$->codigo), loadAI_OP, recuperaEscopo(&lista_ptr, $1.valor.cad_char, pilha), temp, $$->local);

			free(temp); temp = NULL;
		} 
		;

static: 
		TK_PR_STATIC
		|
		;

const: 	
		TK_PR_CONST
		|
		;


%%

int yyerror (char const *s) {
   fprintf (stderr, "%s (line: %d)\n", s, get_line_number());
   liberaPTR(lista_ptr);
   return 1;
 }
