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
%}

%code requires{
	#include "include/valor_lexico.h"
	#include "include/ast.h"
	#include "include/tabela.h"
	#include "include/tipos.h"
}

%code{
	extern LISTA_PTR* lista_ptr;
	AST* lista[10]; //lista de filhos
	extern pilha_tabela *pilha;

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
			$$ = $1; arvore = $$; //printPilha(pilha);  
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
			$$ = $1; if($2 != NULL){appendFilho($$, $2);}
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
			pilha->atual = adicionaListaVar(pilha->atual, lista_variaveis, $2); 
			liberaListaVar(lista_variaveis); lista_variaveis = NULL;
		}

		| static tipo TK_IDENTIFICADOR '[' TK_LIT_INT ']' nomes_g ';'	
		{
			lista_variaveis = novoListaVar(lista_variaveis, $3.valor.cad_char, $5.valor.i, $3.num_linha, 1, $3, TIPO_NA);
			pilha->atual = adicionaListaVar(pilha->atual, lista_variaveis, $2); 
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
		}
		;

cabecalho:	
		static tipo TK_IDENTIFICADOR '(' parametros ')' 
		{
			$$ = $3; 
			pilha->atual = adicionaEntradaTabelaFunc(pilha->atual, $3.valor.cad_char, $3.num_linha, FUNC, $2, $3, 1, lista_parametros);
			liberaParams(lista_parametros); lista_parametros = NULL;
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
			pilha->atual = adicionaListaVar(pilha->atual, lista_variaveis, TIPO_NA);
			liberaListaVar(lista_variaveis); lista_variaveis = NULL;
		}
		;

