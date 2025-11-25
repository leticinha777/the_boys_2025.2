// programa principal do projeto "The Boys - 2025/1"
// Autor: Letícia de Oliveira Santos, GRR 20244503

//seus includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mundo.h"

int main (){

    srand(time(NULL));

    if(simula_mundo())
        return (1);
        
   return (0);
}
