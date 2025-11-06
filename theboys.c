// programa principal do projeto "The Boys - 2025/1"
// Autor: Letícia de Oliveira Santos, GRR 20244503

//seus includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "theboys.h"

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

void maior_fila (struct mundo *w, int b)
{

    int tam;

    tam = lista_tamanho(w->vbases[b].espera);

    //calcula se esse é realmente o maior tamanho de fila, substitui na variável se é
    if(tam > w->vbases[b].fila_max)
    {
        w->vbases[b].fila_max = tam;
    }
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

    //retorna a struct herói inicializada
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
    b.espera = lista_cria();
    b.fila_max = 0;
    b.num_missoes = 0;

    //retorna a struct base inicializada
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

    //retorna a struct missão inicializada
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

        //ordena as distâncias no vetor (para facilitar os calculos na missão)
        insertion_sort(w.vmissoes[i].distancia, w.n_bases);
        //tem que mudar isso daqui, não precisa de uma ordenação, só de calcular as dist
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
        lista_destroi(w->vbases[i].espera);
    }

    for(int i = 0; i < w->n_missoes; i++){
        cjto_destroi(w->vmissoes[i].hab);
    }

    fprio_destroi(w->lef);
}


void inicializa_eventos (struct mundo *w)
{
    struct evento *e;

    //inicializa todos os eventos chega. Se a aocação der errado, ele retorna
    //e não continua a execução do programa
    for(int i = 0; i < w->n_herois; i++)
    {
        if(!(e = malloc(sizeof(struct evento))))
            return;
        
        e->time = aleat(0, 4320);
        e->heroi = i;
        e->base = aleat(0, w->n_bases - 1);
        e->missao = -1;
        fprio_insere(w->lef, e, CHEGA, e->time);
    }

    //inicializa todas as missões em um t aleatório entre 0 e T_FIM_DO_MUNDO
    //se a alocação der errado, ele retorna
    for(int i = 0; i < w->n_missoes; i++)
    {

        if(!(e = malloc(sizeof(struct evento))))
            return;

        e->time = aleat(0, T_FIM_DO_MUNDO);
        e->heroi = -1;
        e->base = -1;
        e->missao = i;
        fprio_insere(w->lef, e, MISSAO, e->time);
    }

    //inicializa o evento fim, agendado para o fim da simulação
    if(!(e = malloc(sizeof(struct evento))))
        return;

    e->time = T_FIM_DO_MUNDO;
    e->heroi = -1;
    e->base = -1;
    e->missao = -1;
    fprio_insere(w->lef, e, FIM, e->time);
}


void chega(struct mundo *w, int t, int h, int b)
{

    int espera, aux;
    struct evento *e;

    if(w->vherois[h].vida == 1) //se o herói já morreu, não faz nada
        return;

    if(!(e = malloc(sizeof(struct evento))))
        return;

    w->vherois[h].base = b; // atualiza a base do herói
    aux = lista_tamanho(w->vbases[b].espera);

    if(cjto_card(w->vbases[b].pres) < w->vbases[b].lot && !aux) // verifica se a base não está lotada e se não há fila de espera
        espera = 1;
    
    else
        espera = (w->vherois[h].paciencia) > (10 * aux);
    

    e->time = t;
    e->heroi = h;
    e->base = b;
    e->missao = -1;

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

void espera(struct mundo *w, int t, int h, int b)
{

    int aux;
    struct evento *e;

    if(w->vherois[h].vida == 1) // se o herói já morreu, não faz nada
        return;
    

    if(!(e = malloc(sizeof(struct evento))))
        return;

    aux = lista_tamanho(w->vbases[b].espera);

    lista_insere(w->vbases[b].espera, w->vherois[h].id_h, -1);
    maior_fila(w, b); // faz a atualização do maior valor da fila de espera

    e->time = t;
    e->heroi = -1;
    e->base = b;
    e->missao = -1;
    fprio_insere(w->lef, e, AVISA, t);

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", t, h, b, aux);
}

void desiste(struct mundo *w, int t, int h, int b)
{
    struct evento *e;

    if(w->vherois[h].vida == 1)
        return;
    

    if(!(e = malloc(sizeof(struct evento))))
        return;


    e->time = t;
    e->heroi = h;
    e->base = aleat(0, w->n_bases - 1);
    e->missao = -1;
    fprio_insere(w->lef, e, VIAJA, t);

    printf("%6d: DESIST HEROI %2d BASE %d\n", t, h, b);
}

void avisa(struct mundo *w, int t, int b)
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

        if(!(e = malloc(sizeof(struct evento))))
            return;

        e->time = t;
        e->heroi = h;
        e->base = b;
        e->missao = -1;
        fprio_insere(w->lef, e, ENTRA, t);

        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", t, b, h);
    }
}

