#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "eventos.h"
#include "mundo.h"


void maior_fila (struct mundo *w, int b)
{

    int tam;

    tam = fila_tamanho(w->vbases[b].espera);

    //calcula se esse é realmente o maior tamanho de fila, substitui na variável se é
    if(tam > w->vbases[b].fila_max)
        w->vbases[b].fila_max = tam;
    
}

struct heroi inicializa_heroi (struct mundo *w, int id_h)
{
    //inicializa a struct herói com valores aleatórios para a paciência, velocidade e habilidades
    //e 0 para xp, base e vida
    struct heroi h;

    h.id_h = id_h;
    h.xp = 0;
    h.paciencia = aleat(0, 100);
    h.velocidade = aleat(50, 5000);
    h.hab = cjto_aleat(aleat(1, 3), w->n_hab);
    h.base = -1;
    h.vida = 0; //quando vida == 0, o herói está vivo

    return h;
}

struct base inicializa_base (struct mundo *w, int id_b)
{

    //inicializa a struct base com valores aleatórios para a localização, lotação, fila máxima e número de missões
    //o restante, inicializa com zero
    struct base b;

    b.id = id_b;
    b.local.x = aleat(0, w->tam_m - 1);
    b.local.y = aleat(0, w->tam_m - 1);
    b.lot = aleat(3, 10);
    b.pres = cjto_cria(w->n_herois);
    b.espera = fila_cria();
    b.fila_max = 0;
    b.num_missoes = 0;

    return b;
}

struct missao inicializa_missao (struct mundo *w, int id_m)
{
    //inicializa a struct missão com valores aleatórios para a localização, habilidades
    //inicializa tentativa com 1 
    struct missao m;

    m.idm = id_m;
    m.coord_m.x = aleat(0, w->tam_m - 1);
    m.coord_m.y = aleat(0, w->tam_m - 1);
    m.hab = cjto_aleat(aleat(6, 10), w->n_hab);
    m.tentativa = 1;

    return m;
}

struct mundo inicializa_mundo ()
{
    struct mundo w;

    //inicializa os valores do mundo
    w.n_herois = N_HEROIS;
    w.n_bases = N_BASES;
    w.n_hab = N_HABILIDADES;
    w.n_missoes = N_MISSOES;
    w.tam_m = N_TAMANHO_MUNDO;
    w.relogio = T_INICIO;
    w.lef = fprio_cria();
    w.missoes_cumpridas = 0;
    w.ncompostov = 30;

    //inicializa o vetor de heróis
    for(int i = 0; i < w.n_herois; i++)
    {
        w.vherois[i] = inicializa_heroi(&w, i);
    }

    //inicializa o vetor de bases
    for(int i = 0; i < w.n_bases; i++)
    {
        w.vbases[i] = inicializa_base(&w, i);
    }

    //inicializa o vetor de missões
    for(int i = 0; i < w.n_missoes; i++)
    {
        w.vmissoes[i] = inicializa_missao(&w, i);

        //calcula a distância entre a missão e as bases
        for(int k = 0; k < w.n_bases; k++)
        {
            w.vmissoes[i].distancia[k].idbase = k;
            w.vmissoes[i].distancia[k].distancia = distancia(w.vmissoes[i].coord_m, w.vbases[k].local);
        }

        //ordena as distâncias no vetor de cada missão(para facilitar os calculos)
        insertion_sort(w.vmissoes[i].distancia, w.n_bases);
    }

    return w;
}

void destroi_simulacao (struct mundo *w)
{

    //destrói todas as alocações dinamicamente feitas no mundo
    //como os vetores são estáticos, não preciso liberar eles
    for(int i = 0; i < w->n_herois; i++)
    {
        cjto_destroi(w->vherois[i].hab);
    }

    for(int i = 0; i < w->n_bases; i++)
    {
        cjto_destroi(w->vbases[i].pres);
        fila_destroi(w->vbases[i].espera);
    }

    for(int i = 0; i < w->n_missoes; i++){
        cjto_destroi(w->vmissoes[i].hab);
    }

    fprio_destroi(w->lef);
}


void inicializa_eventos (struct mundo *w)
{
    struct evento *e;

    //inicializa todos os eventos chega. Se a alocação der errado, ele retorna
    //e não continua a execução do programa
    for(int i = 0; i < w->n_herois; i++)
    {
        e = cria_evento(aleat(0, 4320), i, aleat(0, w->n_bases - 1), -1);
        if(!e)
            return;
        
        fprio_insere(w->lef, e, CHEGA, e->time);
    }

    //inicializa todas as missões em um t aleatório entre 0 e T_FIM_DO_MUNDO
    //se a alocação der errado, ele retorna
    for(int i = 0; i < w->n_missoes; i++)
    {
        e = cria_evento(aleat(0, T_FIM_DO_MUNDO), -1, -1, i);
        if(!e)
            return;

        fprio_insere(w->lef, e, MISSAO, e->time);
    }

    //inicializa o evento fim, agendado para o fim da simulação
    e = cria_evento(T_FIM_DO_MUNDO, -1, -1, -1);
    if(!e)
        return;

    fprio_insere(w->lef, e, FIM, e->time);
}

int simula_mundo()
{
    struct mundo w;
    struct evento *e;
    int qev, tipo, prioridade;
    int final = 0;

    w = inicializa_mundo();  
    inicializa_eventos(&w); 
    w.relogio = 0;
    qev = 0; 

    while(!final)
    {
        e = fprio_retira(w.lef, &tipo, &prioridade);
        qev++; 
        switch(tipo)
        {
            case CHEGA:
                w.relogio = prioridade;
                chega(&w, w.relogio, e->heroi, e->base);
                break;
            case ESPERA:
                w.relogio = prioridade;
                espera(&w, w.relogio, e->heroi, e->base);
                break;
            case DESISTE:
                w.relogio = prioridade;
                desiste(&w, w.relogio, e->heroi, e->base);
                break;
            case AVISA:
                w.relogio = prioridade;
                avisa(&w, w.relogio, e->base);
                break;
            case ENTRA:
                w.relogio = prioridade;
                entra(&w, w.relogio, e->heroi, e->base);
                break;
            case SAI:
                w.relogio = prioridade;
                sai(&w, w.relogio, e->heroi, e->base);
                break;
            case VIAJA:
                w.relogio = prioridade;
                viaja(&w, w.relogio, e->heroi, e->base);
                break;
            case MORRE:
                w.relogio = prioridade;
                morre(&w, w.relogio, e->heroi, e->base, e->missao);
                break;
            case MISSAO:
                w.relogio = prioridade;
                missao(&w, w.relogio, e->missao);
                break;
            case FIM:
                w.relogio = prioridade;
                fim(&w, w.relogio, qev);
                final = 1; 
                break;
        }

        free(e); 
        e = NULL;
    }
    
    destroi_simulacao(&w); 

    return 0;
}

