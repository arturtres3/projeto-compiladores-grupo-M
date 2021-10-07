#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct NODE {
  char *a;
  struct NODE *prim_filho;
  struct NODE *prim_irmao;
} NODE;

NODE* novoNodo(char *data){
    NODE* node = (NODE*)malloc(sizeof(NODE));
    //node->a = malloc(strlen(data) * sizeof(char));
    //strcpy(node->a, data);
    node->a = strdup(data);
    node->prim_filho = NULL;
    node->prim_irmao = NULL;

    return node;
}

void printInorder(NODE* node){
    if (node == NULL)
        return;

    printInorder(node->prim_filho);

    printf("%c ", node->a);

    printInorder(node->prim_irmao);
}

void printPreorder(NODE* node){
    if (node == NULL)
        return;

    printf("%s ", node->a);

    printPreorder(node->prim_filho);

    printPreorder(node->prim_irmao);
}

NODE* encontraNodo(NODE* node, char *key){
    if (node == NULL)
        return NULL;

    if(strcmp(node->a, key) == 0)
        return node;

    NODE* left = encontraNodo(node->prim_filho, key);
    if(left != NULL)
        return left;

    NODE* right = encontraNodo(node->prim_irmao, key);
    return right;
}

int ehPai(NODE* raiz, NODE* node){ //retorna 1 se raiz eh pai de node, 0 senao
    if (node == NULL || raiz == NULL)
        return 0;

    NODE* percorre = raiz->prim_filho;
    while(percorre != NULL && percorre->prim_irmao != NULL){
        if(percorre == node)
            return 1;
        percorre = percorre->prim_irmao;
    }

    return 0;
}

NODE* encontraPai(NODE* raiz, NODE* node){//retona ponteiro para pai do node se estiver em raiz, senao retorna nulo
    if (raiz == NULL)
        return NULL;

    if(ehPai(raiz, node)){
        return raiz;
    }

    NODE* left = encontraPai(raiz->prim_filho, node);
    if(left != NULL)
        return left;

    NODE* right = encontraPai(raiz->prim_irmao, node);
    return right;


}

void removeNodo(NODE* raiz, NODE* node){
    NODE* pai = encontraPai(raiz, node);

    if(pai == NULL)
        return;

    NODE* temp = pai->prim_filho;
    NODE* irmao_anterior = NULL;
    if(pai->prim_filho == node){
        pai->prim_filho = temp->prim_irmao;
        //libera(temp);
    }else{
        while(temp != node){
            temp = temp->prim_irmao;
            irmao_anterior = temp;
        }
        irmao_anterior->prim_irmao = temp->prim_irmao;
        //libera(temp);
    }

}

void alteraNodo(NODE* node, char* novo_valor){
    //free(node-> a);
    node->a = strdup(novo_valor);
}

void printSiblings(NODE* node){

    if (node == NULL)
        return;

    printf("%c ", node->a);

    printSiblings(node->prim_irmao);
}

void printSiblings2(NODE* node, NODE* pai){

    if (node == NULL)
        return;

    printf("%p, %p\n", pai, node);

    printSiblings2(node->prim_irmao, pai);
}

void printEnderecos(NODE* node){

    if (node == NULL)
        return;

    printSiblings2(node->prim_filho, node);

    printEnderecos(node->prim_filho);
    printEnderecos(node->prim_irmao);
}

void printValEnderecos(NODE* node){

    if (node == NULL)
        return;

    printf("%p = [label=\"%s\"]\n", node, node->a);

    printValEnderecos(node->prim_filho);

    printValEnderecos(node->prim_irmao);

}

void adicionaFilhos(NODE* pai, NODE* lista_filhos[], int num_filhos){
    int i = 0;

    if(pai == NULL || lista_filhos == NULL)
        return;

    for(i = 0; i < num_filhos-1; i++){
        lista_filhos[i]->prim_irmao = lista_filhos[i+1];
    }
    //printSiblings(lista_filhos[0]);

    pai->prim_filho = lista_filhos[0];

}

NODE* cria_e_adiciona(char *data, NODE** lista_filhos, int num_filhos){

    NODE* node = novoNodo(data);

    adicionaFilhos(node, lista_filhos, num_filhos);

    return node;
}

void libera(NODE* node){
    if (node == NULL)
        return;

    libera(node->prim_filho);
    libera(node->prim_irmao);

    free(node->a);
    free(node);
    //printf("free node\n");
}

char* float_to_string(float valor){
    int length = snprintf( NULL, 0, "%f", valor);
    char* str = malloc( length + 1 );
    snprintf(str, length + 1, "%f", valor);

    return str;
}

