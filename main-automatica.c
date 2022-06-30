/*============================================================================*/
/* CAVERNA MARCIANA: TESTE PARA M�DULOS DECISORES                             */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu                                                     */
/*============================================================================*/
/** Explora��o da caverna marciana - vers�o autom�tica.  */
/*============================================================================*/

#include <stdio.h>
#include <stdlib.h>

#include "game_state.h"
#include "t3_v1.h" // TODO: substitua este arquivo pelo seu .h.

/*============================================================================*/

#define PASSO_A_PASSO 0        // Flag que diz se o progresso do rob� ser� mostrado passo-a-passo. Mude para 0 se for rodar muitos testes!
#define CLEAR_SCREEN_CMD "cls" // No Linux, mude para "clear".

// TODO: teste real!
/*
#define RANDOM_SEED_OFFSET 1000

const int altura_caverna [] =
{4, 4, 4, 4,
 5, 5, 5, 5, 5,
 6, 6, 6, 6, 6, 6,
 7, 7, 7, 7, 7, 7, 7, 7,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};

 const int largura_caverna [] =
{4, 4, 5, 6,
 5, 5, 5, 6, 7,
 6, 6, 6, 6, 7, 8,
 7, 7, 7, 7, 7, 7, 8, 9,
 8, 8, 8, 8, 8, 8, 8, 8, 9, 10,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 11,
 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 12};

const int n_pocos [] =
{5, 4, 3, 2,
 10, 9, 8, 7, 6,
 15, 14, 13, 12, 11, 10,
 22, 21, 20, 19, 18, 17, 16, 15,
 29, 28, 27, 26, 25, 24, 23, 22, 21, 20,
 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28,
 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35};

#define N_TIPOS_CAVERNA 57
#define N_JOGOS_POR_TIPO 1
*/
/*
 * Um modificador para a semente usada pelo gerador de n�meros aleat�rios na
 * hora de criar um tabuleiro. Todos os trabalhos usar�o os mesmos tabuleiros,
 * ent�o o offset precisa ser igual para todos. Entretanto, usaremos offsets
 * diferentes na hora de testar os trabalhos de fato. */
#define RANDOM_SEED_OFFSET 13

const int altura_caverna[] =
    {4, 4, 4, 4,
     5, 5, 5, 5, 5,
     6, 6, 6, 6, 6, 6,
     7, 7, 7, 7, 7, 7, 7, 7,
     8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
     9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
     10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};

const int largura_caverna[] =
    {4, 4, 5, 6,
     5, 5, 5, 6, 7,
     6, 6, 6, 6, 7, 8,
     7, 7, 7, 7, 7, 7, 8, 9,
     8, 8, 8, 8, 8, 8, 8, 8, 9, 10,
     9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 11,
     10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 12};

const int n_pocos[] =
    {5, 2, 3, 4,
     7, 2, 3, 4, 5,
     8, 2, 3, 4, 5, 6,
     10, 2, 3, 4, 5, 6, 7, 8,
     15, 2, 3, 4, 5, 6, 7, 8, 9, 10,
     20, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
     25, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

#define N_TIPOS_CAVERNA 57
#define N_JOGOS_POR_TIPO 100

/* Voc� pode substituir as linhas acima por estas, para testar s� uma caverna.
const int altura_caverna [] = {4};
const int largura_caverna [] = {4};
const int n_pocos [] = {3};
#define N_TIPOS_CAVERNA 1
#define N_JOGOS_POR_TIPO 1*/

/*============================================================================*/

int main(int argc, char *argv[])
{
    int tipo_caverna, i;
    int n_rodadas = 0;
    int n_vitorias = 0;
    int n_sem_bateria = 0;

    /* Para cada tipo de caverna... */
    for (tipo_caverna = 0; tipo_caverna < N_TIPOS_CAVERNA; tipo_caverna++)
    {

        int max_rodadas = altura_caverna[tipo_caverna] * largura_caverna[tipo_caverna] * 20;

        /* ... joga N vezes. */
        for (i = 0; i < N_JOGOS_POR_TIPO; i++)
        {
            GameState *game_state;
            Decisor *decisor;
            /* Gera o jogo e o decisor. */
            int seed = argc > 1 ? atoi(argv[1]) : RANDOM_SEED_OFFSET;
            game_state = criaGameState(altura_caverna[tipo_caverna], largura_caverna[tipo_caverna], n_pocos[tipo_caverna], i * tipo_caverna + seed);
            decisor = criaDecisor(altura_caverna[tipo_caverna], largura_caverna[tipo_caverna]);
            /* Loop principal do jogo. */
            while (!jogoTerminado(game_state) && game_state->n_rodadas < max_rodadas)
            {
                int direcao = proximoMovimento(decisor, game_state->pos_atual, posicaoAtualComAgua(game_state), nVizinhosComLava(game_state));
                move(game_state, direcao);

                /* Estas linhas permitem acompanhar o movimento do rob� passo-a-passo. */
                // if (PASSO_A_PASSO)
                // {
                //     system (CLEAR_SCREEN_CMD);
                //     mostraMapaAtual (game_state);
                //     system ("pause");
                // }
            }

            n_rodadas += game_state->n_rodadas;
            if (jogoTerminado(game_state) && !game_state->morreu)
                n_vitorias++;
            else if (game_state->n_rodadas >= max_rodadas)
                n_sem_bateria++;

            destroiDecisor(decisor);
            destroiGameState(game_state);
        }
    }

    /* Resultados finais. */
    printf("Vitorias: %d (%.2f%%)\nQuedas: %d\nSem Bateria: %d\nRodadas: %d\n", n_vitorias, ((float)n_vitorias / (float)(N_TIPOS_CAVERNA * N_JOGOS_POR_TIPO)) * 100.0f, N_TIPOS_CAVERNA * N_JOGOS_POR_TIPO - n_vitorias - n_sem_bateria, n_sem_bateria, n_rodadas);

    return (0);
}

/*============================================================================*/
