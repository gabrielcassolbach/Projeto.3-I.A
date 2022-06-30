/*----------------------------------------------------------------------------------------------------------------
AUTORES: Gabriel Cassol Bach - 2129035
         Ian Massanobu Santos Ishikawa - 2449072
         Vitor Boechat Erreira - 2449153
------------------------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>

#define RIGHT 4
#define LEFT 3
#define UP 1
#define DOWN 2

#include "t3-2449072-2129035-2449153.h"
/*--------------------------------------------------------------------------------------------------------------*/
/**                 PROT�TIPOS DAS FUN��ES UTILIZADAS PARA AUXILIAR O PROJETO                                 **/
/*------------------------------------------------------------------------------------------------------------ */
void calculaProb(Decisor* d, Coordenada pos, int n_lava);
void atualizaHistoricoMov(Decisor *d, Coordenada pos);
int procuraAgua(Decisor* d, Coordenada pos, int agua, int n_lava);
int retorna(Decisor* d);
Coordenada getMove(Coordenada pos, int direcao);
int posValida(Decisor *d, Coordenada pos);
int getMatrizProb(Decisor *d, Coordenada pos);
void setMatrizProb(Decisor *d, Coordenada pos, int value);
int getMatrizElementos(Decisor *d, Coordenada pos);
void atualizaNumLavas(Decisor *d, Coordenada pos, int n_lava);
int sumAround(Decisor *d, Coordenada pos, int n_lava);
int movIniciais(Decisor *d);
void setProb(Decisor *d, Coordenada pos, int prob);
int pode_visitar(Decisor *d, Coordenada pos);
int comparaPos(Coordenada c1, Coordenada c2);
int decideMovimento(Decisor *d, Coordenada pos);
int analisaRetorno(Decisor *d);
int tresLivre (Decisor* d, Coordenada pos);
int cemAoRedor(Decisor* d, Coordenada pos);
int movimentoMedio(Decisor *d, Coordenada pos);
int boaChance(Decisor *d, Coordenada pos);
/*-------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/**                           FUN��ES DO PROJETO                                     **/
/*------------------------------------------------------------------------------------*/
Decisor* criaDecisor (int altura, int largura)
{
    int i, j;
    Decisor *d;


    d = (Decisor*)malloc(sizeof(Decisor));

    d -> voltou = 0;
    d -> achou_agua = 0;
    d -> ant.x = 0;
    d -> ant.y = 0;
    d -> num_movimentos = 0;
    d -> altura = altura;
    d -> largura = largura;
    d -> inicio = 3;

    /*  Matriz Elementos */
    /* Matriz que guarda o que foi explorado e o que n�o foi explorado no mapa.*/
    /*-----------------------------------------------------------------------------*/
    d -> matrizElementos = (int **) malloc (sizeof (int*) * altura);
    for(i=0; i < altura; i++)
        d -> matrizElementos[i] = (int *) malloc (sizeof (int) * largura);

    for(i=0; i < altura; i++)
        for(j=0; j < largura; j++)
            d -> matrizElementos[i][j] = -1;
    d -> matrizElementos[0][0] = 0;
    /*-----------------------------------------------------------------------------*/

    /*  Matriz de probabilidade. */
    /* Matriz que guarda a probabilidade de se ter lava em cada ponto da caverna */
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
    /*A cada passo do rob�, esse vetor guarda o passo contr�rio.
    Se o rob� vai para a direita ele guarda a esquerda. Cima -> baixo... e assim por diante.*/
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

    /*Desaloca o vetor */
    free(d -> vetorCaminho);

    free (d);
}

/*  Retorna o pr�ximo movimento a ser realizado pelo rob�.
    Para isso, primeiro atualiza-se a situa��o do rob� no mapa,
    verificando se h� lavas ao redor e vendo qual a probabilidade da posi��o de cada uma das lavas.
    Ap�s atualizar a situa��o no mapa, � calculado o pr�ximo movimento.
    No in�cio s�o feitos os 3 movimentos seguros para obter informa��es do mapa sem correr perigo.
    Ap�s isso, � verificado se a �gua j� foi encontrada. Se sim, � realizado um movimento de retorno para o ponto (0,0).
    Sen�o, � realizado um movimento para procurar �gua. */
