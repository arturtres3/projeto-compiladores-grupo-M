#ifndef TIPOS_HEADER
#define TIPOS_HEADER

// Naturaza do simbolo
typedef enum enum_Natureza
{
    IDENT,
    VAR, 
    LIT,
    FUNC,
    VETOR,
} enum_Natureza;

// Tipos semanticos da linguagem
typedef enum enum_Tipo{
    TIPO_INT,
    TIPO_FLOAT,
    TIPO_BOOL,
    TIPO_CHAR,
    TIPO_STRING,
    TIPO_NA,     // Nao recebe tipo, controle de fluxo, etc
} enum_Tipo;

// Se o simbolo esta no escopo global ou local
typedef enum enum_Escopo{
    GLOBAL,
    LOCAL,
} enum_Escopo;

// Guarda ponteiro para strings de valor_lexico
typedef struct LISTA_PTR
{
	char *ptr;
	struct LISTA_PTR *prox;
} LISTA_PTR;


extern LISTA_PTR *lista_ptr; // var global


// Adiciona ptr para lista
void novoPTR(char* ptr_string, LISTA_PTR** lista);


// Cria uma copia com as mesmas referencias, mas nova struct
LISTA_PTR* copiaLista(LISTA_PTR* lista);


// Concatena duas listas de ponteiros
LISTA_PTR* concatLista(LISTA_PTR* l1, LISTA_PTR* l2);


// Libera a memoria alocada para strings de valor_lexico
void liberaPTR(LISTA_PTR *lista);


// Libera a meme da struct LISTA_PTR, pq o char* ja esta sendo liberado em outro lugar
void liberaStructPTR(LISTA_PTR* lista);


// Tamanho de cada tipo
int bytes_por_tipo(enum_Tipo tipo);


// Retorna char para enum_Tipo DEBUG
char charDoTipo(enum_Tipo tipo);


// strdup(), que aceita NULL
char* copiaStr(char* str);


// Retorna string com o valor float
char* float_to_string(float valor);


// Retorna string com o valor int
char* int_to_string(int valor);


// Coloca char no formato de string 
char* char_to_string(char valor);


// Cria label para nodo de chamada de funcao
char *label_chamada(char *nome_func);

#endif