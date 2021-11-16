#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/tabela.h"
#include "include/ast.h"
#include "include/errors.h"
#include "include/ILOC.h"

void liberaListaVar(lista_var* lista){

    if(lista == NULL)
        return;

    liberaListaVar(lista->prox);

    free(lista->nome);
    free(lista);

}

void liberaParams(Parametro* lista_par){
    if(lista_par == NULL)
        return;

    liberaParams(lista_par->prox);

    free(lista_par);
    lista_par = NULL;
}

void liberaTabela(tabela_simbolos* tabela){

    if(tabela == NULL)
        return;

    liberaTabela(tabela->prox);

    if(tabela->lista_param != NULL)
        liberaParams(tabela->lista_param);

    free(tabela->label);
    free(tabela->chave);
    free(tabela);

}

void liberaPilha(pilha_tabela* pilha){
    if(pilha == NULL)
        return;

    liberaPilha(pilha->prox);
    liberaTabela(pilha->atual);

    free(pilha);
}


void printParams(Parametro* lista_par){
    if(lista_par == NULL)
        return;

    printf("%d", lista_par->tipo);

    printParams(lista_par->prox);
}

void printListaVar(lista_var* lista){
    if(lista == NULL)
        return;

    printf("%s, ", lista->nome);

    printListaVar(lista->prox);
}

void printTabela(tabela_simbolos* tabela){

    if(tabela == NULL)
        return;

    printf("%s(%d), ", tabela->chave, tabela->desloc);
    //printf("%s, ", tabela->chave);

    printTabela(tabela->prox);

}

void printPilha(pilha_tabela* pilha){
    if(pilha == NULL)
        return;

    printf("escopo: \n");
    printTabela(pilha->atual);
    printf("\n\n");

    printPilha(pilha->prox);

}


pilha_tabela* iniciaPilha(){
    pilha_tabela* novo = (pilha_tabela*)malloc(sizeof(pilha_tabela));

    novo->atual = (tabela_simbolos*)malloc(sizeof(tabela_simbolos));

    novo->atual = NULL;

    novo->prox = NULL;

    return novo;
}


lista_var* novoListaVar(lista_var* lista, char* nome, int tam, int linha, int vetor, valor_lexico valor, enum_Tipo tipo){

    lista_var* nova = (lista_var*)malloc(sizeof(lista_var));

    nova->nome = strdup(nome);
    nova->tamanho = tam;
    nova->linha = linha;
    nova->vetor = vetor;
    nova->valor = valor;
    nova->prox = NULL;
    nova->tipo = tipo;
    nova->desloc = 0;

    if(lista == NULL)  
        return nova;

    lista_var* aux = lista;
    while(aux->prox != NULL)
            aux = aux->prox;

    aux->prox = nova;

    return lista;
}


Parametro* novoParametro(Parametro *lista_par, enum_Tipo tipo){

    Parametro* novo = (Parametro*)malloc(sizeof(Parametro));

    novo->tipo = tipo;
    novo->prox = NULL;

    if(lista_par == NULL)
        return novo;

    Parametro* aux = lista_par;
    while(aux->prox != NULL)
            aux = aux->prox;

    aux->prox = novo;

    return lista_par;

}


tabela_simbolos* novaEntradaTabelaFunc(char* chave, int linha, enum_Tipo tipo, valor_lexico valor, int tamanho, Parametro *lista_par, char* label){
    tabela_simbolos* nova_tabela = (tabela_simbolos*)malloc(sizeof(tabela_simbolos));

    nova_tabela->chave = strdup(chave);
    nova_tabela->num_linha = linha;
    nova_tabela->natureza = FUNC;
    nova_tabela->tipo = tipo;
    nova_tabela->tamanho = tamanho * bytes_por_tipo(tipo);
    nova_tabela->valor = valor;
    nova_tabela->desloc = 0;
    nova_tabela->escopo = GLOBAL;
    nova_tabela->label = strdup(label);

    nova_tabela->lista_param = copiaParametros(lista_par);

    nova_tabela->prox = NULL;

    return  nova_tabela;
}