int proximoMovimento (Decisor* d, Coordenada pos, int agua, int n_lava)
{
    atualizaNumLavas(d, pos, n_lava);
    calculaProb(d, pos, n_lava);

    if(d -> inicio){
        (d->inicio)--;
        atualizaHistoricoMov(d, pos);
        d -> ant = pos;
        return movIniciais(d);
    }

    if(d -> voltou == 0){
        atualizaHistoricoMov(d, pos);
    }

    d -> voltou = 0;

    if(agua)
       d -> achou_agua = 1;

    d -> ant = pos;

    if(d -> achou_agua){
        return retorna(d);
    }else
        return procuraAgua(d, pos, agua, n_lava);
}
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/**        FUN��ES AUXILIARES USADAS PARA A TOMADA DE DECIS�O DO ROB�                **/
/*------------------------------------------------------------------------------------*/

/*  Fun��o que retorna os movimentos a serem realizados para buscar por �gua.
    Primeiro, � verificado se h� um movimento 100% seguro para ser feito.
    Se n�o houver, � verificado se h� um movimento 100% seguro pelo caminho que o rob� j� percorreu.
    Se houver algum movimento seguro, ele retorna para o ponto com o movimento seguro e o realiza.
    Sen�o, � verificado se h� algum movimento com m�dia probabilidade de ser seguro. Se houver, esse movimento
    � realizado, sen�o, � realizado um movimento aleat�rio. */
int procuraAgua(Decisor* d, Coordenada pos, int agua, int n_lava)
{
    if(decideMovimento(d, pos))
        return decideMovimento(d, pos);
    else if(analisaRetorno(d))
        return retorna(d);
    else if(movimentoMedio(d, pos))
        return movimentoMedio(d, pos);
    else
        return rand()%4 +1;
    return -1;
}

/* Realiza os movimentos iniciais nas posi��es ao redor de (0,0)*/
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

/*Utiliza o conceito de "pilha" para desfazer os movimentos, retornando pelo percurso de ida.*/
int retorna(Decisor* d)
{
    d -> voltou = 1;
    return d -> vetorCaminho[--d -> num_movimentos];
}

/*-------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/**     FUN��ES AUXILIARES QUE PROCESSAM OS DADOS QUE O ROB� RECEBE E POSSUI          **/
/*------------------------------------------------------------------------------------*/

/*fun��o para verificar se tr�s casas ao redor da posi��o atual est�o livres.*/
int tresLivre (Decisor* d, Coordenada pos)
{
    int soma;

    soma = getMatrizProb(d, getMove(pos, UP))+getMatrizProb(d, getMove(pos, DOWN))+getMatrizProb(d, getMove(pos, LEFT))+getMatrizProb(d, getMove(pos, RIGHT));

    if (!posValida(d, getMove(pos, UP)) && getMatrizProb(d, getMove(pos, RIGHT))==0 && getMatrizProb(d, getMove(pos, LEFT))==0)
        return 1;
    else if (!posValida(d, getMove(pos, LEFT)) && getMatrizProb(d, getMove(pos, UP))==0 && getMatrizProb(d, getMove(pos, DOWN))==0)
        return 1;
    else if (!posValida(d, getMove(pos, DOWN)) && getMatrizProb(d, getMove(pos, RIGHT))==0 && getMatrizProb(d, getMove(pos, LEFT))==0)
        return 1;
    else if (!posValida(d, getMove(pos, RIGHT)) && getMatrizProb(d, getMove(pos, UP))==0 && getMatrizProb(d, getMove(pos, LEFT))==0)
        return 1;
    else if(soma==-1)
        return 1;
    else
        return 0;
}

