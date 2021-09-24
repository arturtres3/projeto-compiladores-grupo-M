#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct NODE {
  char a;
  struct NODE *prim_filho;
  struct NODE *prim_irmao;
} NODE;

NODE* novoNodo(char data){
    NODE* node = (NODE*)malloc(sizeof(NODE));
    node->a = data;
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

    printf("%c ", node->a);

    printPreorder(node->prim_filho);

    printPreorder(node->prim_irmao);
}

NODE* findNode(NODE* node, char key){
    if (node == NULL)
        return NULL;

    if(node->a == key)
        return node;

    NODE* left = findNode(node->prim_filho, key);
    if(left != NULL)
        return left;

    NODE* right = findNode(node->prim_irmao, key);
    return right;

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

    printf("%p = [label=\"%c\"]\n", node, node->a);

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

NODE* cria_e_adiciona(char data, NODE** lista_filhos, int num_filhos){

    NODE* node = novoNodo(data);

    adicionaFilhos(node, lista_filhos, num_filhos);

    return node;
}



void libera(NODE* node){
    if (node == NULL)
        return;

    libera(node->prim_filho);
    libera(node->prim_irmao);

    free(node);
    //printf("free node\n");
}

NODE* testeCriarArvoreApartirDeEntrada(){

    NODE* raiz = novoNodo('$');
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


int main() {

    //NODE* raiz = testeCriarArvoreApartirDeEntrada();

    // ######     CHAMAR libera(raiz)    ######

    /*NODE* nodo = findNode(raiz, 'd');
    if(nodo != NULL){
        printf("Irmaos do nodo d:");
        printSiblings(nodo);
        printf("\n");
    }*/

    //printf("\nfilhos do nodo $: ");
    //printSiblings(raiz->prim_filho);
    //printf("\n");
    NODE nodoA = {.a = 'A'};
    NODE nodoB = {.a = 'B'};
    NODE nodoC = {.a = 'C'};
    NODE nodoD = {.a = 'D'};


    NODE nodo5 = {.a = '5'};
    NODE nodo4 = {.a = '4'};
    NODE nodo3 = {.a = '3'};
    NODE nodo2 = {.a = '2'};
    NODE nodo1 = {.a = '1'};
    NODE nodo0 = {.a = '0'};

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
    teste_criar = cria_e_adiciona('?', lista, 4);

    printf("print nodo criado com cria_e_adiciona()\n");
    printPreorder(teste_criar);

    free(lista);
    free(teste_criar);

    printf("\n\nprint enderecos nodo0\n");
    printEnderecos(&nodo0);

    printf("\n\nValores dos Enderecos\n");
    printValEnderecos(&nodo0);

    printf("\n\nPreorder\n");
    printPreorder(&nodo0);

    printf("\n\nInorder\n");
    printInorder(&nodo0);

    //libera(raiz);


    return 0;
}
