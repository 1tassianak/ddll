#include "DDLL_pri.h"
#include <stdlib.h>


/*compilação do programa para que apareça as mensagens de erros e warnings:
// gcc -Wall -Wextra -g myApp.c mylib.c -o main

passar valgrind com relatório completo:
// valgrind --leak-check=full --track-origins=yes ./main */


// Funções básicas de uma DDLL
int cDDLL(ppDDLL pp, int sizedata){
    /*verifica se pp é nulo (caso na main na hora de criar envie um cDLL(NULL, sizeof(in))) e se sizedata tem algum tamanho para ser alocado*/
    if(pp == NULL || *pp != NULL || sizedata <= 0) return FAIL;

    /*aloca memória para a estrutura*/
    *pp = malloc(sizeof(DDLL));
    /*se não alocou, falha*/
    if(!*pp) return FAIL;

    /*inicializa as variáveis*/
    (*pp)->inicio = NULL;
    (*pp)->fim = NULL;
    (*pp)->qtd = 0;
    (*pp)->sizedata = sizedata;

    return SUCCESS;
}

int dDDLL(ppDDLL pp){
    /*verificar se o ponteiro da lista e o conteúdo que o ponteiro pp da lista aponta não são null*/
    if(pp == NULL || *pp == NULL) return FAIL;
    
    /*chama cleanDDLL para limpar os dados*/
    cleanDDLL(*pp);

    /*libera a estrutura da lista*/
    free(*pp);

    /*atribui NULL ao ponteiro*/
    *pp = NULL;

    return SUCCESS;
}

int cleanDDLL(pDDLL p){
    /*verifica se o ponteiro da estrutura é null
    para maior segurança, não verifica se a estrutura está vazia, pois clean funcionará mesmo se a lista estiver vazia
    permitindo que a função seja chamada a qualquer momento ou repetidas vezes sem dar bug*/
    if(p == NULL || p->sizedata <= 0) return FAIL;

    /*cria um ponteiro que recebe o dado inicial para começar a limpar*/
    No *atual = p->inicio;
    /*percorre os nós para liberar memória*/
    while (atual !=NULL){
        /*guarda o valor do próximo nó para não o perder quando apagar o primeiro nó*/
        No *prox = atual->next;
        /*libera o dado inicial (apaga)*/
        free(atual->dados);
        /*libera o ponteiro atual*/
        free(atual);
        /*segue para o próximo nó para que possa apagar o próximo dado*/
        atual = prox;
    }

    /*reseta as variáveis da estrutura*/
    p->inicio = 0;
    p->fim = 0;
    p->qtd = 0;

    return SUCCESS;
}

int iBegin(pDDLL p, void *element){
    /*verifica se a estrutura é null e se elemento é válido*/
    if(p == NULL || p->sizedata <= 0 || element == NULL) return FAIL;
    
    /*aloca memória para o novo nó*/
    No *novo_no = malloc(sizeof(No));
    if (!novo_no) return FAIL;

    /*aloca memória para o dado do novo nó*/
    novo_no->dados = malloc(p->sizedata);
    if(!novo_no->dados){
        /*se não alocou, libera a memória*/
        free(novo_no);
        return FAIL;
    }

    /*copia o novo elemento para o novo nó*/
    memcpy(novo_no->dados, element, p->sizedata);

    /*novo nó vai para o início da lista, então precisa conectar o novo nó à lista
    apontando o novo_no->next para o antigo inicio da lista*/
    novo_no->next = p->inicio;
    /*aponta novo_no->prev para NULL, pois o novo nó é o primeiro agora*/
    novo_no->prev = NULL;

    /*se a lista não estiver vazia*/
    if(p->inicio != NULL){
        /*o antigo início da lista precisa apontar de volta para o novo início*/
        p->inicio->prev = novo_no;
    }else{
        /*se a lista estava vazia, p->fim também é NULL
        então p->fim deve apontar para o novo nó, pois ele também é o último da lista*/
        p->fim = novo_no;
    }

    /*indiferente da lista estar vazia ou não, o novo início agora é o novo nó
    atualiza o ponteiro de inicio da lista*/
    p->inicio = novo_no;

    /*incrementa a quantidade de elementos na fila*/
    p->qtd++;

    return SUCCESS;
}

