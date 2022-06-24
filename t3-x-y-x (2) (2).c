#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

#define RIGHT 4
#define LEFT 3
#define UP 1
#define DOWN 2

#include "t3-x-y-z.h"
/*--------------------------------------------------------------------------------------------------------------*/
/**                 PROT�TIPOS DAS FUN��ES UTILIZADAS PARA AUXILIAR O PROJETO                                 **/
/*------------------------------------------------------------------------------------------------------------ */
void calculaProb(Decisor* d, Coordenada pos, int n_lava);
void atualizaHistoricoMov(Decisor *d, Coordenada pos);
int procuraAgua(Decisor* d, Coordenada pos, int agua, int n_lava);
int retorna(Decisor* d, Coordenada pos);
Coordenada getMove(Coordenada pos, int direcao);
int posValida(Decisor *d, Coordenada pos);
int getMatrizProb(Decisor *d, Coordenada pos);
void setMatrizProb(Decisor *d, Coordenada pos, int value);
int getMatrizElementos(Decisor *d, Coordenada pos);
void atualizaNumLavas(Decisor *d, Coordenada pos, int n_lava);
void imprimeHist(Decisor *d);
int sumAround(Decisor *d, Coordenada pos, int n_lava);
int movIniciais(Decisor *d);
void setProb(Decisor *d, Coordenada pos, int n_lava, int prob);
void imprimeProb(Decisor *d, Coordenada pos);
int pode_visitar(Decisor *d, Coordenada pos);
int boa_chance(Decisor *d, Coordenada pos);
void imprimeMatrizProb(Decisor *d, Coordenada pos);
int comparaPos(Coordenada c1, Coordenada c2);
void imprimeMatrizElementos(Decisor *d, Coordenada pos);
int decideMovimento(Decisor *d, Coordenada pos);
int analisaRetorno(Decisor *d, Coordenada pos);
/*-------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/**                           FUN��ES DO PROJETO                                     **/
/*------------------------------------------------------------------------------------*/
Decisor* criaDecisor (int altura, int largura)
{
    int i, j;
    Decisor *d;


    d = (Decisor*)malloc(sizeof(Decisor));

    d -> tem_melhor_caminho = 0;
    d -> achou_agua = 0;
    d -> ant.x = 0;
    d -> ant.y = 0;
    d -> num_movimentos = 0;
    d -> altura = altura;
    d -> largura = largura;
    d -> inicio = 3;

    /*  Matriz Elementos */
    /*-----------------------------------------------------------------------------*/
    d -> matrizElementos = (int **) malloc (sizeof (int*) * altura);
    for(i=0; i < altura; i++)
        d -> matrizElementos[i] = (int *) malloc (sizeof (int) * largura);

    for(i=0; i < altura; i++)
        for(j=0; j < largura; j++)
            d -> matrizElementos[i][j] = -1;
    d -> matrizElementos[0][0] = 0;
    /*-----------------------------------------------------------------------------*/

    /*  matriz de probabilidade. */
    /*-----------------------------------------------------------------------------*/
    d -> matrizProb = (int **) malloc (sizeof (int*) * altura);
    for(i=0; i < altura; i++)
        d -> matrizProb[i] = (int *) malloc (sizeof (int) * largura);

    for(i=0; i < altura; i++)
        for(j=0; j < largura; j++)
            d -> matrizProb[i][j] = -1;
    d -> matrizProb[0][0] = 0;

    /*-----------------------------------------------------------------------------*/

    /*  vetor de caminhos  */
    /*-----------------------------------------------------------------------------*/
    d -> vetorCaminho = (int *) malloc (sizeof (int) * (altura * largura * 20));
    /*-----------------------------------------------------------------------------*/

    return d;
}

void destroiDecisor (Decisor* d)
{
    int i;
    /*Desaloca a matriz de elementos. */
    for (i = 0; i < d->altura ; i++)
        free (d -> matrizElementos[i]);
    free (d->matrizElementos );

    /*Desaloca a matriz de probabilidade */
    for (i = 0; i < d->altura; i++)
        free (d -> matrizProb [i]);
    free (d -> matrizProb);

    /*Desalocar o vetor */
    free(d -> vetorCaminho);

    free (d);
}

int proximoMovimento (Decisor* d, Coordenada pos, int agua, int n_lava)
{
    atualizaNumLavas(d, pos, n_lava);
    calculaProb(d, pos, n_lava);// Ainda � necess�rio aprimorar aqui!
    imprimeMatrizElementos(d, pos);
    imprimeMatrizProb(d, pos);
    imprimeHist(d);

    if(d -> inicio){
        d->inicio--;
        atualizaHistoricoMov(d, pos);
        return movIniciais(d);
    }


    if((d->achou_agua) == 0 && (d ->tem_melhor_caminho == 0)){
        atualizaHistoricoMov(d, pos);
    }

    if(agua){
       d -> achou_agua = 1;
    }

    if(d -> achou_agua){
        return retorna(d, pos); // Est� rodando perfeitamente,
    }else
        return procuraAgua(d, pos, agua, n_lava); // Aqui est� com problema.
}
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/**        FUN��ES AUXILIARES USADAS PARA A TOMADA DE DECIS�O DO ROB�                **/
/*------------------------------------------------------------------------------------*/

