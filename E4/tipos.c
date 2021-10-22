#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tipos.h"

enum_Tipo inferencia_tipo(enum_Tipo tipo1, enum_Tipo tipo2){

    if(tipo1 == TIPO_STRING || tipo2 == TIPO_STRING)
        return TIPO_NA; // CHAMA ERRO

    if(tipo1 == TIPO_CHAR || tipo2 == TIPO_CHAR)
        return TIPO_NA; // CHAMA ERRO

    if(tipo1 == tipo2)
        return tipo1;

    if(tipo1 == TIPO_FLOAT || tipo2 == TIPO_FLOAT)
        return TIPO_FLOAT;

    return TIPO_INT;

}

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