#ifndef MUNDO
#define MUNDO

#include "fprio.h"
#include "conjunto.h"
#include "fila.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 5)
#define N_MISSOES (T_FIM_DO_MUNDO / 100)
#define CHEGA 0
#define ESPERA 1
#define DESISTE 2
#define AVISA 3
#define ENTRA 4
#define SAI 5
#define VIAJA 6
#define MORRE 7
#define MISSAO 8
#define FIM 9

struct coordenada
{
  int x;
  int y;
};

struct distancia_bases
{
  int idbase;
  float distancia;
};

struct heroi
{
  int id_h;               
  int paciencia;             
  int velocidade;              
  int xp;                       
  int base;                    
  int vida;                    
  struct cjto_t *hab;         
};

struct base
{
  int id;                   
  int lot;                  
  int fila_max;             
  int num_missoes;          
  struct cjto_t *pres;      
  struct fila_t *espera;     
  struct coordenada local;  
};

struct missao
{
  int idm;                                                
  int tentativa;                    
  struct cjto_t *hab;               
  struct distancia_bases distancia[N_BASES];  
  struct coordenada coord_m;  
};

struct evento
{
  int time;    
  int heroi;    
  int base;   
  int missao;   
};

struct mundo
{
  int n_herois;                   
  int n_bases;                     
  int n_missoes;                   
  int n_hab;                       
  int tam_m;                    
  int relogio;                      
  int missoes_cumpridas;   
  int ncompostov;            
  struct heroi vherois[N_HEROIS];    
  struct base vbases[N_BASES];        
  struct missao vmissoes[N_MISSOES];  
  struct fprio_t *lef;           
};

//Função que atualiza, a cada evento que usa a fila de espera na base, qual foi o tamanho maior dela
void maior_fila(struct mundo *w,int b);

//Inicializa os herois a partir de i
struct heroi inicializa_heroi (struct mundo *w,int id_h);

//Inicializa a base a partir de i
struct base inicializa_base (struct mundo *w, int id_b);

//Inicializa a missão a partir de i
struct missao inicializa_missao (struct mundo *w, int id_m);

//Cria e inicializa o mundo, o vetor dos heróis, das bases e das missões
// Retorna o mundo inicializado
struct mundo inicializa_mundo ();

void inicializa_eventos (struct mundo *w);

// Ordena um vetor de struct distancia_base pelo campo 'distancia'
void insertion_sort(struct distancia_bases vetor[], int num);

int simula_mundo ();

//Libera toda a memória da simulação (tudo que foi alocado dinamicamente)
void destroi_simulacao (struct mundo *w);

#include "eventos.h"

#endif