char* int_to_string(int valor){
    int length = snprintf( NULL, 0, "%d", valor);
    char* str = malloc( length + 1 );
    snprintf(str, length + 1, "%d", valor);

    return str;
}
//char str[2] = {teste_char, '\0'};

void appendFilho(NODE* pai, NODE* novo_filho){
    NODE* filho = pai->prim_filho;

    while(filho->prim_irmao != NULL){
        filho = filho->prim_irmao;
    }

    filho->prim_irmao = novo_filho;

}

int main() {

    //NODE* raiz = testeCriarArvoreApartirDeEntrada();
    // ######     CHAMAR libera(raiz)    ######

    NODE nodoA = {.a = "Aabc"};
    NODE nodoB = {.a = "Babc"};
    NODE nodoC = {.a = "Cabc"};

    float teste_float = 2.5;
    int teste_int = 250;
    char teste_char = 'T';
    char teste_str[] = "nome_func";
    char* str = NULL;

    //char* str = float_to_string(teste_float); free(str);
    //char* str = int_to_string(teste_int);
    //char str[2] = {teste_char, '\0'};

    //int a = strlen(teste_str)+6;
    str = realloc(str, (strlen(teste_str)+6) * sizeof(char));
    strcpy(str, "call ");
    strcat(str, teste_str);
    printf("%d\n", strlen(teste_str)+6);


    NODE nodoD = {.a = "Dabc"};


    NODE nodo5 = {.a = str};
    NODE nodo4 = {.a = "4abc"};
    NODE nodo3 = {.a = "3abc"};
    NODE nodo2 = {.a = "2abc"};
    NODE nodo1 = {.a = "1abc"};
    NODE nodo0 = {.a = "0abc"};

    NODE** lista = malloc(2 * sizeof(NODE));
    lista[0] = &nodo1;
    lista[1] = &nodo2;
    adicionaFilhos(&nodo0, lista, 2); //no parser se deve criar o nodo pai na hora

    lista = realloc(lista, 1 * sizeof(NODE));
    lista[0] = &nodo3;
    adicionaFilhos(&nodo1, lista, 1);

    lista = realloc(lista, 2 * sizeof(NODE));
    lista[0] = &nodo4;
    lista[1] = &nodo5;
    adicionaFilhos(&nodo2, lista, 2);

    lista = realloc(lista, 4 * sizeof(NODE));
    lista[0] = &nodoA;
    lista[1] = &nodoB;
    lista[2] = &nodoC;
    lista[3] = &nodoD;

    NODE* teste_criar;
    teste_criar = cria_e_adiciona("raiz", lista, 4);

    printf("print nodo criado com cria_e_adiciona()\n");
    printPreorder(teste_criar);

    NODE nodoE = {.a = "Eabc(append)"};

    appendFilho(teste_criar, &nodoE);

    printf("\n\nprint teste_criar dps do append\n");
    printPreorder(teste_criar);

    char outra_str[] = "novo_valor";
    alteraNodo(&nodoB, outra_str);

    printf("\n\nprint teste_criar dps da alteracao\n");
    printPreorder(teste_criar);

    int resposta = ehPai(teste_criar, &nodoB);
    printf("\n\nEh pai? %d\n", resposta);

    NODE* pai_encontrado = encontraPai(&nodo0, &nodo4);
    printf("\n\nPai encontrado do nodo4: %s\n", pai_encontrado->a);

    printf("\n\nprint nodo0 antes de remover nodo4\n");
    printPreorder(&nodo0);

    removeNodo(&nodo0, &nodo4);
    printf("\n\nprint nodo0 dps de remover nodo4\n");
    printPreorder(&nodo0);


    free(lista);

    free(teste_criar);

    printf("\n\nprint enderecos nodo0\n");
    printEnderecos(&nodo0);

    printf("\n\nValores dos Enderecos\n");
    printValEnderecos(&nodo0);


    free(str);

    /*printf("\n\nPreorder\n");
    printPreorder(&nodo0);

    printf("\n\nInorder\n");
    printInorder(&nodo0);*/

    //libera(raiz);


    return 0;
}

/*
NODE* testeCriarArvoreApartirDeEntrada(){

    NODE* raiz = novoNodo("$");
    NODE* ultimo;
    char teste[50];
    int i;

    printf("Entrada: ");
    fgets(teste, sizeof teste, stdin);

    raiz->prim_filho = novoNodo(teste[0]);
    ultimo = raiz->prim_filho;

    for(i = 1; i < strlen(teste); i++){
        if(teste[i] == ' '){
            ultimo->prim_filho =  novoNodo('#');
            ultimo = ultimo->prim_filho;
        }else{
            ultimo->prim_irmao =  novoNodo(teste[i]);
            ultimo = ultimo->prim_irmao;
        }
    }

    return raiz;
}
*/