/*verifica se uma posi��o adjacente tem probabilidade 0% ou 100%*/
int zeroOuCem (Decisor* d, Coordenada pos)
{
    if (getMatrizProb(d, pos)==0 || getMatrizProb(d, pos)>=100)
        return 1;
    else
        return 0;
}

/*verifica se alguma das 4 casas adjacente possui probabilidade 100%*/
int cemAoRedor(Decisor* d, Coordenada pos)
{
    if (getMatrizProb(d, getMove(pos,UP))== 100)
        return 1;
    else if(getMatrizProb(d, getMove(pos,DOWN))== 100)
        return 1;
    else if (getMatrizProb(d, getMove(pos,RIGHT))== 100)
        return 1;
    else if (getMatrizProb(d, getMove(pos,LEFT))== 100)
        return 1;
    else
        return 0;
}

/*Verifica se h� alguma posi��o do mapa que ainda n�o foi explorada e que tem-se certeza de que n�o h� lava nela.*/
int analisaRetorno(Decisor *d)
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

/*Fun��o respons�vel por calcular a probabilidade que ser� usada na matriz de probabilidade.*/
void calculaProb(Decisor* d, Coordenada pos, int n_lava)
{
    int sum_around, prob;
    sum_around = sumAround(d, pos, n_lava);

    if (n_lava==1 && cemAoRedor(d, pos)) //condi��o quando se tem certeza de 3 casas com probabilidade 0
        prob = 0;
    else if ((n_lava==1) && tresLivre(d, pos)) //condi��o quando se tem certeza de 1 casa com probabilidade 100
        prob = 100;
    else if(sum_around)
        prob = ((float)(n_lava)/(float)(sum_around)) * 100; //c�lculo da probabilidade
    else
        prob = 0;

    setProb(d, pos, prob);

}

/*Preenche a matriz de probabilidade.*/
void setProb(Decisor *d, Coordenada pos, int prob)
{
    if(getMatrizElementos(d, getMove(pos, RIGHT)) == -1 && zeroOuCem(d,getMove(pos, RIGHT)) == 0)
        setMatrizProb(d, getMove(pos, RIGHT), prob);
    else if(zeroOuCem(d,getMove(pos, RIGHT))== 0)
        setMatrizProb(d, getMove(pos, RIGHT), 0);
    if(getMatrizElementos(d, getMove(pos, LEFT)) == -1 && zeroOuCem(d,getMove(pos, LEFT))== 0)
        setMatrizProb(d, getMove(pos, LEFT), prob);
    else if (zeroOuCem(d,getMove(pos, LEFT))== 0)
        setMatrizProb(d, getMove(pos, LEFT), 0);
    if(getMatrizElementos(d, getMove(pos, UP)) == -1 && zeroOuCem(d,getMove(pos, UP))== 0)
        setMatrizProb(d, getMove(pos, UP), prob);
    else if (zeroOuCem(d,getMove(pos, UP))== 0)
        setMatrizProb(d, getMove(pos, UP), 0);
    if(getMatrizElementos(d, getMove(pos, DOWN)) == -1 && zeroOuCem(d,getMove(pos, DOWN))== 0)
        setMatrizProb(d, getMove(pos, DOWN), prob);
    else if (zeroOuCem(d,getMove(pos, DOWN))== 0)
        setMatrizProb(d, getMove(pos, DOWN), 0);
}

/* Essa fun��o olha os arredores para calcular a probabilidade de modo adequado. */
int sumAround(Decisor *d, Coordenada pos, int n_lava)
{
    int right = 0, left = 0, up = 0 , down = 0;
    if(n_lava){
        if(getMatrizElementos(d, getMove(pos, RIGHT)) == -1 && zeroOuCem(d,getMove(pos, RIGHT))== 0) // pos n�o explorada
            right = 1;
        if(getMatrizElementos(d, getMove(pos, LEFT)) == -1 && zeroOuCem(d,getMove(pos, LEFT))== 0) // pos n�o explorada
            left = 1;
        if(getMatrizElementos(d, getMove(pos, UP)) == -1 && zeroOuCem(d,getMove(pos, UP))== 0) // pos n�o explorada
            up = 1;
        if(getMatrizElementos(d, getMove(pos, DOWN)) == -1 && zeroOuCem(d,getMove(pos, DOWN))== 0) // pos n�o explorada
            down = 1;
    }
    return (right + left + up + down);
}

