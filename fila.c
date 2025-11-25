#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fila.h"

struct fila_t *fila_cria ()
{
    struct fila_t *f;

    f = malloc (sizeof (struct fila_t));

    if(!f)
        return NULL;

    f->prim = NULL;
    f->ult = NULL;
    f->num = 0;

    return f;
}

struct fila_t *fila_destroi (struct fila_t *f)
{
    struct fila_nodo_t *atual, *prox;

    if(!f)
        return NULL;

    atual = f->prim;

    while(atual)
    {
        prox = atual->prox;
        free(atual);
        atual = prox;
    }

    free(f);

    return NULL;
}

int fila_insere (struct fila_t *f, int item)
{
    struct fila_nodo_t *novo;

    if(!f)
        return 0;

    novo = malloc (sizeof (struct fila_nodo_t));

    if(!novo)
        return 0;

    novo->item = item;
    novo->prox = NULL;

    // caso a fila esteja vazia
    if(!f->prim)
    {
        f->prim = novo;
        f->ult = novo;  
    }
    
    else
    {
        f->ult->prox = novo;
        f->ult = novo;
    }
    
    f->num++;
    return 1;
}

int fila_retira (struct fila_t *f, int *item)
{
    struct fila_nodo_t *temp;

    if(!f || !f->num)
        return 0;

    temp = f->prim;
    *item = temp->item;
    f->prim = f->prim->prox;

    free(temp);
    f->num--;

    // se a fila ficou vazia
    if(!f->num)
    {
        f->ult = NULL;
        f->prim = NULL;
    }

    return 1;
}

int fila_tamanho (struct fila_t *f)
{
    if(!f)
        return -1;

    return f->num;
}

void fila_imprime (struct fila_t *f)
{
    struct fila_nodo_t *atual;

    if(!f)
        return;

    atual = f->prim;

    while(atual)
    {
        printf("%d ", atual->item);
        atual = atual->prox;
    }

}