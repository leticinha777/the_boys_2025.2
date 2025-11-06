// TAD lista de números inteiros
// Carlos Maziero - DINF/UFPR, Out 2024
//
// Implementação do TAD - a completar
// Implementação com lista encadeada dupla não-circular

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lista.h"


// Cria uma lista vazia.
// Retorno: ponteiro p/ a lista ou NULL em erro.
struct lista_t *lista_cria ()
{
    struct lista_t *lista = malloc(sizeof(struct lista_t));

    //Se nao foi possivel alocar, retorna NULL
    if(!lista)
        return NULL;

    lista->prim = NULL;
    lista->ult = NULL;
    lista->tamanho = 0;

    return lista; //retoena o ponteiro que aponta para a lista
}
    
// Remove todos os itens da lista e libera a memória.
// Retorno: NULL.
struct lista_t *lista_destroi (struct lista_t *lst)
{
    //verifica se a lista já é null
    if (!lst)
        return NULL;
    
    // Variáveis temporarias, que vão auxiliar a liberar os itens
    struct item_t *atual = lst->prim; 
    struct item_t *aux; 

    while (atual) // enquanto o atual for diferente de null
    {
        aux = atual->prox; // armazena o próximo 
        free(atual); // libera o item atual
        atual = aux; // move para o próximo item da lista
    }

    free(lst);
    return NULL;
}

int lista_insere (struct lista_t *lst, int item, int pos)
{
    if (!lst) 
        return -1; 

    struct item_t *novo_item = malloc(sizeof(struct item_t));

    if (!novo_item) 
        return -1;
     
    novo_item->valor = item;

    // caso a lista esteja vazia
    if (lst->prim == NULL)
    {
        novo_item->ant = NULL; 
        novo_item->prox = NULL; 
        lst->prim = novo_item; 
        lst->ult = novo_item; 
        lst->tamanho++;
        //retorna o novo tamanho da lista
        return lst->tamanho; 
    }
    
    // inserção no inicio da lista          
    if (pos == 0) 
    {
        novo_item->prox = lst->prim;
        novo_item->ant = NULL;
        lst->prim->ant = novo_item;
        lst->prim = novo_item;
        lst->tamanho++;
        return lst->tamanho;
    }

    //Caso 3: Se a posição for além do fim da lista ou for -1, insere no fim
    if ((pos >= lst->tamanho) || (pos == -1))
    {
        lst->ult->prox = novo_item; 
        novo_item->ant = lst->ult;  
        novo_item->prox = NULL; 
        lst->ult = novo_item;  
        lst->tamanho++;
        return lst->tamanho;
    }

    // caso geral (qqr posição)
    else
    {   
        struct item_t *temp = lst->prim; 
        for (int i = 0; i < pos; i++)
            temp = temp->prox; //avança a lista

        novo_item->prox = temp; 
        novo_item->ant = temp->ant;
        temp->ant->prox = novo_item;
        temp->ant = novo_item; 
        lst->tamanho++; 

        return lst->tamanho; 
    }
}

int lista_retira (struct lista_t *lst, int *item, int pos)
{
    if ((lst == NULL) || (item == NULL) || (pos < -1) || (pos >= lst->tamanho))
        return -1; 

    // se a lista estiver vazia
    if (lst->tamanho == 0)
        return -1; 

    struct item_t *aux;

    if (pos == -1) {
        pos = lst->tamanho - 1;
    }

    // Percorre a lista para encontrar o nó 'aux' a ser retirado
    aux = lst->prim;
    for (int i = 0; i < pos; i++) {
        if (aux == NULL) { //verificação extra 
            return -1;
        }
        aux = aux->prox;
    }
    
    // mais uma verificação extra
    if (aux == NULL) {
        return -1; 
    }

    *item = aux->valor; // Salva o valor do item antes de retirá-lo

    if (aux->ant != NULL) {
        aux->ant->prox = aux->prox;
    } else { 
        lst->prim = aux->prox;
    }

    // se o item não é o último da lista
    if (aux->prox != NULL) 
    {
        aux->prox->ant = aux->ant;
    } 
    else 
    { //se for o último
        lst->ult = aux->ant; 
    }
    
    // Decrementa o tamanho da lista (feito uma única vez)
    lst->tamanho--;

    // Libera a memória do nó retirado
    free(aux);

    // Retorna o novo tamanho da lista
    return lst->tamanho;
}

int lista_consulta (struct lista_t *lst, int *item, int pos)
{
    //Caso 1: erro
    if ((lst == NULL) || (lst->tamanho == 0) || (pos >=  lst->tamanho))
        return -1;

    //Caso 2: Consulta do fim
    if ((pos == -1) || (pos == (lst->tamanho -1)))
    {
        *item = lst->ult->valor;

        return lst->tamanho;
    }

    //Declara variável auxiliar:
    struct item_t *temp;
    temp = lst->prim;
    //Caso 3: Consulta na posição indicada:
    for (int i = 0; i < pos; i++)
        temp = temp->prox;

    *item = temp->valor;

    //Retorna o novo tamanho da lista:
    return lst->tamanho;
}

int lista_procura (struct lista_t *lst, int valor)
{
    
    //Caso 1: erro:
    if ((lst == NULL) || (lst->tamanho == 0))
        return -1;

    //Declara variável auxiliar:
    struct item_t *temp;
    int i = 0;
    temp = lst->prim;

    //Caso 2: percorre a lista até encontrar o fim ou o valor desejado:
    while ((temp != NULL))
    {
        if (temp->valor == valor)
            return i;
        temp = temp->prox;
        i++;
    }

    //Se o valor nao foi encontrado, retorna erro
    return -1;
}

int lista_tamanho (struct lista_t *lst)
{
    //Caso 1: lista nula, retorna erro
    if (lst == NULL)
        return -1;

    //Caso 2: Retorna o tamanho
    return lst->tamanho;
}

void lista_imprime (struct lista_t *lst)
{
    
    //Caso 1: erro
    if ((lst == NULL) || (lst->tamanho == 0))
        return;

    //Declara variável auxiliar:
    struct item_t *temp;
    temp = lst->prim;

    //Caso 2: 
    for (int i= 0; ((temp->prox != NULL) && (i < lst->tamanho - 1)); i++)
    {
        printf("%d ", temp->valor);
        temp = temp->prox;
    }
    printf("%d", temp->valor);
}





