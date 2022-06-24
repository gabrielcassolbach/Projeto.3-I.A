/*============================================================================*/
/*----------------------------------------------------------------------------*/
/*============================================================================*/
/** Um rob� que est� tentando sobreviver **/
/*============================================================================*/

#ifndef __T3_H
#define __T3_H

/*============================================================================*/

#include "game_state.h"

/*============================================================================*/
typedef struct
{
    int** matrizProb;
    int* vetorCaminho;
    int** matrizElementos;
    Coordenada ant;
    int tem_melhor_caminho;
    int achou_agua;
    int num_movimentos;
    int altura;
    int largura;
} Decisor;

/*============================================================================*/
Decisor* criaDecisor (int altura, int largura);
void destroiDecisor (Decisor* d);
int proximoMovimento (Decisor* d, Coordenada pos, int agua, int n_lava);
/*============================================================================*/
#endif