int iEnd(pDDLL p, void *element){
    /*verifica se a estrutura é null, se sizedata e elemento são válidos*/
    if(p == NULL || p->sizedata <= 0 || element == NULL) return FAIL;
    
    /*aloca memória para o novo nó*/
    No *novo_no = malloc(sizeof(No));
    if (!novo_no) return FAIL;

    /*aloca memória para o dado do novo nó*/
    novo_no->dados = malloc(p->sizedata);
    if(!novo_no->dados){
        /*se não alocou, libera a memória*/
        free(novo_no);
        return FAIL;
    }

    /*copia o novo elemento para o novo nó*/
    memcpy(novo_no->dados, element, p->sizedata);

    /*novo nó vai para o fim da lista, então precisa conectar o novo nó à lista
    apontando o novo_no->next para null*/
    novo_no->next = NULL;
    /*aponta novo_no->prev para antigo nó final, pois o novo nó é o último agora*/
    novo_no->prev = p->fim;

    /*se a lista não estiver vazia*/
    if(p->fim != NULL){
        /*o antigo fim da lista precisa apontar de volta para o novo fim*/
        p->fim->next = novo_no;
    }else{
        /*se a lista estava vazia, p->inicio também é NULL
        então p->inicio deve apontar para o novo nó, pois ele também é o primeiro da lista*/
        p->inicio = novo_no;
    }

    /*indiferente da lista estar vazia ou não, o novo fim agora é o novo nó
    atualiza o ponteiro de fim da lista*/
    p->fim = novo_no;

    /*incrementa a quantidade de elementos na lista*/
    p->qtd++;

    return SUCCESS;
}

int iPosition(pDDLL p, int N, void *element){
    /*verifica se a estrutura é null e se elemento, sizedata e N são válidos, N precisa estar entre 0 (início) e p->qtd (fim)*/
    if(p == NULL || p->sizedata <= 0 || element == NULL || N < 0 || N > p->qtd) return FAIL;
    
    /*verifica se N é o index da primeira posição, se for, reaproveita a inserção iBegin*/
    if(N==0) return iBegin(p, element);

    /*caso N for igual à quantidade de itens, indica que N é o index final, então reaproveita a inserção iEnd*/
    if(N == p->qtd) return iEnd(p, element);

    /*se N não tiver index inicial ou final, navega até a posição solicitada
    para isso, utiliza-se um ponteiro auxiliar, que recebe p->inicio, pois quero encontrar a posição à frente dele*/
    No *atual = p->inicio;
    /*percorre a lista para encontrar o nó*/
    for(int i = 0; i < N; i++) {
        /*armazena o nó encontrado*/
        atual = atual->next;
    }
    /*cria o novo nó, para depois inserir o elemento*/
    No *novo = malloc(sizeof(No));
    if(!novo) return FAIL;

    /*aloca memória para o novo dado*/
    novo->dados = malloc(p->sizedata);
    if (!novo->dados){
        /*se não alocou, libera a memória*/
        free(novo);
        return FAIL;
    }

    /*copia o dado de element para o novo nó*/
    memcpy(novo->dados, element, p->sizedata);

    /*linka o novo nó na lista
    ex: lista [10] <-> [20] <-> [30] quero inserir [25] entre [20] e [30]
    atual->prev é [20] e atual é [30]
    então: novo->prev aponta para o nó anterior ao atual (atual->prev)*/
    novo->prev = atual->prev; //novo->prev = 20
    /*novo->next aponta para atual*/
    novo->next = atual; //novo->next = 30
    /*o anterior à atual aponta para o novo*/
    atual->prev->next = novo; //o 20 aponta para o 25 como próximo
    /*atual->prev deve ser atualizado para receber o novo nó*/
    atual->prev = novo;//30 agora aponta para 25 como anterior
    //resultado: [10] <-> [20] <-> [25] <-> [30]

    /*incrementa o número de elementos na lista*/
    p->qtd++;

    return SUCCESS;
}

int rBegin(pDDLL p, void *element){
    /*verifica se a estrutura e p->inicio são null, se elemento e sizedata são válidos, e se a lista não está vazia (p->qtd = 0)*/
    if(p == NULL || p->inicio == NULL || element == NULL || p->qtd == 0 || p->sizedata <= 0) return FAIL;

    /*cria um nó para armazenar o nó a ser removido (não é necessário alocar memória)
    pois já estou atribuindo valor*/
    No *remover = p->inicio;

    /*copia o dado a ser removido para o element retornar para a main*/
    memcpy(element, remover->dados, p->sizedata);

    /*atualiza o nó inicio para apontar para o próximo nó*/
    p->inicio = remover->next;

    /*se o novo inicio não for NULL, ainda tem elementos, então atualiza o prev do no inicial*/
    if(p->inicio != NULL){
        p->inicio->prev = NULL;
    }else{
        /*se a lista ficou vazia, fim também será null*/
        p->fim = NULL;
    }

    /*libera a memória*/
    free(remover->dados);
    free(remover);

    /*decrementa a qtd de elementos na lista*/
    p->qtd--;

    return SUCCESS;
}