tabela_simbolos* novaEntradaTabela(char* chave, int linha, enum_Natureza natureza, enum_Tipo tipo, valor_lexico valor, int tamanho, int desloc, enum_Escopo escopo){

    tabela_simbolos* nova_tabela = (tabela_simbolos*)malloc(sizeof(tabela_simbolos));

    nova_tabela->chave = strdup(chave);
    nova_tabela->num_linha = linha;
    nova_tabela->natureza = natureza;
    nova_tabela->tipo = tipo;
    nova_tabela->tamanho = tamanho * bytes_por_tipo(tipo);
    nova_tabela->valor = valor;
    nova_tabela->desloc = desloc;
    nova_tabela->escopo = escopo;
    nova_tabela->label = NULL;

    nova_tabela->lista_param = NULL;

    nova_tabela->prox = NULL;

    return  nova_tabela;

}


tabela_simbolos* adicionaEntradaTabelaFunc(tabela_simbolos* escopo_atual, char* chave, int linha, enum_Tipo tipo, valor_lexico valor, int tamanho, Parametro *lista_par, char* label){

    tabela_simbolos* nova = novaEntradaTabelaFunc(chave, linha, tipo, valor, tamanho, lista_par, label);

    if(tipo == TIPO_STRING){
        mensagemErro(ERR_FUNCTION_STRING, linha, chave);
    }

    Parametro* verifica = lista_par;
    while(verifica != NULL){
        if(verifica->tipo == TIPO_STRING){
            mensagemErro(ERR_FUNCTION_STRING, linha, chave);
        }
        verifica = verifica->prox;
    }

    if(escopo_atual == NULL){
        return nova;

    }else{
        tabela_simbolos* aux = escopo_atual;

        while(aux->prox != NULL)
            aux = aux->prox;

        aux->prox = nova;

        return escopo_atual;
    }

}


tabela_simbolos* adicionaEntradaTabela(tabela_simbolos* escopo_atual, char* chave, int linha, enum_Natureza natureza, enum_Tipo tipo, valor_lexico valor, int tamanho, int desloc, enum_Escopo escopo){

    tabela_simbolos* foi_declarado = procuraTabela(chave, escopo_atual);

    if(foi_declarado != NULL){
        if(natureza == LIT){
            return escopo_atual;
        }
        mensagemErro(ERR_DECLARED, linha, foi_declarado);
    }

    tabela_simbolos* nova = novaEntradaTabela(chave, linha, natureza, tipo, valor, tamanho, desloc, escopo);

    if(escopo_atual == NULL){
        return nova;

    }else{
        tabela_simbolos* aux = escopo_atual;

        while(aux->prox != NULL)
            aux = aux->prox;

        aux->prox = nova;

        return escopo_atual;
    }

}


tabela_simbolos* adicionaListaVar(tabela_simbolos* escopo_atual, lista_var* variaveis, enum_Tipo tipo, enum_Escopo escopo, int main){
    int desloc;
    while(variaveis != NULL){
        enum_Natureza natur;
        if(variaveis->vetor == 1){
            natur = VETOR;
            if(tipo == TIPO_STRING){
                mensagemErro(ERR_STRING_VECTOR, variaveis->linha, NULL);
            }
        }else{
            natur =  VAR;
        }
        
        if(escopo == GLOBAL){
            desloc = deslocGlobal();
        }else{
            desloc = deslocLocal(0);
            if(main != 1){
                desloc = desloc + 16;
            }
        }

        if(variaveis->tipo == TIPO_NA){
            escopo_atual = adicionaEntradaTabela(escopo_atual, variaveis->nome, variaveis->linha, natur, tipo, variaveis->valor, variaveis->tamanho, desloc, escopo);
        }else{
            escopo_atual = adicionaEntradaTabela(escopo_atual, variaveis->nome, variaveis->linha, natur, variaveis->tipo, variaveis->valor, variaveis->tamanho, desloc, escopo);
        }
        variaveis = variaveis->prox;
        
    }

    return escopo_atual;

}


pilha_tabela* novoEscopo(pilha_tabela* topo){

    pilha_tabela* novo = (pilha_tabela*)malloc(sizeof(pilha_tabela));

    novo->atual = (tabela_simbolos*)malloc(sizeof(tabela_simbolos));
    novo->atual = NULL;
    novo->prox = topo;

    return novo;

}


pilha_tabela* fechaEscopo(pilha_tabela* pilha){

    liberaTabela(pilha->atual);

    pilha_tabela* aux = pilha->prox;

    free(pilha);

    return aux;

}


