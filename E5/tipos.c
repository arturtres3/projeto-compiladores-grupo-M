#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/tipos.h"


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


void novoPTR(char* ptr_string, LISTA_PTR** lista){
    LISTA_PTR* novo = (LISTA_PTR*)malloc(sizeof(LISTA_PTR));
	novo->ptr = ptr_string;
	novo->prox = NULL;

	LISTA_PTR* aux = *lista;
	if(aux != NULL){
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = novo;
	}else{
        *lista = novo;
	}

}


LISTA_PTR* copiaLista(LISTA_PTR* lista){
    
    LISTA_PTR* novo = NULL;

    while(lista != NULL){
        novoPTR(lista->ptr, &novo);
        lista = lista->prox;
    }

    return novo;

}


LISTA_PTR* concatLista(LISTA_PTR* l1, LISTA_PTR* l2){
    LISTA_PTR* aux = l1;

    if(aux != NULL){
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = l2;
        return l1;

    }else{
        return l2;
    }

}


void liberaPTR(LISTA_PTR* lista){
	if(lista == NULL)
		return;
	liberaPTR(lista->prox);
    //printf("%s, ", lista->ptr);
	free(lista->ptr);
	free(lista);
}


void liberaStructPTR(LISTA_PTR* lista){
	if(lista == NULL)
		return;
	liberaPTR(lista->prox);

	free(lista);
}


char* copiaStr(char* str){ //strdup quebra se passar nulo
    char* saida;

    if(str != NULL){
        saida = strdup(str);
    }else{
        saida = NULL;
    }


    return saida;
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