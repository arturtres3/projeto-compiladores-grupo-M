#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tipos.h"


int bytes_por_tipo(enum_Tipo tipo){
    switch (tipo)
    {
    case TIPO_INT:
        return 4;
    case TIPO_FLOAT:
        return 8;
    case TIPO_BOOL:
        return 1;
    case TIPO_CHAR:
        return 1;
    case TIPO_STRING:
        return 1;
    default:
        return 0;
    }
}

char charDoTipo(enum_Tipo tipo){
    switch (tipo)
    {
    case TIPO_INT:
        return 'i';
    case TIPO_FLOAT:
        return 'f';
    case TIPO_BOOL:
        return 'b';
    case TIPO_CHAR:
        return 'c';
    case TIPO_STRING:
        return 's';
    default:
        return '#';
    }
}