tabela_simbolos* procuraTabela(char* chave, tabela_simbolos* tabela){

    while(tabela != NULL){
        if(strcmp(tabela->chave, chave) == 0)
            return tabela;
        tabela = tabela->prox;
    }

    return NULL;

}


tabela_simbolos* encontraSimbolo(char* chave, pilha_tabela* pilha){

    if(pilha == NULL)
        return NULL;

    tabela_simbolos* resultado = procuraTabela(chave, pilha->atual);

    if(resultado != NULL)
        return resultado;

    encontraSimbolo(chave, pilha->prox);

}


int comparaParams(Parametro* lista1, Parametro* lista2){ 
    
    if(lista1 == NULL && lista2 == NULL){ // OK
        return 0;
    }

    if(lista1 == NULL && lista2 != NULL){ // Excess Args
        return 1;
    }

    if(lista1 != NULL && lista2 == NULL){ // Missing Args
        return 2;
    }

    if(lista1->tipo == lista2->tipo){
        return comparaParams(lista1->prox, lista2->prox);
    }else{
        return 3; // Wrong type args
    }
}


void confereChamadaFunc(pilha_tabela* pilha, char* chave_func, Parametro* argumentos, int linha){
    
    tabela_simbolos* simbolo_func = encontraSimbolo(chave_func, pilha);

    recuperaTipo(pilha, chave_func, linha); // se nao estiver declarado cai em erro aqui

    Parametro* verifica = argumentos;
    while(verifica != NULL){
        if(verifica->tipo == TIPO_STRING){
            mensagemErro(ERR_FUNCTION_STRING, linha, chave_func);
        }
        verifica = verifica->prox;
    }
    
    int result = comparaParams(simbolo_func->lista_param, argumentos);

    switch(result){
        case 0:
            return;
            break;
        case 1: 
            mensagemErro(ERR_EXCESS_ARGS, linha, NULL);
            break;
        case 2:
            mensagemErro(ERR_MISSING_ARGS, linha, NULL);
            break;
        case 3:
            mensagemErro(ERR_WRONG_TYPE_ARGS, linha, NULL);
            break;
        default:
            printf("erro");
            
    }


}


Parametro* copiaParametros(Parametro* lista_par){
    Parametro* nova_lista = NULL;
    Parametro* aux = lista_par;

    while(aux != NULL){
        nova_lista = novoParametro(nova_lista, aux->tipo);
        aux = aux->prox;
    }

    return nova_lista;
}


void mensagemErro(int erro, int linha, void* ref1){

    printf("[ERRO] Erro semantico encontrado na linha: %d\n", linha);

    switch (erro){
        case ERR_UNDECLARED:
			printf("[ERRO] Identificador \"%s\" nao declarada.\n", ((char *)ref1) );  
            break;
        case ERR_DECLARED:
            printf("[ERRO] Variavel ja declarada.\n");
            printf("[ERRO] Primeira declaracao na linha: %d\n", ((tabela_simbolos *)ref1)->num_linha);
            break;
        case ERR_VARIABLE:
            printf("[ERRO] Variavel \"%s\" nao pode ser usada como vetor ou funcao.\n", ((char *)ref1) );
            break;
        case ERR_VECTOR:
            printf("[ERRO] Vetor \"%s\" nao pode ser usado como variavel ou funcao.\n", ((char *)ref1) );
            break;
        case ERR_FUNCTION:
            printf("[ERRO] Funcao \"%s\" nao pode ser usada como variavel ou vetor.\n", ((char *)ref1) );
            break;
        case ERR_WRONG_TYPE:
			printf("[ERRO] Atribuicao de tipo incompativel.\n");
            break;
        case ERR_STRING_TO_X:
            printf("[ERRO] Strings nao podem ser convertidas implicitamente.\n");
            break;
        case ERR_CHAR_TO_X:
            printf("[ERRO] Caracteres nao podem ser convertidos implicitamente.\n");
            break;
        case ERR_MISSING_ARGS:
            printf("[ERRO] Chamada de funcao com numero insuficiente de argumentos.\n");
            break;
        case ERR_EXCESS_ARGS:
            printf("[ERRO] Chamada de funcao com numero excessivo de argumentos.\n");
            break;
        case ERR_WRONG_TYPE_ARGS:
            printf("[ERRO] Argumento com tipo incompativel na chamada de funcao.\n");
            break;
        case ERR_WRONG_PAR_INPUT:
            printf("[ERRO] Comando input aceita apenas identificadores dos tipos int e float.\n");
            break;
        case ERR_WRONG_PAR_OUTPUT:
            printf("[ERRO] Comando output aceita apenas identificadores ou literais dos tipos int e float.\n");
            break;
        case ERR_WRONG_PAR_RETURN:
            printf("[ERRO] Retorno incompativel com o tipo declarado da funcao.\n");
            break;
        case ERR_WRONG_PAR_SHIFT:
            printf("[ERRO] Comando de shift nao aceita numeros maiores que 16.\n");
            break;
        case ERR_FUNCTION_STRING:
            printf("[ERRO] Tipo string nao pode ser definido como retorno ou parametro de funcao.\n Funcao nao pode ser chamada com argumento tipo string.\n");
            printf("[ERRO] Encontrado na funcao \"%s\".\n", ((char *)ref1) );
            break;
        case ERR_STRING_VECTOR:
            printf("[ERRO] Vetores nao podem ser declarados com tipo string.\n");
            break;
        case ERR_STRING_MAX:
            printf("[ERRO] String excede o tamanho definido.\n");
            break;
        default:
            
            break;
        }

        printf("Terminando execucao com codigo de status: %d \n\n", erro);

        exit(erro);

}



