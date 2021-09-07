%{
	// Grupo M
    // Eduardo Henrique Ferreira do Nascimento (00260846)
    // Artur Tres do Amaral (00287682)
#include <stdio.h>
#define YYERROR_VERBOSE 1
int yylex(void);
int yyerror (char const *s);
int get_line_number(void);
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
		| ',' vetor nomes_g
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


comandos: 	comando_simples ';' comandos
		|
		;
		
comando_simples:
		declaracao
		| atribuicao
		| entrada_saida
		| chamada_funcao
		| comand_shift
		| return
		| break
		| continue
		| controle_fluxo
		| bloco
		;

//  1. Declaracao Variavel local
declaracao: 	static const tipo TK_IDENTIFICADOR inicializacao nomes_l
		;
		
inicializacao:	TK_OC_LE TK_IDENTIFICADOR
		| TK_OC_LE literal
		|
		;
		
nomes_l: 	',' TK_IDENTIFICADOR inicializacao nomes_l
		|
		;
		
//  2. Comando de Atribuicao
atribuicao: 	TK_IDENTIFICADOR '=' expressao 
		| vetor '=' expressao
		;
		
//  3. Comando de Entrada e Saida
entrada_saida:	TK_PR_INPUT TK_IDENTIFICADOR
		| TK_PR_OUTPUT TK_IDENTIFICADOR
		| TK_PR_OUTPUT literal
		;
		
//  4. Chamada de Funcao
chamada_funcao:TK_IDENTIFICADOR '(' argumentos ')'
		;
		
argumentos:	expressao mais_argumentos
		|
		;

mais_argumentos:
		',' expressao mais_argumentos
		|
		;
		
		
//  5. Comandos de Shift
comand_shift:	TK_IDENTIFICADOR shift TK_LIT_INT
		| vetor shift TK_LIT_INT
		;
		
//  6. Comando de Retorno, Break e Continue
return:	TK_PR_RETURN expressao;

break: 	TK_PR_BREAK;

continue:	TK_PR_CONTINUE;

//  7. Comandos de Controle de Fluxo
controle_fluxo:if
		| for
		| while
		;

if: 		TK_PR_IF '(' expressao ')' bloco
		| TK_PR_IF '(' expressao ')' bloco TK_PR_ELSE bloco
		;
		
for:		TK_PR_FOR '(' atribuicao ':' expressao ':' atribuicao ')' bloco
		;
		
while:		TK_PR_WHILE '(' expressao ')' TK_PR_DO bloco
		;


		
/* -------   Expressoes   ------- */

expressao: 	TK_IDENTIFICADOR
		| TK_LIT_INT
		| TK_LIT_FLOAT
		| TK_LIT_FALSE
		| TK_LIT_TRUE
		| chamada_funcao
		| vetor
		| '+' expressao %prec UNARIO_L
		| '-' expressao %prec UNARIO_L
		| '!' expressao
		| '&' expressao %prec UNARIO_R
		| '*' expressao %prec UNARIO_R
		| '?' expressao
		| '#' expressao 
    		| expressao '+' expressao
    		| expressao '-' expressao
    		| expressao '*' expressao
   		| expressao '/' expressao
  		| expressao '<' expressao
 		| expressao '>' expressao
   		| expressao '|' expressao
		| expressao '%' expressao
    		| expressao '^' expressao
    		| expressao '&' expressao
    		| expressao TK_OC_LE expressao
    		| expressao TK_OC_GE expressao
    		| expressao TK_OC_EQ expressao
    		| expressao TK_OC_NE expressao
    		| expressao TK_OC_AND expressao
    		| expressao TK_OC_OR expressao
    		| '(' expressao ')'
    		| expressao '?' expressao ':' expressao %prec TERNARIO
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
 		
shift:		TK_OC_SL
		| TK_OC_SR
		;
		
vetor:		TK_IDENTIFICADOR '[' expressao ']' 
		;

static: 	TK_PR_STATIC
		|
		;
		
const: 	TK_PR_CONST
		|
		;




%%

int yyerror (char const *s) {
   fprintf (stderr, "%s (line: %d)\n", s, get_line_number());
   return 1;
 }


   
 