int procuraAgua(Decisor* d, Coordenada pos, int agua, int n_lava)
{
    if(decideMovimento(d, pos)){
        d -> tem_melhor_caminho = 0;
        printf("decide movimento");
        system("pause");
        return decideMovimento(d, pos);
    }else if(analisaRetorno(d, pos)){
        d -> tem_melhor_caminho = 1;
        printf("retorna");
        system("pause");
        return retorna(d, pos);
    }

    d -> tem_melhor_caminho = 0;
    printf("random");
    system("pause");
    return rand()%4 + 1;
}

int movIniciais(Decisor *d)
{
    if(d -> num_movimentos == 1)
        return RIGHT;
    if(d -> num_movimentos == 2)
        return DOWN;
    if(d -> num_movimentos == 3)
        return LEFT;

    return -1;
}

int retorna(Decisor* d, Coordenada pos)
{
    return d -> vetorCaminho[--d -> num_movimentos];
}

/*-------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/**        FUN��ES QUE PROCESSAM OS DADOS QUE O ROB� RECEBE E POSSUI                 **/
/*------------------------------------------------------------------------------------*/

int analisaRetorno(Decisor *d, Coordenada pos)
{
    int i, j;
    for(i=0; i < d -> altura; i++){
        for(j=0; j < d -> largura; j++){
            if((d -> matrizProb[i][j] == 0) && (d -> matrizElementos[i][j] == -1))
                return 1;
        }
    }
    return 0;
}

void calculaProb(Decisor* d, Coordenada pos, int n_lava)
{
    int sum_around, prob;
    sum_around = sumAround(d, pos, n_lava); // Essa fun��o olha os arredores para calcular a probabilidade de modo adequado.

    if(sum_around)
        prob = ((float)(n_lava)/(float)(sum_around)) * 100;
    else
        prob = 0;

    setProb(d, pos, n_lava, prob);

}

void setProb(Decisor *d, Coordenada pos, int n_lava, int prob)
{
    if(getMatrizElementos(d, getMove(pos, RIGHT)) == -1)
        setMatrizProb(d, getMove(pos, RIGHT), prob);
    else
        setMatrizProb(d, getMove(pos, RIGHT), 0);
    if(getMatrizElementos(d, getMove(pos, LEFT)) == -1)
        setMatrizProb(d, getMove(pos, LEFT), prob);
    else
        setMatrizProb(d, getMove(pos, LEFT), 0);
    if(getMatrizElementos(d, getMove(pos, UP)) == -1)
        setMatrizProb(d, getMove(pos, UP), prob);
    else
        setMatrizProb(d, getMove(pos, UP), 0);
    if(getMatrizElementos(d, getMove(pos, DOWN)) == -1)
        setMatrizProb(d, getMove(pos, DOWN), prob);
    else
        setMatrizProb(d, getMove(pos, DOWN), 0);
}

int sumAround(Decisor *d, Coordenada pos, int n_lava)
{
    int right = 0, left = 0, up = 0 , down = 0;
    if(n_lava){
        if(getMatrizElementos(d, getMove(pos, RIGHT)) == -1) // pos n�o explorada
            right = 1;
        if(getMatrizElementos(d, getMove(pos, LEFT)) == -1) // pos n�o explorada
            left = 1;
        if(getMatrizElementos(d, getMove(pos, UP)) == -1) // pos n�o explorada
            up = 1;
        if(getMatrizElementos(d, getMove(pos, DOWN)) == -1) // pos n�o explorada
            down = 1;
    }
    //printf(" ->right:%d left:%d up:%d down:%d<- ", right, left, up, down);
    //system("pause");
    return (right + left + up + down);
}

void atualizaHistoricoMov(Decisor *d, Coordenada pos)
{
    if(comparaPos(getMove(d -> ant, RIGHT), pos))
        d -> vetorCaminho[d->num_movimentos] = LEFT;
    else if (comparaPos(getMove(d -> ant, LEFT), pos))
        d -> vetorCaminho[d->num_movimentos] = RIGHT;
    else if (comparaPos(getMove(d -> ant, UP), pos))
        d -> vetorCaminho[d->num_movimentos] = DOWN;
    else if (comparaPos(getMove(d -> ant, DOWN), pos))
        d -> vetorCaminho[d->num_movimentos] = UP;
    else
        d -> vetorCaminho[d->num_movimentos] = -1;
    d -> ant = pos;
    d -> num_movimentos++;
}