/*Verifica qual foi o �ltimo movimento realizado e salva-o no "hist�rico"*/
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
    d -> num_movimentos++;
}

/* Verifica se a posi��o ainda n�o foi visitada e se � 100% seguro visit�-la*/
int pode_visitar(Decisor *d, Coordenada pos)
{
    if((getMatrizProb(d, pos) == 0) && (getMatrizElementos(d, pos) == -1))
        return 1;
    return 0;
}

/*Verifica se, partindo da posi��o especificada, h� algum movimento com 100% de chance de ser seguro*/
int decideMovimento(Decisor *d, Coordenada pos)
{
    int i;
    for(i=1; i<=4; i++){
        if(pode_visitar(d, getMove(pos, i)))
            return i;
    }
    return 0;
}

/* Verifica se partindo da posi��o especificada h� algum movimento com boa chance de ser seguro */
int movimentoMedio(Decisor *d, Coordenada pos)
{
    int i;
    for (i = 1; i <= 4; i++)
    {
        if (boaChance(d, getMove(pos, i)))
            return i;
    }
    return 0;
}

/* Verifica se a posi��o ainda n�o foi visitada e se h� uma boa chance(>=50%) de ser seguro visit�-la */
int boaChance(Decisor *d, Coordenada pos)
{
    if ((getMatrizProb(d, pos) <= 50) && (getMatrizElementos(d, pos) == -1))
        return 1;
    return 0;
}

/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
/**               FUN��ES AUXILIARES PARA SIMPLIFICAR OPERA��ES USADAS NO PROJETO                **/
/*------------------------------------------------------------------------------------------------*/
/*retorna a coordenada da posi��o em uma dire��o.*/
Coordenada getMove(Coordenada pos, int direcao)
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

/*verifica se uma posi��o da matriz � v�lida.*/
int posValida(Decisor *d, Coordenada pos)
{
    if(((pos.x < d -> largura) && (pos.x >= 0)) && ((pos.y < d -> altura) && (pos.y >= 0)))
        return 1;
    else
        return 0;
}

/*pega o valor em uma coordenada da vari�vel "matrizProb" declarada na STRUCT DECISOR.*/
int getMatrizProb(Decisor *d, Coordenada pos)
{
    if(!posValida(d, pos)) return 1234;
    return d->matrizProb[pos.y][pos.x];
}

/*coloca um valor em uma coordenada da vari�vel "matrizProb" declarada na STRUCT DECISOR.*/
void setMatrizProb(Decisor *d, Coordenada pos, int value)
{
    if(!posValida(d, pos)) return ;
    d->matrizProb[pos.y][pos.x] = value;
}

/*pega o valor em uma coordenada da vari�vel "matrizElementos" declarada na STRUCT DECISOR.*/
int getMatrizElementos(Decisor *d, Coordenada pos)
{
    if(!posValida(d, pos)) return 1234;
    return d->matrizElementos[pos.y][pos.x];
}

/*coloca um valor em uma coordenada da vari�vel "matrizElementos" declarada na STRUCT DECISOR.*/
void atualizaNumLavas(Decisor *d, Coordenada pos, int n_lava)
{

    if(!posValida(d, pos)) return  ;
    d->matrizElementos[pos.y][pos.x] = n_lava;
}
/*Compara duas coordenadas.*/
int comparaPos(Coordenada c1, Coordenada c2)
{
    if(c1.x == c2.x && c1.y == c2.y)
        return 1;
    else
        return 0;
}
/*------------------------------------------------------------------------------------------------*/

