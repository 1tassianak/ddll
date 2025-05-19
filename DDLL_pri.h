#ifndef DDLL_PRI_H
#define DDLL_PRI_H

#include "DDLL_pub.h"
#include <stdio.h>  /* para printf em printList */

typedef struct No{
    void *dados;         /* valor dinâmico */
    struct No *next;     /* ponteiro para próximo nó */
    struct No *prev;     /* ponteiro para nó anterior */
} No;

typedef struct DDLL{
    No *inicio;          /* ponteiro para início da lista */
    No *fim;             /* ponteiro para fim da lista */
    int qtd;             /* quantidade de elementos */
    int sizedata;        /* tamanho dos dados armazenados */
} DDLL;

/*imprime a lista, tipo: 0=int, 1=float, outros=string */
void printList(pDDLL p, int tipo);

#endif /* DDLL_PRI_H */