int pode_visitar(Decisor *d, Coordenada pos)
{
    if((getMatrizProb(d, pos) == 0) && (getMatrizElementos(d, pos) == -1))
        return 1;
    return 0;
}

int decideMovimento(Decisor *d, Coordenada pos)
{
    int i;
    for(i=1; i<=4; i++){
        if(pode_visitar(d, getMove(pos, i)))
            return i;
    }
    return 0;
}

/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
/**                     FUN��ES PARA SIMPLIFICAR OPERA��ES USADAS NO PROJETO                     **/
/*------------------------------------------------------------------------------------------------*/
Coordenada getMove(Coordenada pos, int direcao) //retorna a coordenada da posi��o em uma dire��o.
{
    Coordenada movimento;
    if(direcao == RIGHT){
        movimento.x = pos.x + 1;
        movimento.y = pos.y;
    }
    if(direcao == LEFT){
        movimento.x = pos.x - 1;
        movimento.y = pos.y;
    }
    if(direcao == UP){
        movimento.x = pos.x;
        movimento.y = pos.y - 1;
    }
    if(direcao == DOWN){
        movimento.x = pos.x;
        movimento.y = pos.y + 1;
    }
    return movimento;
}

int posValida(Decisor *d, Coordenada pos)//verifica se uma posi��o da matriz � v�lida.
{
    if(((pos.x < d -> largura) && (pos.x >= 0)) && ((pos.y < d -> altura) && (pos.y >= 0)))
        return 1;
    else
        return 0;
}

int getMatrizProb(Decisor *d, Coordenada pos)//pega o valor em uma coordenada da vari�vel "matrizProb" declarada na STRUCT DECISOR.
{
    if(!posValida(d, pos)) return 1234;
    return d->matrizProb[pos.y][pos.x];
}

void setMatrizProb(Decisor *d, Coordenada pos, int value)//coloca um valor em uma coordenada da vari�vel "matrizProb" declarada na STRUCT DECISOR.
{
    if(!posValida(d, pos)) return ;
    d->matrizProb[pos.y][pos.x] = value;
}

int getMatrizElementos(Decisor *d, Coordenada pos) //pega o valor em uma coordenada da vari�vel "matrizElementos" declarada na STRUCT DECISOR.
{
    if(!posValida(d, pos)) return 1234;
    return d->matrizElementos[pos.y][pos.x];
}

void atualizaNumLavas(Decisor *d, Coordenada pos, int n_lava)//coloca um valor em uma coordenada da vari�vel "matrizElementos" declarada na STRUCT DECISOR.
{
    if(!posValida(d, pos)) return  ;
    d->matrizElementos[pos.y][pos.x] = n_lava;
}

int comparaPos(Coordenada c1, Coordenada c2)
{
    if(c1.x == c2.x && c1.y == c2.y)
        return 1;
    else
        return 0;
}
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
/**                         FUN��ES PARA DEBUGGAR O C�DIGO                                       **/
/*------------------------------------------------------------------------------------------------*/

void imprimeProb(Decisor *d, Coordenada pos)
{
    printf("prob direita: %d ", getMatrizProb(d, getMove(pos, RIGHT)));
    printf("prob esquerda: %d ", getMatrizProb(d, getMove(pos, LEFT)));
    printf("prob cima: %d ", getMatrizProb(d, getMove(pos, UP)));
    printf("prob baixo: %d ", getMatrizProb(d, getMove(pos, DOWN)));

    system("pause");
}

void imprimeHist(Decisor *d)
{
    int i;
    for(i=0; i < d -> num_movimentos; i++)
        printf("%d ", d -> vetorCaminho[i]);
    printf("\n");
    system("pause");
}

void imprimeMatrizProb(Decisor *d, Coordenada pos)
{
    int i, j;
    for(i = 0; i < d -> altura; i++){
        for(j=0; j < d -> largura; j++){
            if(d -> matrizProb[i][j] > 0)
                printf("+%d ", d -> matrizProb[i][j]);
            else
                printf("%d ", d -> matrizProb[i][j]);
        }
        printf("\n");
    }
    system("pause");
}

void imprimeMatrizElementos(Decisor *d, Coordenada pos)
{
    int i, j;
    for(i = 0; i < d -> altura; i++){
        for(j=0; j < d -> largura; j++){
            if(d -> matrizElementos[i][j] > 0)
                printf("%d ", d -> matrizElementos[i][j]);
            else
                printf("%d ", d -> matrizElementos[i][j]);
        }
        printf("\n");
    }
    system("pause");
}

/*------------------------------------------------------------------------------------------------*/
