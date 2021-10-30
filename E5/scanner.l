%{
    // Grupo M
	// Eduardo Henrique Ferreira do Nascimento (00260846)
	// Artur Tres do Amaral (00287682)
#include "parser.tab.h"
#include "include/valor_lexico.h"
int get_line_number(void);
int line_number = 1;
LISTA_PTR* lista_ptr = NULL;

%}
%option noyywrap

digito [0-9]
letra [a-zA-Z_]
alfanumerico [a-zA-Z0-9_]
inteiro {digito}+
not_cientifica (e|E)[+-]?{inteiro}
especiais [,;:()\[\]{}+\-|*/<>=!&%#^.$?]

%x COMENTARIO

%%

"//".* 	{}
"/*" 		BEGIN(COMENTARIO);

"int"		{return TK_PR_INT;}
"float"	{return TK_PR_FLOAT;}
"bool"		{return TK_PR_BOOL;}
"char"		{return TK_PR_CHAR;}
"string"	{return TK_PR_STRING;}
"if"		{return TK_PR_IF;}
"then"		{return TK_PR_THEN;}
"else"		{return TK_PR_ELSE;}
"while"	{return TK_PR_WHILE;}
"do"		{return TK_PR_DO;}
"input"	{return TK_PR_INPUT;}
"output"	{return TK_PR_OUTPUT;}
"return"	{return TK_PR_RETURN;}
"const"	{return TK_PR_CONST;}
"static"	{return TK_PR_STATIC;}
"foreach"	{return TK_PR_FOREACH;}
"for"		{return TK_PR_FOR;}
"switch"	{return TK_PR_SWITCH;}
"case"		{return TK_PR_CASE;}
"break"	{return TK_PR_BREAK;}
"continue"	{return TK_PR_CONTINUE;}
"class"	{return TK_PR_CLASS;}
"private"	{return TK_PR_PRIVATE;}
"public"	{return TK_PR_PUBLIC;}
"protected"	{return TK_PR_PROTECTED;}
"end"		{return TK_PR_END;}
"default"	{return TK_PR_DEFAULT;}


"true"		{yylval.valor_lexico = setValor(get_line_number(), TK_LIT_TRUE, yytext);
		 	return TK_LIT_TRUE;}

"false"		{yylval.valor_lexico = setValor(get_line_number(), TK_LIT_FALSE, yytext);
		 	return TK_LIT_FALSE;}

{letra}{alfanumerico}* {
			yylval.valor_lexico = setValor(get_line_number(), TK_IDENTIFICADOR, yytext);
			lista_ptr = novoPTR(yylval.valor_lexico.valor.cad_char, lista_ptr);
			return TK_IDENTIFICADOR;}


[+-]?{inteiro}"."{digito}+{not_cientifica}? {
			yylval.valor_lexico = setValor(get_line_number(), TK_LIT_FLOAT, yytext);
			return TK_LIT_FLOAT;}


{inteiro} 	{yylval.valor_lexico = setValor(get_line_number(), TK_LIT_INT, yytext);
		 	return TK_LIT_INT;}


{especiais} 	{yylval.valor_lexico = setValor(get_line_number(), TK_CHAR_ESP, yytext);
				lista_ptr = novoPTR(yylval.valor_lexico.valor.cad_char, lista_ptr);
				return yytext[0];}

"<=" 		{yylval.valor_lexico = setValor(get_line_number(), TK_OC_LE, yytext);
			lista_ptr = novoPTR(yylval.valor_lexico.valor.cad_char, lista_ptr);
			return TK_OC_LE;}

">=" 		{yylval.valor_lexico = setValor(get_line_number(), TK_OC_GE, yytext);
			lista_ptr = novoPTR(yylval.valor_lexico.valor.cad_char, lista_ptr);
			return TK_OC_GE;}

"==" 		{yylval.valor_lexico = setValor(get_line_number(), TK_OC_EQ, yytext);
			lista_ptr = novoPTR(yylval.valor_lexico.valor.cad_char, lista_ptr);
			return TK_OC_EQ;}

"!=" 		{yylval.valor_lexico = setValor(get_line_number(), TK_OC_NE, yytext);
			lista_ptr = novoPTR(yylval.valor_lexico.valor.cad_char, lista_ptr);
			return TK_OC_NE;}

"&&" 		{yylval.valor_lexico = setValor(get_line_number(), TK_OC_AND, yytext);
			lista_ptr = novoPTR(yylval.valor_lexico.valor.cad_char, lista_ptr);
			return TK_OC_AND;}

"||" 		{yylval.valor_lexico = setValor(get_line_number(), TK_OC_OR, yytext);
			lista_ptr = novoPTR(yylval.valor_lexico.valor.cad_char, lista_ptr);
			return TK_OC_OR;}

">>" 		{yylval.valor_lexico = setValor(get_line_number(), TK_OC_SL, yytext);
			lista_ptr = novoPTR(yylval.valor_lexico.valor.cad_char, lista_ptr);
			return TK_OC_SR;}

"<<" 		{yylval.valor_lexico = setValor(get_line_number(), TK_OC_SR, yytext);
			lista_ptr = novoPTR(yylval.valor_lexico.valor.cad_char, lista_ptr);
			return TK_OC_SL;}

'[^']'		{yylval.valor_lexico = setValor(get_line_number(), TK_LIT_CHAR, yytext);
		 	return TK_LIT_CHAR;}

\"[^"]*\" 	{yylval.valor_lexico = setValor(get_line_number(), TK_LIT_STRING, yytext);
			lista_ptr = novoPTR(yylval.valor_lexico.valor.str, lista_ptr);
			return TK_LIT_STRING;}

"\n" 		{line_number++;}

[ \t]

. 		{return TOKEN_ERRO;}

<COMENTARIO>.
<COMENTARIO>"\n" {line_number++;}
<COMENTARIO>"*/" BEGIN(0);

%%

int get_line_number(void){return line_number;}
