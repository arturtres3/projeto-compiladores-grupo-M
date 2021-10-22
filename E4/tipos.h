#ifndef TIPOS_HEADER
#define TIPOS_HEADER

typedef enum enum_Natureza
{
    IDENT,
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
    TIPO_NA,     // Nao recebe tipo, controle de fulxo, etc
} enum_Tipo;

enum_Tipo inferencia_tipo(enum_Tipo tipo1, enum_Tipo tipo2);

int bytes_por_tipo(enum_Tipo tipo);

#endif