enum_Tipo recuperaTipo(pilha_tabela* pilha, char* chave, int linha){

    tabela_simbolos* entrada_tabela = encontraSimbolo(chave, pilha);

    if(entrada_tabela != NULL){
        return entrada_tabela->tipo;
    }else{
        mensagemErro(ERR_UNDECLARED, linha, chave);
    }
}


void verificaInputOutput(enum_Tipo tipo, char comando, int linha){
    if(tipo == TIPO_INT || tipo == TIPO_FLOAT){
        return;
    }else{
        if(comando == 'i')
            mensagemErro(ERR_WRONG_PAR_INPUT, linha, NULL);
        
        if(comando == 'o')
            mensagemErro(ERR_WRONG_PAR_OUTPUT, linha, NULL);
    }
}


tabela_simbolos* encontraUltimaFuncao(pilha_tabela* pilha){

    tabela_simbolos* ultima_func = NULL; //return deve estar associado a ultima funcao declarada

    while(pilha != NULL){
        tabela_simbolos* atual = pilha->atual;
        if(atual != NULL){
            while(atual->prox != NULL){
                atual = atual->prox;
            }
        
            if(atual->natureza == FUNC){
                ultima_func = atual;
                return ultima_func;
            }
        }
        pilha = pilha->prox;
    }

    return ultima_func;
}


void verificaReturn(pilha_tabela* pilha, enum_Tipo tipo, int linha){
    tabela_simbolos* ultima_func = encontraUltimaFuncao(pilha);

    if(ultima_func != NULL){
        if(ultima_func->tipo == tipo){
            return;
        }
    }

    mensagemErro(ERR_WRONG_PAR_RETURN, linha, NULL);

}


void confereAtribuicao(enum_Tipo tipo_recebe, enum_Tipo tipo_recebido, int linha){

    if(tipo_recebe != tipo_recebido){
        if(tipo_recebido == TIPO_CHAR){
            mensagemErro(ERR_CHAR_TO_X, linha, NULL);
        }
        if(tipo_recebido == TIPO_STRING){
            mensagemErro(ERR_STRING_TO_X, linha, NULL);
        }
        if(tipo_recebe == TIPO_CHAR || tipo_recebe == TIPO_STRING){
            mensagemErro(ERR_WRONG_TYPE, linha, NULL);
        }  
    }

    // se nenhum dos valores e char ou string entao os tipos sao alguma
    // combinacao de int, float e bool. Conversao implicita e permitida
    // para todas essas combinacoes.

    return;
}


void confereNatureza(pilha_tabela* pilha, char* chave, enum_Natureza natureza, int linha){
    tabela_simbolos* simbolo = encontraSimbolo(chave, pilha);
    
    if(simbolo == NULL)
        recuperaTipo(pilha, chave, linha);
        
    switch(simbolo->natureza){
        case VETOR:
            if(natureza != VETOR)
                mensagemErro(ERR_VECTOR, linha, chave);
            break;
        case VAR:
            if(natureza != VAR)
                mensagemErro(ERR_VARIABLE, linha, chave);
            break;
        case FUNC:
            if(natureza != FUNC)
                mensagemErro(ERR_FUNCTION, linha, chave);
            break;
        default:
            printf("erro");
            
    }
        
}


