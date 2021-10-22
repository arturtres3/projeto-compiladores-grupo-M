%{
	// Grupo M
	// Eduardo Henrique Ferreira do Nascimento (00260846)
	// Artur Tres do Amaral (00287682)
	#include <stdio.h>
	#define YYERROR_VERBOSE 1
	extern void *arvore;

	int yylex(void);
	int yyerror (char const *s);
	int get_line_number(void);


	char* temp = NULL; //variavel para colocar literais em strings
%}

%code requires{
	#include "valor_lexico.h"
	#include "ast.h"
	#include "tabela.h"
	#include "tipos.h"
}

%code{
	extern LISTA_PTR* lista_ptr;
	AST* lista[10]; //lista de filhos
	extern pilha_tabela *pilha;
}

%union{
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

programa:	lista_var_global_func			{$$ = $1; arvore = $$;
											liberaPTR(lista_ptr);}
			;

lista_var_global_func:
		var_global lista_var_global_func	{$$ = $2;}
		| func lista_var_global_func		{$$ = $1;
											if($2 != NULL){appendFilho($$, $2);}}
		|									{$$ = NULL;}
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

func: 		cabecalho bloco		{lista[0] = $2;
								$$ = cria_e_adiciona($1.valor.cad_char, lista, 1);}
		;

cabecalho:	static tipo TK_IDENTIFICADOR '(' parametros ')' {$$ = $3;}
		;

parametros:	const tipo TK_IDENTIFICADOR mais_parametros
		|
		;

mais_parametros:
		',' const tipo TK_IDENTIFICADOR mais_parametros
		|
		;

/* -------   Bloco de Comandos   ------- */

bloco: 	'{' comandos '}'					{$$ = $2;}
		;


comandos: 	comando_simples ';' comandos	{if($1 != NULL)
												{$$ = $1; appendFilho($$, $3);}
											else {$$ = $3;}
											}
		|									{$$ = NULL;}
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
declaracao: 	static const tipo TK_IDENTIFICADOR nomes_l	{$$ = $5;}
				| static const tipo inicializacao nomes_l	{$$ = $4; appendFilho($$, $5);}
				;

inicializacao:	identificador TK_OC_LE identificador		{lista[0] = $1; lista[1] = $3;
															$$ = cria_e_adiciona("<=", lista, 2);}
				| identificador TK_OC_LE literal_nao_expr  	{lista[0] = $1; lista[1] = $3;
															$$ = cria_e_adiciona("<=", lista, 2);}
				;

nomes_l: 		',' TK_IDENTIFICADOR nomes_l	{$$ = $3;}
				|',' inicializacao nomes_l		{$$ = $2; appendFilho($$, $3);}
				|								{$$ = NULL;}
				;

//  2. Comando de Atribuicao
atribuicao: 	identificador '=' expressao	{lista[0] = $1; lista[1] = $3;
											$$ = cria_e_adiciona("=", lista, 2);}
				| vetor '=' expressao		{lista[0] = $1; lista[1] = $3;
											$$ = cria_e_adiciona("=", lista, 2);}
				;

//  3. Comando de Entrada e Saida
entrada_saida:	TK_PR_INPUT identificador	{lista[0] = $2;
											$$ = cria_e_adiciona("input", lista, 1);}
		| TK_PR_OUTPUT identificador		{lista[0] = $2;
											$$ = cria_e_adiciona("output", lista, 1);}
		| TK_PR_OUTPUT literal_nao_expr		{lista[0] = $2;
											$$ = cria_e_adiciona("output", lista, 1);}
		;

//  4. Chamada de Funcao
chamada_funcao: TK_IDENTIFICADOR '(' argumentos ')'	{lista[0] = $3;
													temp = label_chamada($1.valor.cad_char);
													$$ = cria_e_adiciona(temp, lista, 1);
													free(temp); temp = NULL;}
		;

argumentos:	expressao mais_argumentos	{$$ = $1;
										appendFilho($$, $2);}
		|								{$$ = NULL;}
		;

mais_argumentos:
		',' expressao mais_argumentos	{$$ = $2;
										appendFilho($$, $3);}
		|								{$$ = NULL;}
		;


//  5. Comandos de Shift
comand_shift:	identificador shift int	{lista[0] = $1; lista[1] = $3;
										$$ = $2; adicionaFilhos($$, lista, 2);}
				| vetor shift int		{lista[0] = $1; lista[1] = $3;
										$$ = $2; adicionaFilhos($$, lista, 2);}
				;

//  6. Comando de Retorno, Break e Continue
return:	TK_PR_RETURN expressao	{lista[0] = $2;
								$$ = cria_e_adiciona("return", lista, 1);}
		;

break: 		TK_PR_BREAK				{$$ = novoNodo("break");}
			;

continue:	TK_PR_CONTINUE		{$$ = novoNodo("continue");}
			;

//  7. Comandos de Controle de Fluxo
controle_fluxo:if 	{$$ = $1;}
		| for 		{$$ = $1;}
		| while 	{$$ = $1;}
		;

if: 		TK_PR_IF '(' expressao ')' bloco				{lista[0] = $3; lista[1] = $5;
															$$ = cria_e_adiciona("if", lista, 2);}
		| TK_PR_IF '(' expressao ')' bloco TK_PR_ELSE bloco	{lista[0] = $3; lista[1] = $5; lista[2] = $7;
															$$ = cria_e_adiciona("if", lista, 3);}
		;

for:		TK_PR_FOR '(' atribuicao ':' expressao ':' atribuicao ')' bloco
															{lista[0] = $3; lista[1] = $5;
															lista[2] = $7; lista[3] = $9;
															$$ = cria_e_adiciona("for", lista, 4);}
		;

while:		TK_PR_WHILE '(' expressao ')' TK_PR_DO bloco	{lista[0] = $3; lista[1] = $6;
															$$ = cria_e_adiciona("while", lista, 2);}
		;



/* -------   Expressoes   ------- */

expressao: 	identificador	{$$ = $1;}
		| literal			{$$ = $1;}
		| chamada_funcao	{$$ = $1;}
		| vetor				{$$ = $1;}
		| '+' expressao %prec UNARIO_L	{lista[0] = $2;
										$$ = cria_e_adiciona("+", lista, 1);}
		| '-' expressao %prec UNARIO_L	{lista[0] = $2;
										$$ = cria_e_adiciona("-", lista, 1);}
		| '!' expressao					{lista[0] = $2;
										$$ = cria_e_adiciona("!", lista, 1);}
		| '&' expressao %prec UNARIO_R	{lista[0] = $2;
										$$ = cria_e_adiciona("&", lista, 1);}
		| '*' expressao %prec UNARIO_R	{lista[0] = $2;
										$$ = cria_e_adiciona("*", lista, 1);}
		| '?' expressao					{lista[0] = $2;
										$$ = cria_e_adiciona("?", lista, 1);}
		| '#' expressao					{lista[0] = $2;
										$$ = cria_e_adiciona("#", lista, 1);}

    	| expressao '+' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("+", lista, 2);}
    	| expressao '-' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("-", lista, 2);}
    	| expressao '*' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("*", lista, 2);}
   		| expressao '/' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("/", lista, 2);}
  		| expressao '<' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("<", lista, 2);}
 		| expressao '>' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona(">", lista, 2);}
   		| expressao '|' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("|", lista, 2);}
		| expressao '%' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("%", lista, 2);}
    	| expressao '^' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("^", lista, 2);}
    	| expressao '&' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("&", lista, 2);}
    	| expressao TK_OC_LE expressao	{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("<=", lista, 2);}
    	| expressao TK_OC_GE expressao	{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona(">=", lista, 2);}
    	| expressao TK_OC_EQ expressao	{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("==", lista, 2);}
    	| expressao TK_OC_NE expressao	{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("!=", lista, 2);}
    	| expressao TK_OC_AND expressao	{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("&&", lista, 2);}
    	| expressao TK_OC_OR expressao	{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("||", lista, 2);}
    	| '(' expressao ')'				{$$ = $2;}
    	| expressao '?' expressao ':' expressao %prec TERNARIO
										{lista[0] = $1; lista[1] = $3; lista[2] = $5;
										$$ = cria_e_adiciona("?:", lista, 3);
										}
    	;





