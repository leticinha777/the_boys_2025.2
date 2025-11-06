#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "eventos.h"

struct evento *cria_evento (int time, int heroi, int base, int missao)
{
    struct evento *e;

    if(!e)
        return NULL;

    e->base = base;
    e->time = time;
    e->heroi = heroi;
    e->missao = missao;

    return e;
}

//Função que trata o evento CHEGA,
void chega (struct mundo *w, int t,int h,int b)
{
    int espera, aux;
    struct evento *e;

    if(w->vherois[h].vida == 1) //se o herói já morreu, não faz nada
        return;

    w->vherois[h].base = b; // atualiza a base do herói
    aux = lista_tamanho(w->vbases[b].espera);

    if(cjto_card(w->vbases[b].pres) < w->vbases[b].lot && !aux) // verifica se a base não está lotada e se não há fila de espera
        espera = 1;
    
    else
        espera = (w->vherois[h].paciencia) > (10 * aux);
    
    e = cria_evento(t,h,b,-1);

    if(espera)
    {
        fprio_insere(w->lef, e, ESPERA, e->time);

        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n", t, h, b, cjto_card(w->vbases[b].pres), w->vbases[b].lot);
    }
    else
    {
        fprio_insere(w->lef, e, DESISTE, t);

        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n", t, h, b, cjto_card(w->vbases[b].pres), w->vbases[b].lot);
    }
}

//Função que trata o evento ESPERA
void espera (struct mundo *w, int t, int h, int b)
{
    int aux;
    struct evento *e;

    if(w->vherois[h].vida == 1) // se o herói já morreu, não faz nada
        return;
    
    aux = lista_tamanho(w->vbases[b].espera);

    lista_insere(w->vbases[b].espera, w->vherois[h].id_h, -1);
    maior_fila(w, b); // faz a atualização do maior valor da fila de espera

    e = cria_evento(t,-1,b,-1);
    fprio_insere(w->lef, e, AVISA, t);

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", t, h, b, aux);
}

//Função que trata o evento DESISTE
void desiste (struct mundo *w, int t, int h, int b)
{
    struct evento *e;

    if(w->vherois[h].vida == 1)
        return;
    
    e = cria_evento(t,h,(aleat(0,w->n_bases -1)),-1);
    fprio_insere(w->lef, e, VIAJA, t);

    printf("%6d: DESIST HEROI %2d BASE %d\n", t, h, b);
}

//Função que trata o evento AVISA
void avisa (struct mundo *w, int t, int b)
{
    int h;
    struct evento *e;

    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA [ ", t, b, cjto_card(w->vbases[b].pres) , w->vbases[b].lot);
    lista_imprime(w->vbases[b].espera);
    printf(" ]\n");

    while((cjto_card(w->vbases[b].pres) < w->vbases[b].lot) && (lista_tamanho(w->vbases[b].espera) > 0))
    {
        h = -1;
        if(lista_retira(w->vbases[b].espera, &h, 0) == 0) // Verifica se a remoção foi bem-sucedida
            continue;

        cjto_insere(w->vbases[b].pres, h);

        e = cria_evento(t,h,b,-1);
        fprio_insere(w->lef, e, ENTRA, t);

        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", t, b, h);
    }
}

//Função que trata o evento ENTRA
void entra (struct mundo *w, int t, int h, int b)
{
    int tpb;
    struct evento *e;

    if(w->vherois[h].vida == 1)
        return;

    tpb = 15 + w->vherois[h].paciencia * aleat(1, 20);

    e = cria_evento(t+tpb, h, b, -1);
    fprio_insere(w->lef, e, SAI, e->time);

    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", t, h, b, cjto_card(w->vbases[b].pres), w->vbases[b].lot, e->time);
}

//Função que trata o evento SAI
void sai (struct mundo *w, int t, int h, int b)
{
    struct evento *e;

    if(w->vherois[h].vida == 1)
        return;

    cjto_retira(w->vbases[b].pres, h);

    
    e->time = t;
    e->heroi = h;
    e->base = aleat(0, w->n_bases - 1);
    e->missao = -1;
    fprio_insere(w->lef, e, VIAJA, t);

    if(!(e = malloc(sizeof(struct evento))))
        return;

    e->time = t;
    e->heroi = -1;
    e->base = b;
    e->missao = -1;
    fprio_insere(w->lef, e, AVISA, t);

    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", t, h, b, cjto_card(w->vbases[b].pres), w->vbases[b].lot);
}

//Função que trata o evento VIAJA
void viaja (struct mundo *w, int t, int h, int b)

/* O herói H morre no instante T */
void morre (struct mundo *w, int t, int h, int b, int ms)

//Função usada para calcular as bases mais proximas da missão
int base_mais_proxima (struct mundo *w, int t, int missao)

//Função que trata o evento MISSAO
void missao (struct mundo *w, int t, int ms)

//Função que trata o evento FIM, imprime as estatísticas finais da simulação
void fim (struct mundo *w, int t, int qev)