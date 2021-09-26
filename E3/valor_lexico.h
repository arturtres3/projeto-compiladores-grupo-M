#ifndef VALOR_HEADER
#define VALOR_HEADER
#define TK_CHAR_ESP        304 //novo token para sinalizar char especial

//novos tipos
#define TIPO_CHAR_ESP      300
#define TIPO_OP_COMP       301
#define TIPO_IDENT         302
#define TIPO_LIT           303

union Valores{
   	int i;
  	int b;                  //boolean
  	float f;
  	char c;
  	char* str;
   	char* cad_char;         //cadeia de caracteres quando for char_especial ou identificador
};

typedef struct valor_lexico{
	int num_linha; 		    //linha
	int tipo; 	   	        //novo código com os tipos
	union Valores valor;	//valor
	int tipo_literal;       //se for literal guarda o token

}valor_lexico;


valor_lexico setValor(int linha, int token, char* yytext);

void limpaString(char* str);

char* float_to_string(float valor);

char* int_to_string(int valor);

char* char_to_string(char valor);

char* label_chamada(char* nome_func);

void printValorTESTE(valor_lexico valor);

#endif
