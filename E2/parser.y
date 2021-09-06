%{
#include <stdio.h>
int yylex(void);
int yyerror (char const *s);
%}

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
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
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
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_OC_SL
%token TK_OC_SR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_LIT_STRING
%token TK_IDENTIFICADOR
%token TOKEN_ERRO

%start programa

%%

programa:	lista_var_global_func; 

lista_var_global_func: 
		var_global lista_var_global_func
		| func lista_var_global_func
		|
		;
		
/* -------   Variaveis globais    ------- */
var_global: 	static tipo TK_IDENTIFICADOR nomes_g ';'
		| static tipo TK_IDENTIFICADOR '[' TK_LIT_INT ']' nomes_g ';'
		;

nomes_g: 	',' TK_IDENTIFICADOR nomes_g
		| ',' TK_IDENTIFICADOR '[' TK_LIT_INT ']' nomes_g
		|
		;
	
/* -------   Funcoes   ------- */

func: 		cabecalho bloco
		;
	
cabecalho:	static tipo TK_IDENTIFICADOR '(' parametros ')'
		;
		
parametros:	const tipo TK_IDENTIFICADOR mais_parametros
		|
		;
		
mais_parametros:
		',' const tipo TK_IDENTIFICADOR mais_parametros
		|
		;
		
/* -------   Bloco de Comandos   ------- */

bloco: 	'{' comandos '}';

comandos: 	comando_simples comandos
		|
		;
		
comando_simples:
		//var local
		static const tipo TK_IDENTIFICADOR inicializacao nomes_l ';'
		;
		
nomes_l: 	',' TK_IDENTIFICADOR inicializacao nomes_l
		|
		;
		
inicializacao:	TK_OC_LE TK_IDENTIFICADOR
		| TK_OC_LE literal
		|
		;
		


/* -------   Gerais   ------- */
tipo: 		TK_PR_INT
		| TK_PR_FLOAT
		| TK_PR_CHAR
		| TK_PR_BOOL
		| TK_PR_STRING
		;
		
literal: 	TK_LIT_INT
 		|TK_LIT_FLOAT
 		|TK_LIT_FALSE
		|TK_LIT_TRUE
 		|TK_LIT_CHAR
 		|TK_LIT_STRING
 		;

static: 	TK_PR_STATIC
		|
		;
		
const: 	TK_PR_CONST
		|
		;




%%

int yyerror (char const *s) {
   fprintf (stderr, "%s\n", s);
   return 1;
 }


   
 






