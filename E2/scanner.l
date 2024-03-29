%{
    // Grupo M
	// Eduardo Henrique Ferreira do Nascimento (00260846)
	// Artur Tres do Amaral (00287682)
#include "parser.tab.h"
int get_line_number(void);
int line_number = 1;
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
"true"		{return TK_LIT_TRUE;}
"false"	{return TK_LIT_FALSE;}
{letra}{alfanumerico}* {return TK_IDENTIFICADOR;}
[+-]?{inteiro}"."{digito}+{not_cientifica}? {return TK_LIT_FLOAT;}
{inteiro} 	{return TK_LIT_INT;} 
{especiais} 	{return yytext[0];}
"<=" 		{return TK_OC_LE;}
">=" 		{return TK_OC_GE;}
"==" 		{return TK_OC_EQ;}
"!=" 		{return TK_OC_NE;}
"&&" 		{return TK_OC_AND;}
"||" 		{return TK_OC_OR;}
">>" 		{return TK_OC_SL;}
"<<" 		{return TK_OC_SR;}

'[^']'		{return TK_LIT_CHAR;}

\"[^"]*\" 	{return TK_LIT_STRING;}

"\n" 		{line_number++;} 

[ \t] 

. 		{return TOKEN_ERRO;}

<COMENTARIO>.
<COMENTARIO>"\n" {line_number++;} 
<COMENTARIO>"*/" BEGIN(0);

%%

int get_line_number(void){return line_number;}







