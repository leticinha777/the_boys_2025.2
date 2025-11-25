#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "eventos.h"


int aleat (int min, int max)
{
    //retorna um número aleatório entre min e max
    return rand() % (max - min + 1) + min;
}

//Ordena um vetor utilizando insertion Sort
void insertion_sort(struct distancia_bases vetor[], int num)
{
  struct distancia_bases  temp;
  for (int i = 1; i < num; i++)
  {
    int j = i -1;
    temp = vetor[i];

    while((j>=0) && vetor[j].distancia > temp.distancia)
    {
      vetor[j+1] = vetor[j];
      j--;
    }
    vetor[j +1] = temp;
  }
}

float distancia (struct coordenada b, struct coordenada d)
{
    //calcula a distância entre dois ponos usando pitágoras, retorna a distância como um float
    return sqrt((b.x - d.x)*(b.x - d.x) + (b.y - d.y)*(b.y - d.y));
}

struct evento *cria_evento (int time, int heroi, int base, int missao)
{
    struct evento *e;

    e = malloc (sizeof (struct evento));

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
    aux = fila_tamanho(w->vbases[b].espera);

    if(cjto_card(w->vbases[b].pres) < w->vbases[b].lot && !aux) // verifica se a base não está lotada e se não há fila de espera
        espera = 1;
    
    else
        espera = (w->vherois[h].paciencia) > (10 * aux);
    
    e = cria_evento(t,h,b,-1);

    if(!e)
        return;

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
    
    aux = fila_tamanho(w->vbases[b].espera);

    fila_insere(w->vbases[b].espera, w->vherois[h].id_h);
    maior_fila(w, b); // faz a atualização do maior valor da fila de espera

    e = cria_evento(t,-1,b,-1);

    if(!e)
        return;
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

    if(!e)
        return;

    fprio_insere(w->lef, e, VIAJA, t);

    printf("%6d: DESIST HEROI %2d BASE %d\n", t, h, b);
}

