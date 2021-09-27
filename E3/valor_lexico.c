#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "valor_lexico.h"
#include "parser.tab.h"

LISTA_PTR* novoPTR(char* ptr_string, LISTA_PTR* lista){
	LISTA_PTR* novo = (LISTA_PTR*)malloc(sizeof(LISTA_PTR));
	novo->ptr = ptr_string;
	novo->prox = lista;

	return novo;
}

void liberaPTR(LISTA_PTR* lista){
	if(lista == NULL)
		return;
	liberaPTR(lista->prox);
	free(lista->ptr);
	free(lista);
}

valor_lexico setValor(int linha, int token, char* yytext){
	valor_lexico novo;
	union Valores valor;
	char* temp = NULL;
	int length = strlen(yytext);

	novo.num_linha = linha;

	switch(token)
	{
        case TK_CHAR_ESP:
            valor.cad_char = strdup(yytext);
			novo.tipo = TIPO_CHAR_ESP;
			break;
		case TK_OC_LE :
		case TK_OC_GE :
		case TK_OC_EQ :
		case TK_OC_NE :
		case TK_OC_AND:
		case TK_OC_OR :
		case TK_OC_SL :
		case TK_OC_SR :
			valor.cad_char = strdup(yytext);
			novo.tipo = TIPO_OP_COMP;
			break;
		case TK_IDENTIFICADOR :
			valor.cad_char = strdup(yytext);
			novo.tipo = TIPO_IDENT;
			break;
		case TK_LIT_INT:
			valor.i = atoi(yytext);
			novo.tipo = TIPO_LIT;
			break;
		case TK_LIT_FLOAT:
			valor.f = atof(yytext);
			novo.tipo = TIPO_LIT;
			break;
		case TK_LIT_CHAR:
			valor.c = yytext[1];
			novo.tipo = TIPO_LIT;
			break;
		case TK_LIT_STRING:
			valor.str = strdup(yytext);
			limpaString(valor.str, strlen(valor.str));
			novo.tipo = TIPO_LIT;
			break;
		case TK_LIT_TRUE:
			valor.b = 1;
			novo.tipo = TIPO_LIT;
			break;
        case TK_LIT_FALSE:
			valor.b = 0;
			novo.tipo = TIPO_LIT;
			break;
	}

	if(novo.tipo == TIPO_LIT){
        novo.tipo_literal = token;
	}else{
        novo.tipo_literal = 0;
	}

    novo.valor = valor;
	return novo;

}

void limpaString(char* str, int length){ //tira as aspas do literal string
	int i = 1;

	for(i = 1; i < length-1; i++){
		str[i-1] = str[i];
	}

	str[length-2] = '\0';
}

void printValorTESTE(valor_lexico valor){

    printf("num_linha: %d\n", valor.num_linha);

    printf("tipo: %d\n", valor.tipo);

    if(valor.tipo != TIPO_LIT){
        printf("valor: %s", valor.valor.cad_char);
    }else{
        switch(valor.tipo_literal){
            case TK_LIT_STRING:
                printf("valor: %s", valor.valor.str);
                break;
            case TK_LIT_INT:
                printf("valor: %d", valor.valor.i);
                break;
            case TK_LIT_CHAR:
                printf("valor: %c", valor.valor.c);
                break;
            case TK_LIT_FLOAT:
                printf("valor: %f", valor.valor.f);
                break;
            case TK_LIT_FALSE:
            case TK_LIT_TRUE:
                printf("valor: %d", valor.valor.b);
                break;
            default:
                printf("ERRO");
        }
        printf("\n");
    }
}

//PRECISA CHAMAR FREE
char* float_to_string(float valor){
    int length = snprintf( NULL, 0, "%f", valor);
    char* str = malloc( length + 1 );
    snprintf(str, length + 1, "%f", valor);

    return str;
}

//PRECISA CHAMAR FREE
char* int_to_string(int valor){
	int length = snprintf( NULL, 0, "%d", valor);
    char* str = malloc( length + 1 );
    snprintf(str, length + 1, "%d", valor);

    return str;
}

//PRECISA CHAMAR FREE
char* char_to_string(char valor){
	char* str = malloc(2 * sizeof(char));
	str[0] = valor;
	str[1] = '\0';

	return str;
}

//PRECISA CHAMAR FREE
char* label_chamada(char* nome_func){
	int length = strlen(nome_func) + 6;
    char* str = malloc(length * sizeof(char));
	strcpy(str, "call ");
	strcat(str, nome_func);

	return str;
}