void entra(struct mundo *w, int t, int h, int b)
{

    int tpb;
    struct evento *e;

    if(w->vherois[h].vida == 1)
        return;

    if(!(e = malloc(sizeof(struct evento))))
        return;

    tpb = 15 + w->vherois[h].paciencia * aleat(1, 20);

    e->time = t + tpb;
    e->heroi = h;
    e->base = b;
    e->missao = -1;
    fprio_insere(w->lef, e, SAI, e->time);

    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", t, h, b, cjto_card(w->vbases[b].pres), w->vbases[b].lot, e->time);
}

void sai(struct mundo *w, int t, int h, int b)
{
    struct evento *e;

    if(w->vherois[h].vida == 1)
        return;

    cjto_retira(w->vbases[b].pres, h);

    if(!(e = malloc(sizeof(struct evento))))
        return;

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

void viaja(struct mundo *w, int t, int h, int b)
{

    int b_agora, duracao, velocidade;
    float distancia_a;
    struct evento *e;

    if(w->vherois[h].vida == 1)
        return;

    if(!(e = malloc(sizeof(struct evento))))
        return;

    b_agora = w->vherois[h].base;
    distancia_a = distancia(w->vbases[b_agora].local, w->vbases[b].local);
    velocidade = w->vherois[h].velocidade;
    duracao = distancia_a / velocidade;
    w->vherois[h].base = -1;

    e->time = t + duracao;
    e->heroi = h;
    e->base = b;
    e->missao = -1;
    fprio_insere(w->lef, e, CHEGA, e->time);

    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %f VEL %d CHEGA %d\n", t, h, b_agora, b, distancia_a, velocidade, e->time);
}

void morre(struct mundo *w, int t, int h, int b, int missao){

    struct evento *e;

    if(w->vherois->vida)
        return;

    if(!(e = malloc(sizeof(struct evento))))
        return;

    w->vherois[h].vida = 1;
    w->vherois[h].base = -1;
    if(b >=0 && b < w->n_bases && w->vbases[b].pres != NULL)
    cjto_retira(w->vbases[b].pres, h);

    e->time = t;
    e->heroi = -1;
    e->base = b;
    e->missao = -1;
    fprio_insere(w->lef, e, AVISA, t);

    printf("%6d: MORRE  HEROI %2d MISSAO %d\n", t, h, missao);
}

void missao (struct mundo *w, int time, int missao)
{
  int bmp= -1;//base mais próxima, começa em -1 por causa da ordenação e das próximas verificações
  struct evento *e;
  struct cjto_t *habilidades_e = NULL;


  printf("%6d: MISSAO %d TENT %d HAB REQ : [", time, missao, w->vmissoes[missao].tentativa);
  cjto_imprime(w->vmissoes[missao].hab);
  printf("]\n");

  // percorre as bases para encontrar a primeira que tem as hab necessárias
  for (int i = 0; i < w->n_bases; i++)
  {
    int base_atual_id = w->vmissoes[missao].distancia[i].idbase; 
    
    // Aloca o conjunto de habilidades, se der erro, retorna
    habilidades_e = cjto_cria(N_HABILIDADES);
    if(!habilidades_e) {
        fprintf(stderr, "Erro: Falha ao alocar habilidades_e em evento_missao.\n");
        return;
    }

    if (cjto_card(w->vbases[base_atual_id].pres) > 0) 
    {
        // Percorre todos os heróis para ver quais estão na base atual
        for(int h_id = 0; h_id < w->n_herois ; h_id++)
        {
          // verifica se o h em questão está na base e está vivo
          if(cjto_pertence(w->vbases[base_atual_id].pres, w->vherois[h_id].id_h) && w->vherois[h_id].vida == 0)
          {
            // Une as habilidades do herói ao conjunto da missao
            for (int k = 0; k < N_HABILIDADES; k++) {
              if (cjto_pertence(w->vherois[h_id].hab, k)) 
                  cjto_insere(habilidades_e, k);
            }
          }
        }
    }
    else //se a base estiver 
    {
        cjto_destroi(habilidades_e);
        habilidades_e = NULL; 
        continue; 
    }

    // Verifica se o conjunto contém as habilidades da missão
    if(cjto_contem(habilidades_e, w->vmissoes[missao].hab))
    {
      bmp = base_atual_id; 
      break; 
      //se encontrou, o break sai do loop
    }
    cjto_destroi(habilidades_e); 
    habilidades_e = NULL;
  }

  //se há uma bmp com habilidades necessárias
  if(bmp != -1)
  {
    w->vbases[bmp].num_missoes++; 

    for (int h_id = 0; h_id < w->n_herois; h_id++) 
    {
      // Verifica se o herói está na base bmp e está vivo
      if(cjto_pertence(w->vbases[bmp].pres, w->vherois[h_id].id_h) && w->vherois[h_id].vida == 0)
        w->vherois[h_id].xp++;
    }

    printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [", time, missao, bmp);
    if (habilidades_e) cjto_imprime(habilidades_e); 
    printf("]\n");

    w->missoes_cumpridas++;
    if (habilidades_e) cjto_destroi(habilidades_e); 
    return; 
  }

  if(time % 2500 == 0 && w->ncompostov > 0)
  {
    if (w->n_bases == 0) 
    {
        printf("%6d: MISSAO %d IMPOSSIVEL \n", time, missao);
        w->vmissoes[missao].tentativa++; 
        if (!(e = malloc(sizeof(struct evento))))
        {
            fprintf(stderr, "Erro: Falha ao criar evento MISSAO reagendada para missao %d.\n", missao);
        } 
        else 
        {
            e->time = time + 24 * 60;
            e->missao = missao;
            e->heroi = -1;
            e->base = -1;
            fprio_insere(w->lef, e, MISSAO, time + 24 * 60);
        }
        return;
    }
    int base_id_para_composto_v = w->vmissoes[missao].distancia[0].idbase;
    
    if (cjto_card(w->vbases[base_id_para_composto_v].pres) > 0)
    {
      struct heroi *heroi_mais_experiente = NULL; 
      int max_xp = -1;
      int id_heroi_que_morrera = -1; 

      // encontra o herói mais experiente na base
      for(int h_idx = 0; h_idx < w->n_herois; h_idx++)
      {
        // vê se o herói está nesta base e está vivo
        if(cjto_pertence(w->vbases[base_id_para_composto_v].pres, w->vherois[h_idx].id_h) && (w->vherois[h_idx].vida == 0))
        {
          if(w->vherois[h_idx].xp > max_xp)
          {
            max_xp = w->vherois[h_idx].xp;
            heroi_mais_experiente = &(w->vherois[h_idx]); 
            id_heroi_que_morrera = w->vherois[h_idx].id_h; // guarda o id do herói que vai morrer
          }
        }
      }

      if(heroi_mais_experiente != NULL) 
      {
        w->ncompostov--; 
        w->missoes_cumpridas++; 

        if (!(e = malloc(sizeof(struct evento))))
        {
          fprintf(stderr, "Erro ao criar evento MORRE para heroi %d.\n", heroi_mais_experiente->id_h);
          return;
        }
        e->time = time;
        e->heroi = heroi_mais_experiente->id_h;
        e->base = base_id_para_composto_v;
        e->missao = missao;
        fprio_insere(w->lef, e, MORRE, time); 

        for (int h_idx = 0; h_idx < w->n_herois; h_idx++)
        {
          if (cjto_pertence(w->vbases[base_id_para_composto_v].pres, w->vherois[h_idx].id_h) &&
              (w->vherois[h_idx].vida == 0) &&
              w->vherois[h_idx].id_h != id_heroi_que_morrera) 
          {
            w->vherois[h_idx].xp++;
          }
        }
        printf("%6d: MISSAO %d CUMPRIDA COMPOSTO BASE %d HABS: [", time, missao, base_id_para_composto_v);
        cjto_imprime(w->vmissoes[missao].hab); 
        printf("]\n");
        return; 
      }
    }
  }


  w->vmissoes[missao].tentativa++;
  if (!(e = malloc(sizeof(struct evento))))
  {
    fprintf(stderr, "Erro: Falha ao criar evento MISSAO reagendada para missao %d.\n", missao);
    return; // Se falhou ao alocar, não faz nada mais
  }
  e->time = time + 24 * 60;
  e->missao = missao;
  e->heroi = -1;
  e->base = -1;
  fprio_insere(w->lef, e, MISSAO, time + 24 * 60);
  printf("%6d: MISSAO %d IMPOSSIVEL\n", time, missao);
}

void fim(struct mundo *w, int time, int qev){

    int i, mortos, tent, maior, menor;
    float aux, media;

    mortos = 0;

    printf("%6d: FIM\n", time);

    for(i = 0; i < w->n_herois; i++){
        if(w->vherois[i].vida == 1){
            printf("HEROI %2d MORTO  PAC %3d VEL %4d EXP %4d HABS [ ", i, w->vherois[i].paciencia, w->vherois[i].velocidade, w->vherois[i].xp);
            mortos++;
        }
        else
        {
            printf("HEROI %2d VIVO  PAC %3d VEL %4d EXP %4d HABS [ ", i, w->vherois[i].paciencia, w->vherois[i].velocidade, w->vherois[i].xp);
        }

        cjto_imprime(w->vherois[i].hab);
        printf(" ]\n");
    }

    for(i = 0; i < w->n_bases; i++){
        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", i, w->vbases[i].lot, w->vbases[i].fila_max, w->vbases[i].num_missoes);
    }

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

int main (){

    int qev, final, tipo, prioridade;
    struct evento *e;
    struct mundo w;

    srand(0);

    w = inicializa_mundo();
    inicializa_eventos(&w);
    w.relogio = 0;
    qev = 0;
    final = 0;

    while(!final){
        e = fprio_retira(w.lef, &tipo, &prioridade);
        qev++;

        switch(tipo){
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

    return (0);
}