//Função que trata o evento AVISA
void avisa (struct mundo *w, int t, int b)
{
    int h;
    struct evento *e;

    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA [ ", t, b, cjto_card(w->vbases[b].pres) , w->vbases[b].lot);
    fila_imprime(w->vbases[b].espera);
    printf(" ]\n");

    while((cjto_card(w->vbases[b].pres) < w->vbases[b].lot) && (fila_tamanho(w->vbases[b].espera) > 0))
    {
        h = -1;
        if(fila_retira(w->vbases[b].espera, &h) == -1)
            return;

        cjto_insere(w->vbases[b].pres, h);

        e = cria_evento(t,h,b,-1);

        if(!e)
            return;

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

    if(!e)
        return;

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

    e = cria_evento(t,h,aleat(0, w->n_bases -1), -1);
    
    if(!e)
        return;

    fprio_insere(w->lef, e, VIAJA, t);

    e = cria_evento(t,-1,b,-1);

    if(!e)
        return;

    fprio_insere(w->lef, e, AVISA, t);

    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", t, h, b, cjto_card(w->vbases[b].pres), w->vbases[b].lot);
}

//Função que trata o evento VIAJA
void viaja (struct mundo *w, int t, int h, int b)
{
    int b_agora, duracao, velocidade;
    float distancia_a;
    struct evento *e;

    if(w->vherois[h].vida == 1)
        return;

    b_agora = w->vherois[h].base;
    distancia_a = distancia(w->vbases[b_agora].local, w->vbases[b].local);
    velocidade = w->vherois[h].velocidade;
    duracao = distancia_a / velocidade;
    w->vherois[h].base = -1;

    e = cria_evento(t+duracao, h, b, -1);
    
    if(!e)
        return;
    fprio_insere(w->lef, e, CHEGA, e->time);

    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %f VEL %d CHEGA %d\n", t, h, b_agora, b, distancia_a, velocidade, e->time);
}

/* O herói H morre no instante T */
void morre (struct mundo *w, int t, int h, int b, int ms)
{
    struct evento *e;

    if(w->vherois->vida)
        return;

    w->vherois[h].vida = 1;
    w->vherois[h].base = -1;
    if(b >=0 && b < w->n_bases && w->vbases[b].pres != NULL)
    cjto_retira(w->vbases[b].pres, h);

    e = cria_evento(t,-1,b,-1);

    if(!e)
        return;

    fprio_insere(w->lef, e, AVISA, t);

    printf("%6d: MORRE  HEROI %2d MISSAO %d\n", t, h, ms);
}

//Função usada para calcular as bases mais proximas da missão
int encontra_bmp_apta(struct mundo *w, int id_missao) 
{
    // Percorre essa lista
    for (int i = 0; i < w->n_bases; i++) 
    {
        int base_id = w->vmissoes[id_missao].distancia[i].idbase;

        //verifica se ha herois na base
        if (cjto_card(w->vbases[base_id].pres) > 0) 
        {
            //cria um conjunto temporario, para armazenar as habs da equipe
            struct cjto_t *habilidades_equipe = cjto_cria(N_HABILIDADES);
            if (!habilidades_equipe)
                return -1; 
            
            for (int h_id = 0; h_id < w->n_herois; h_id++) 
                //se o herói pertence ao conjunto da base e esta vivo, adiciona as habs ao cjto temporario  
                if (cjto_pertence(w->vbases[base_id].pres, w->vherois[h_id].id_h) && w->vherois[h_id].vida == 0) 
                {
                    for (int hab_id = 0; hab_id < w->n_hab; hab_id++) 
                    {
                        if (cjto_pertence(w->vherois[h_id].hab, hab_id)) 
                            cjto_insere(habilidades_equipe, hab_id);
                        
                    }
                }

            // vê se o conjunto de habs da equipe contem as habs da missão
            if (cjto_contem(habilidades_equipe, w->vmissoes[id_missao].hab)) 
            {
                cjto_destroi(habilidades_equipe);
                return base_id;
            }
            
            // se não for apta, destrói o conjunto temporário e tenta a próxima base
            cjto_destroi(habilidades_equipe);
        }
    }
    
    // não encontrou base apta
    return -1;
}

//Função que trata o evento MISSAO
void missao (struct mundo *w, int t, int ms)
{
  int bmp = encontra_bmp_apta(w, ms); // função auxiliar que encontra a base mais próxima apta
  struct evento *e;
  
  if(bmp != -1)
  {
    w->vbases[bmp].num_missoes++; 
    for (int h_id = 0; h_id < w->n_herois; h_id++) 
    {
        //incrementa a xp dos heróis que participaram da missao e estão vivos
      if(cjto_pertence(w->vbases[bmp].pres, w->vherois[h_id].id_h) && w->vherois[h_id].vida == 0)
        w->vherois[h_id].xp++;
    }
    printf("%6d: MISSAO %d CUMPRIDA BASE %d\n", t, ms, bmp);
    w->missoes_cumpridas++;
    return; 
  }

  //nenhuma equipe está apta, bmp = -1. O primeiro elemento do vetor de distâncias é a base mais próxima
    bmp = w->vmissoes[ms].distancia[0].idbase;

  if(t % 2500 == 0 && w->ncompostov > 0 && bmp != -1 && cjto_card(w->vbases[bmp].pres) > 0)
  {
    struct heroi *heroi_xp = NULL; 
    int max_xp = -1;
    int id_morto = -1; 
    
    // encontra o heroi mais experiente da base
    for(int h_idx = 0; h_idx < w->n_herois; h_idx++)
    {
        //se o herói está nesta base e está vivo
      if(cjto_pertence(w->vbases[bmp].pres, w->vherois[h_idx].id_h) && (w->vherois[h_idx].vida == 0))
      {
        //verifica se o herói tem mais xp que o maximo atual
        if(w->vherois[h_idx].xp > max_xp)
        {
          max_xp = w->vherois[h_idx].xp;
          heroi_xp = &(w->vherois[h_idx]); // guarda o endereço do herói mais experiente
          id_morto = w->vherois[h_idx].id_h;
        }
      }
    }

    if(heroi_xp != NULL) 
    {
      w->ncompostov--; 
      w->missoes_cumpridas++;

      e = cria_evento(t, heroi_xp->id_h, bmp, ms);
      if(!e)
          return;

      fprio_insere(w->lef, e, MORRE, t);

      // aumenta o xp dos outros herois
      for (int h_idx = 0; h_idx < w->n_herois; h_idx++)
      {
        if (cjto_pertence(w->vbases[bmp].pres, w->vherois[h_idx].id_h) && (w->vherois[h_idx].vida == 0) && w->vherois[h_idx].id_h != id_morto) 
          w->vherois[h_idx].xp++;
        
      }
      printf("%6d: MISSAO %d CUMPRIDA COMPOSTO BASE %d\n", t, ms, bmp);
      return;
    }
  }


  //missao adiada, impossivel no momento
  w->vmissoes[ms].tentativa++;
  e = cria_evento(t + 24 * 60, -1, -1, ms);
  if (!e)
    return;
  fprio_insere(w->lef, e, MISSAO, t + 24 * 60);
  printf("%6d: MISSAO %d IMPOSSIVEL\n", t, ms);
}

//Função que trata o evento FIM, imprime as estatísticas finais da simulação
void fim (struct mundo *w, int t, int qev)
{
    int i, mortos, tent, maior, menor;
    float aux, media;

    mortos = 0;

    printf("%6d: FIM\n", t);

    for(i = 0; i < w->n_herois; i++)
    {
        if(w->vherois[i].vida == 1)
        {
            printf("HEROI %2d MORTO  PAC %3d VEL %4d EXP %4d HABS [ ", i, w->vherois[i].paciencia, w->vherois[i].velocidade, w->vherois[i].xp);
            mortos++;
        }
        else
            printf("HEROI %2d VIVO  PAC %3d VEL %4d EXP %4d HABS [ ", i, w->vherois[i].paciencia, w->vherois[i].velocidade, w->vherois[i].xp);
        
        cjto_imprime(w->vherois[i].hab);
        printf(" ]\n");
    }

    for(i = 0; i < w->n_bases; i++)
        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", i, w->vbases[i].lot, w->vbases[i].fila_max, w->vbases[i].num_missoes);
    

    printf("EVENTOS TRATADOS: %d\n", qev);

    maior = 1;
    menor = 1;

    for(i = 0; i < w->n_missoes; i++){
        tent = w->vmissoes[i].tentativa;

        if(tent > maior){
            maior = tent;
            continue;
        }

        if(tent < menor){
            menor = tent;
        }
    }

    aux = w->missoes_cumpridas * 100.0 / w->n_missoes;

    media = (menor + maior) / 2.0;

    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", w->missoes_cumpridas, w->n_missoes, aux);
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", menor, maior, media);

    aux = mortos * 100.0 / w->n_herois;

    printf("TAXA MORTALIDADE: %.1f%%\n", aux);
}