/* -------   Gerais   ------- */

vetor:		identificador '[' expressao ']'	{lista[0] = $1; lista[1] = $3;
											$$ = cria_e_adiciona("[]", lista, 2);}
		;

tipo: 		TK_PR_INT
		| TK_PR_FLOAT
		| TK_PR_CHAR
		| TK_PR_BOOL
		| TK_PR_STRING
		;

literal:	int 	{$$ = $1;}
			| float {$$ = $1;}
			| false {$$ = $1;}
			| true 	{$$ = $1;}
			;

literal_nao_expr: 	literal 	{$$ = $1;}
					| string 	{$$ = $1;}
					| char 		{$$ = $1;}
					;

int:  TK_LIT_INT 	{temp = int_to_string($1.valor.i);
					$$ = novoNodo(temp);
					free(temp); temp = NULL;}
	  ;

float: TK_LIT_FLOAT	{temp = float_to_string($1.valor.f);
					$$ = novoNodo(temp);
					free(temp); temp = NULL;}
	   ;

false: 	TK_LIT_FALSE	{$$ = novoNodo("false");}
		;

true: 	TK_LIT_TRUE		{$$ = novoNodo("true");}
		;

string:	TK_LIT_STRING	{$$ = novoNodo($1.valor.str);}
		;

char:	TK_LIT_CHAR		{temp = char_to_string($1.valor.c);
						$$ = novoNodo(temp);
						free(temp); temp = NULL;}
		;

shift:		TK_OC_SL	{$$ = novoNodo("<<");}
		| TK_OC_SR		{$$ = novoNodo(">>");}
		;

identificador: 	TK_IDENTIFICADOR	{$$ = novoNodo($1.valor.cad_char);}
				;

static: TK_PR_STATIC
		|
		;

const: 	TK_PR_CONST
		|
		;




%%

int yyerror (char const *s) {
   fprintf (stderr, "%s (line: %d)\n", s, get_line_number());
   liberaPTR(lista_ptr);
   return 1;
 }