int rEnd(pDDLL p, void *element){
    /*verifica se a estrutura e p->inicio são null, se elemento e sizedata são válidos, e se a lista não está vazia (p->qtd = 0)*/
    if(p == NULL || p->fim == NULL || element == NULL || p->qtd == 0 || p->sizedata <= 0) return FAIL;
    
    /*cria um ponteiro para um nó para armazenar o nó a ser removido (não é necessário alocar memória)*/
    No *remover = p->fim;

    /*copia o dado a ser removido para o element retornar para a main*/
    memcpy(element, remover->dados, p->sizedata);

    /*atualiza o nó do final para apontar para o nó anterior a ele, que será o novo nó final*/
    p->fim = remover->prev;

    /*se a lista não estiver vazia*/
    if(p->fim != NULL){
        /*novo fim não aponta para ninguém à frente (next)*/
        p->fim->next = NULL;
    }else{
        /*se a lista ficou vazia (p->fim == NULL), inicio também será null*/
        p->inicio = NULL;
    }

    /*libera a memória*/
    free(remover->dados);
    free(remover);

    /*decrementa a qtd de elementos na lista*/
    p->qtd--;

    return SUCCESS;    
}

int rPosition(pDDLL p, int N, void *element){
    /*verifica se a estrutura é null e se elemento, sizedata e N são válidos, N precisa estar entre 0 (início) e p->qtd (fim)*/
    if (p == NULL || p->sizedata <= 0 || element == NULL || N < 0 || N >= p->qtd) return FAIL;
    
    /*se a posição N é no início, reutiliza rBegin*/
    if (N == 0) return rBegin(p, element);

    /*se a posição N é a última, reutiliza rEnd*/
    if (N == p->qtd - 1) return rEnd(p, element);

    /*se N não for inicial nem final
    cria um nó para armazenar o nó inicial para percorrer a lista*/
    No *atual = p->inicio;
    for(int i=0; i < N; i++){
        /*achou a posição a ser removida*/
        atual = atual->next;
    }

    /*o next do nó anterior à posição aponta para o próximo (next) atual*/
    atual->prev->next = atual->next;
    /*o prev do próximo(next) nó deve apontar para o anterior(prev) do nó atual*/
    atual->next->prev = atual->prev;

    /*copia os dados do nó a ser removido para element*/
    memcpy(element, atual->dados, p->sizedata);

    /*libera a memória alocada*/
    free(atual->dados);
    free(atual);

    /*decrementa a quantidade de elementos da lista*/
    p->qtd--;

    return SUCCESS;
}

int sBegin(pDDLL p, void *element){
    /*verifica se p e p->inicio não são nulos, se element e sizedata são válidos e se a lista não está vazia*/
    if(p == NULL || p->inicio == NULL || element == NULL || p->qtd == 0 || p->sizedata <= 0) return FAIL;
    
    /*copia o dado do início para element, sem deletar nada*/
    memcpy(element, p->inicio->dados, p->sizedata);

    return SUCCESS;
}

int sEnd(pDDLL p, void *element){
    /*verifica se p e p->inicio não são nulos, se element e sizedata são válidos e se a lista não está vazia*/
    if(p == NULL || p->fim == NULL || element == NULL || p->qtd == 0 || p->sizedata <= 0) return FAIL;

    /*copia o dado do início para element, sem deletar nada*/
    memcpy(element, p->fim->dados, p->sizedata);

    return SUCCESS;
}

int sPosition(pDDLL p, int N, void *element){
    /*verifica se a estrutura é null e se elemento, sizedata e N são válidos, N precisa estar entre 0 (início) e p->qtd (fim)*/
    if (p == NULL || p->sizedata <= 0 || element == NULL || N < 0 || N >= p->qtd) return FAIL;
    
    /*cria um nó para percorrer a lista até encontrar N*/
    No *atual = p->inicio;
    for(int i = 0; i < N; i++){
        atual = atual->next;
    }

    /*copia o dado para element retornar para main*/
    memcpy(element, atual->dados, p->sizedata);

    return SUCCESS;
}

int empty(pDDLL p){
    /*verifica se a estrutura é nula, se for, a lista não existe, se qtd e sizedata são válidos indicando que possem elementos na lista*/
    if(p == NULL || p->sizedata <= 0 || p->qtd == 0) return SUCCESS;
    return FAIL;
}

int countElements(pDDLL p){
    /*verifica se a estrutura é nula, se for, a lista não existe, se sizedata é menor ou igual a 0 não possui elementos*/
    if(p == NULL || p->sizedata <= 0) return FAIL;

    return p->qtd;
}

/*Exibe todos os elementos da lista da esquerda para a direita*/
void printList(pDDLL p, int tipo) {
    /*não retorna FAIL, apenas return, pois a função é void e não deve retornar um número (FAIL = 1)*/
    if (p == NULL || p->inicio == NULL || p->sizedata <= 0) return;
    
    /*cria um nó armazenando p->inicio*/
    No *atual = p->inicio;

    /*percorre a lista para imprimir*/
    while (atual != NULL) {
        /*verifica se for tipo int*/
        if (tipo == 0)
            printf("%d ", *(int *)atual->dados);
        /*verifica se for tipo float*/
        else if (tipo == 1)
            printf("%.2f ", *(float *)atual->dados);
        /*se não é string*/
        else
            printf("%s ", (char *)atual->dados);
        /*passa para o próximo dado*/    
        atual = atual->next;
    }
    
    printf("\n");
}
