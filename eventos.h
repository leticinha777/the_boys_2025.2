#ifndef EVENTOS
#define EVENTOS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mundo.h"

struct mundo;

float distancia (struct coordenada b, struct coordenada d);

int aleat (int min, int max);

struct evento *cria_evento (int time, int heroi, int base, int missao);

//Função que trata o evento CHEGA,
void chega (struct mundo *w, int t,int h,int b);

//Função que trata o evento ESPERA
void espera (struct mundo *w, int t, int h, int b);

//Função que trata o evento DESISTE
void desiste (struct mundo *w, int t, int h, int b);

//Função que trata o evento AVISA
void avisa (struct mundo *w, int t, int b);

//Função que trata o evento ENTRA
void entra (struct mundo *w, int t, int h, int b);

//Função que trata o evento SAI
void sai (struct mundo *w, int t, int h, int b);

//Função que trata o evento VIAJA
void viaja (struct mundo *w, int t, int h, int b);

/* O herói H morre no instante T */
void morre (struct mundo *w, int t, int h, int b, int ms);

//Função usada para calcular as bases mais proximas da missão
int base_mais_proxima (struct mundo *w, int t, int missao);

//Função que trata o evento MISSAO
void missao (struct mundo *w, int t, int ms);

//Função que trata o evento FIM, imprime as estatísticas finais da simulação
void fim (struct mundo *w, int t, int qev);



#endif