fecha_bloco:	
		'}'		
		{
			pilha = fechaEscopo(pilha);
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
			}else {
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
			pilha->atual = adicionaListaVar(pilha->atual, lista_variaveis, $3);
			liberaListaVar(lista_variaveis); lista_variaveis = NULL;
			confereInicializacao(pilha, $$, $3, $4.num_linha);
		}

		| static const tipo inicializacao nomes_l		
		{
			$$ = $4; appendFilho($$, $5);
			pilha->atual = adicionaListaVar(pilha->atual, lista_variaveis, $3);
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
		identificador '=' expressao	
		{
			lista[0] = $1; lista[1] = $3;
			$$ = cria_e_adiciona("=", lista, 2, $1->tipo);
			confereAtribuicao($1->tipo, $3->tipo, $2.num_linha);
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
		}
		;

argumentos:	
		expressao mais_argumentos	
		{
			$$ = $1;appendFilho($$, $2);
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
			$$ = $2;appendFilho($$, $3);
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
		TK_PR_IF '(' expressao ')' bloco				
		{
			lista[0] = $3; lista[1] = $5;
			$$ = cria_e_adiciona("if", lista, 2, TIPO_NA);
		}

		| TK_PR_IF '(' expressao ')' bloco TK_PR_ELSE bloco	
		{
			lista[0] = $3; lista[1] = $5; lista[2] = $7;
			$$ = cria_e_adiciona("if", lista, 3, TIPO_NA);
		}
		;

for:	
		TK_PR_FOR '(' atribuicao ':' expressao ':' atribuicao ')' bloco
		{
			lista[0] = $3; lista[1] = $5;
			lista[2] = $7; lista[3] = $9;
			$$ = cria_e_adiciona("for", lista, 4, TIPO_NA);
		}
		;

while:	
		TK_PR_WHILE '(' expressao ')' TK_PR_DO bloco	
		{
			lista[0] = $3; lista[1] = $6;
			$$ = cria_e_adiciona("while", lista, 2, TIPO_NA);
		}
		;



/* -------   Expressoes   ------- */

expressao: 	
		identificador					{$$ = $1;}
		| literal						{$$ = $1;}
		| chamada_funcao				{$$ = $1;}
		| vetor							{$$ = $1;}
		| '+' expressao %prec UNARIO_L	{lista[0] = $2;
										$$ = cria_e_adiciona("+", lista, 1, $2->tipo); }
		| '-' expressao %prec UNARIO_L	{lista[0] = $2;
										$$ = cria_e_adiciona("-", lista, 1, $2->tipo); }
		| '!' expressao					{lista[0] = $2;
										$$ = cria_e_adiciona("!", lista, 1, $2->tipo); }
		| '&' expressao %prec UNARIO_R	{lista[0] = $2;
										$$ = cria_e_adiciona("&", lista, 1, $2->tipo); }
		| '*' expressao %prec UNARIO_R	{lista[0] = $2;
										$$ = cria_e_adiciona("*", lista, 1, $2->tipo); }
		| '?' expressao					{lista[0] = $2;
										$$ = cria_e_adiciona("?", lista, 1, $2->tipo); }
		| '#' expressao					{lista[0] = $2;
										$$ = cria_e_adiciona("#", lista, 1, $2->tipo); }										
    	| expressao '+' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("+", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha)); }
    	| expressao '-' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("-", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha)); }
    	| expressao '*' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("*", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha)); }
   		| expressao '/' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("/", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha)); }
  		| expressao '<' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("<", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha)); }
 		| expressao '>' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona(">", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha)); }
   		| expressao '|' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("|", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha)); }
		| expressao '%' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("%", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha)); }
    	| expressao '^' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("^", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha)); }
    	| expressao '&' expressao		{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("&", lista, 2, inferencia_tipo($1->tipo, $3->tipo, $2.num_linha)); }
    	| expressao TK_OC_LE expressao	{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("<=", lista, 2, TIPO_BOOL); }
    	| expressao TK_OC_GE expressao	{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona(">=", lista, 2, TIPO_BOOL); }
    	| expressao TK_OC_EQ expressao	{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("==", lista, 2, TIPO_BOOL); }
    	| expressao TK_OC_NE expressao	{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("!=", lista, 2, TIPO_BOOL); }
    	| expressao TK_OC_AND expressao	{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("&&", lista, 2, TIPO_BOOL); }
    	| expressao TK_OC_OR expressao	{lista[0] = $1; lista[1] = $3;
										$$ = cria_e_adiciona("||", lista, 2, TIPO_BOOL); }
    	| '(' expressao ')'				{$$ = $2;}
    	| expressao '?' expressao ':' expressao %prec TERNARIO
										{lista[0] = $1; lista[1] = $3; lista[2] = $5;
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
			adicionaEntradaTabela(pilha->atual, temp, $1.num_linha, LIT, TIPO_INT, $1, 1);
			$$ = novoNodo(temp, TIPO_INT);
			free(temp); temp = NULL;
		}
	  	;

float: 
		TK_LIT_FLOAT	
		{
			temp = float_to_string($1.valor.f);
			adicionaEntradaTabela(pilha->atual, temp, $1.num_linha, LIT, TIPO_FLOAT, $1, 1);
			$$ = novoNodo(temp, TIPO_FLOAT);
			free(temp); temp = NULL;
		}
	   ;

false: 	
		TK_LIT_FALSE	
		{
			$$ = novoNodo("false", TIPO_BOOL);
			adicionaEntradaTabela(pilha->atual, "false", $1.num_linha, LIT, TIPO_BOOL, $1, 1);
		}
		;

true: 	
		TK_LIT_TRUE		
		{
			$$ = novoNodo("true", TIPO_BOOL);
			adicionaEntradaTabela(pilha->atual, "true", $1.num_linha, LIT, TIPO_BOOL, $1, 1);
		}
		;

string:	
		TK_LIT_STRING	
		{
			$$ = novoNodo($1.valor.str, TIPO_STRING);
			adicionaEntradaTabela(pilha->atual, $1.valor.str, $1.num_linha, LIT, TIPO_STRING, $1, strlen($1.valor.str));
		}
		;

char:	
		TK_LIT_CHAR		
		{
			temp = char_to_string($1.valor.c);
			adicionaEntradaTabela(pilha->atual, temp, $1.num_linha, LIT, TIPO_CHAR, $1, 1);
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