void confereInicializacao(pilha_tabela* pilha, void* nodo_in, enum_Tipo tipo, int linha){

    AST* nodo = nodo_in; // para nao incluir ast.h em tabela.h

    while(nodo != NULL){
        if(strcmp(nodo->label, "<=") == 0){
            alteraTipoNodo(nodo, tipo);
            alteraTipoNodo(nodo->prim_filho, tipo);

            enum_Tipo tipo_recebido = nodo->prim_filho->prim_irmao->tipo;

            if(tipo != tipo_recebido){
                if(tipo_recebido == TIPO_CHAR){
                    mensagemErro(ERR_CHAR_TO_X, linha, NULL);
                }
                if(tipo_recebido == TIPO_STRING){
                    mensagemErro(ERR_STRING_TO_X, linha, NULL);
                }
                if(tipo == TIPO_CHAR || tipo == TIPO_STRING){
                    mensagemErro(ERR_WRONG_TYPE, linha, NULL);
                }  
                // se nenhum dos valores for char ou string entao os tipos sao alguma
                // combinacao de int, float e bool. Conversao implicita eh permitida
                // para todas essas combinacoes.
            } 

            // o primeiro filho eh a var sendo declarada, prim_irmao eh o valor recebido e o prox irmao eh outra inicializacao ou nulo
            //printf("nodo: %s, filho: %s, irmao: %s \n", nodo->label, nodo->prim_filho->label, nodo->prim_filho->prim_irmao->label);
            nodo = nodo->prim_filho->prim_irmao->prim_irmao;
                 

        }else{
            return; // sai do loop se algum absurdo ocorrer
        }
    }

}


enum_Tipo inferencia_tipo(enum_Tipo tipo1, enum_Tipo tipo2, int linha){

    if(tipo1 == tipo2)
        return tipo1;

    if(tipo1 == TIPO_STRING || tipo2 == TIPO_STRING)
        mensagemErro(ERR_STRING_TO_X, linha, NULL);

    if(tipo1 == TIPO_CHAR || tipo2 == TIPO_CHAR)
        mensagemErro(ERR_CHAR_TO_X, linha, NULL);

    if(tipo1 == TIPO_FLOAT || tipo2 == TIPO_FLOAT)
        return TIPO_FLOAT;

    
    // se chegou aqui eh alguma combinacao de int e bool, que vira int
    return TIPO_INT;

}


void confereShift(int valor, int linha){

    if(valor > 16){
        mensagemErro(ERR_WRONG_PAR_SHIFT, linha, NULL);
    }
}



int recuperaDesloc(char* chave, pilha_tabela* pilha){
    tabela_simbolos* saida = encontraSimbolo(chave, pilha);

    return saida->desloc;
}


char* recuperaEscopo(LISTA_PTR** lista_ptr,char* chave, pilha_tabela* pilha){
    tabela_simbolos* saida = encontraSimbolo(chave, pilha);
    char* reg = NULL;
    char g[] = "rbss";
    char l[] = "rfp";

    if(saida->escopo == GLOBAL){
        reg = strdup(g);
    }else{
        reg = strdup(l);
    }
    novoPTR(reg, lista_ptr);
    

    return reg;
}

enum_Escopo retornaEscopo(char* chave, pilha_tabela* pilha){
    tabela_simbolos* saida = encontraSimbolo(chave, pilha);

    if(saida != NULL)
        return saida->escopo;

}


int quantidadeVarLocais(tabela_simbolos* atual){
    int num = 0;
    while(atual != NULL){
        if(atual->escopo == LOCAL){
            num++;
        }
        atual = atual->prox;
    }
    //printf("# %d #", num);
    return num;
}


int contaParams(Parametro* lista){
    int num = 0;
    Parametro* aux = lista;
    while(aux != NULL){
        num++;
        aux = aux->prox;
    }
    //printf("# %d #", num);
    return num;

}


void inverteListaVar(lista_var** lista){
    lista_var* anterior = NULL;
    lista_var* atual = *lista;
    lista_var* proximo = *lista;


    while(atual != NULL){
        proximo = atual->prox;
        atual->prox = anterior;

        anterior = atual;
        atual = proximo;
    }

    *lista = anterior;

}