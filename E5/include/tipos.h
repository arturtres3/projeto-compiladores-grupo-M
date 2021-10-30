#ifndef TIPOS_HEADER
#define TIPOS_HEADER

typedef enum enum_Natureza
{
    IDENT,
    VAR, 
    LIT,
    FUNC,
    VETOR,
} enum_Natureza;


typedef enum enum_Tipo{
    TIPO_INT,
    TIPO_FLOAT,
    TIPO_BOOL,
    TIPO_CHAR,
    TIPO_STRING,
    TIPO_NA,     // Nao recebe tipo, controle de fluxo, etc
} enum_Tipo;


int bytes_por_tipo(enum_Tipo tipo);

char charDoTipo(enum_Tipo tipo);

char* float_to_string(float valor);

char* int_to_string(int valor);

char* char_to_string(char valor);

#endif