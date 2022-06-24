/*============================================================================*/
/* UM DECISOR MUITO, MUITO SIMPLES PARA O PROBLEMA DA CAVERNA MARCIANA        */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu                                                     */
/*============================================================================*/
/** Um rob� muito ruim - ele decide seus movimentos aleatoriamente! */
/*============================================================================*/

#include <stdlib.h>

#include "decisor_aleatorio.h"

/*============================================================================*/
/** Simplesmente aloca um Decisor.
 *
 * Par�metros: int altura: n�mero de linhas no tabuleiro.
 *             int largura: n�mero de colunas no tabuleiro.
 *
 * Valor de Retorno: um Decisor alocado. */

Decisor* criaDecisor (int altura, int largura)
{
    return ((Decisor*) malloc (sizeof (Decisor)));
}

/*----------------------------------------------------------------------------*/
/** Simplesmente desaloca um Decisor.
 *
 * Par�metros: Decisor* d: Decisor a destruir.
 *
 * Valor de Retorno: NENHUM */

void destroiDecisor (Decisor* d)
{
    free (d);
}

/*----------------------------------------------------------------------------*/
/** Este decisor n�o faz absolutamente nada quando � informado sobre o conte�do
 * da posi��o atual! Ele simplesmente retorna um movimento aleat�rio.
 *
 * Par�metros: Decisor* d: o Decisor.
 *             Coordenada pos: posi��o atual.
 *             int agua: 1 se a posi��o atual tiver �gua, 0 do contr�rio.
 *             int n_lava: n�mero de po�os de lava em casas vizinhas � atual.
 *
 * Valor de Retorno: a dire��o do pr�ximo movimento: 1 � para cima, 2 � para baixo, 3 � para a esquerda, 4 � para a direita. */

int proximoMovimento (Decisor* d, Coordenada pos, int agua, int n_lava)
{
    return (1+(rand ()%4)); // Retorna um valor aleat�rio entre 1 e 4.
}

/*============================================================================*/
