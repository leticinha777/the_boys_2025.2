// TAD Fila de prioridades (FPRIO) genérica
// Carlos Maziero, DINF/UFPR, Out 2024
// Implementação com lista encadeada simples

// A COMPLETAR
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fprio.h"

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fprio_t *fprio_cria ()
{
    struct fprio_t *f;
    f = malloc(sizeof (struct fprio_t));
    if (!f)
        return NULL;

    //Inicializa a fila sem nada
    f->prim = NULL;
    f->num = 0;
    return(f);
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fprio_t *fprio_destroi (struct fprio_t *f)
{
    if (!f)
        return NULL;
    struct fpnodo_t *aux = f->prim;
    struct fpnodo_t *temp;

    //percorre toda a fila, liberando-a nodo a nodo
    while (aux!= NULL)
    {
        temp = aux->prox;
        free(aux->item);
        free(aux);
        aux = temp;
    }
    free(f);
    return NULL;
}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
// Itens com a mesma prioridade devem respeitar a politica FIFO (retirar
// na ordem em que inseriu).
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio)
{   
    if (!f || !item)
    return (-1);
    
    struct fpnodo_t *temp = f->prim;
    
    //percorre a lista e retorna -1 se algum item for igual
    while (temp)
    {
        if (temp->item == item)
            return -1;
        temp = temp->prox;
    }

    //Faz o malloc do novo item e retorna -1 se der errado
    struct fpnodo_t *novo_item = (malloc (sizeof (struct fpnodo_t)));
    if (!novo_item)
        return -1;
    
    novo_item->item = item;
    novo_item->tipo = tipo;
    novo_item->prio = prio;

    //inserir no início
    if (!f->num || (prio < f->prim->prio))
    {
        novo_item->prox = f->prim;
        f->prim = novo_item;
        (f->num)++;
        return (f->num);
    }

    temp = f->prim;
    // percorre a fila até achar um item com a prioridade maior/fim da fila e insere
    while (((temp->prox) != NULL) && (temp->prox->prio <= novo_item->prio))
        temp = temp->prox;

    novo_item->prox = temp->prox;
    temp->prox = novo_item;
    (f->num)++;

    return f->num;
}
// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fprio_retira (struct fprio_t *f, int *tipo, int *prio)
{
    
    if (!f || !f->num || !tipo || !prio)
        return NULL;
    struct fpnodo_t * aux = f->prim;

    void *temp = aux->item;
    *tipo = aux->tipo;
    *prio = aux->prio;
    f->prim = f->prim->prox;
    (f->num)--;
    
    free(aux);
    return (temp);
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fprio_tamanho (struct fprio_t *f)
{
    if (!f)
        return -1;
    return(f->num);
}
// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime (struct fprio_t *f)
{
    if (!f->num)
        return;
    struct fpnodo_t *temp = f->prim;
    for (int i = 0; i<(f->num) -1; i++)
    {
        printf("(%d %d)", temp->tipo, temp->prio);
        printf(" ");
        temp = temp->prox;
    }
    printf("(%d %d)", temp->tipo, temp->prio);

}