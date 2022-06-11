#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
    #include <Windows.h>
    #define LIMPA "cls"
#else
    #include <unistd.h>
    #define LIMPA "clear"
#endif

#define ORG 'X'
#define VAZ '.'
#define TAM 101
#define TMP_ESPERA 1

typedef struct{
    char nomeJogo[TAM]; //o nome do jogo deve ser relativo ao padrao
    //de inicializacao. Por exemplo, JogoSapo ou JogoBloco
    int ciclosVida; // Define quanto cada jogo vai rodar (ciclos)
    int dim1,dim2; //dimensoes do tabuleiro linhas x colunas
    char **m; //Atenção! Essa matriz terá que ser alocada dinamicamente
    //para que a funcao que inicializa possa funcionar
    char atvInvasoes;
}Tab;

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

char **alocaMatriz(Tab *tabuleiro);
void desalocaMatriz(Tab *tabuleiro);
void limpaMatriz(Tab *tabuleiro);
void inicBlinker(Tab *tabuleiro);
void inicBloco(Tab *tabuleiro);
void inicSapo(Tab *tabuleiro);
void inicGlider(Tab *tabuleiro);
void inicLWSS(Tab *tabuleiro);
void menuInicJogo(Tab *tabuleiro);
void imprimeMatriz(Tab *tabuleiro);
void copiaMatriz(Tab *tabuleiro, char **mAnterior);
void jogaJogoVida(Tab *tabuleiro);
void atualizaMat(Tab *tabuleiro, char **mAnterior);
void vizinhos_possiveis(int *tamanho, int pos_atual,  Tab *tabuleiro, int vizinhos[]);
char calcula_vivo_morto(int pos_possiveis[2][3], int tamanhos[2], char **mAnterior, int pos_atual[2]);
void insereInvasores(Tab *tabuleiro);


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Parte a ser completada //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////



int main()
{
    srand(time(NULL));
    char **mat, jogarNovamente;
    Tab tabuleiro;

    do
    {  
        printf("Digite o numero de linhas da matriz, o numero  de colunas da matriz, e o numero de ciclos, separados por espaços (valores inteiros): ");

        scanf(" %d %d %d", &tabuleiro.dim1, &tabuleiro.dim2, &tabuleiro.ciclosVida);


        tabuleiro.m = alocaMatriz(&tabuleiro);

        menuInicJogo(&tabuleiro);
        jogaJogoVida(&tabuleiro);

        printf("Deseja jogar novamente? Digite qualquer caractere para continuar, ou \"n\"/\"N\" para finalizar: ");
        scanf("%c", &jogarNovamente);
    } while (jogarNovamente != 'n' && jogarNovamente != 'N');

    desalocaMatriz(&tabuleiro);
}

void limpaMatriz(Tab *tabuleiro)
{
    int i,j;
    for(i=0;i<tabuleiro->dim1;i++)
        for(j=0;j<tabuleiro->dim2;j++)
            tabuleiro->m[i][j]=VAZ;
}

void inicBlinker(Tab *tabuleiro)
{
    char padrao[1][3]={{ORG,ORG,ORG}};
    int i,j, xInic=tabuleiro->dim1/2, yInic=tabuleiro->dim2/2;

    limpaMatriz(tabuleiro);

    for(i=0;i<1;i++)
        for(j=0;j<3;j++)
            tabuleiro->m[xInic+i][yInic+j]=padrao[i][j];
}

void inicBloco(Tab *tabuleiro)
{
    char padrao[2][2]={{ORG,ORG},{ORG,ORG}};
    int i,j,xInic=tabuleiro->dim1/2, yInic=tabuleiro->dim2/2;
    limpaMatriz(tabuleiro);

    for(i=0;i<2;i++)
        for(j=0;j<2;j++)
            tabuleiro->m[xInic+i][yInic+j]=padrao[i][j];

}

void inicSapo(Tab *tabuleiro)
{

    char padrao[2][4]={{VAZ,ORG,ORG,ORG},{ORG,ORG,ORG,VAZ}};
    int i,j,xInic=tabuleiro->dim1/2, yInic=tabuleiro->dim2/2;

    limpaMatriz(tabuleiro);

    for(i=0;i<2;i++)
        for(j=0;j<4;j++)
            tabuleiro->m[xInic+i][yInic+j]=padrao[i][j];

}

void inicGlider(Tab *tabuleiro)
{
    char padrao[3][3]={{ORG,ORG,ORG},{ORG,VAZ,VAZ},{VAZ,ORG,VAZ}};
    int i,j,xInic,yInic;

    limpaMatriz(tabuleiro);

    xInic=tabuleiro->dim1-4;
    yInic=tabuleiro->dim2-4;

    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            tabuleiro->m[xInic+i][yInic+j]=padrao[i][j];
}

void inicLWSS(Tab *tabuleiro)
{
    char padrao[4][5]={{VAZ,ORG,VAZ,VAZ,ORG},{ORG,VAZ,VAZ,VAZ,VAZ},{ORG,VAZ,VAZ,VAZ,ORG},{ORG,ORG,ORG,ORG,VAZ}};
    int i,j,xInic,yInic;

    limpaMatriz(tabuleiro);

    xInic=tabuleiro->dim1-5;
    yInic=tabuleiro->dim2-6;

    for(i=0;i<4;i++)
        for(j=0;j<5;j++)
            tabuleiro->m[xInic+i][yInic+j]=padrao[i][j];

}
void menuInicJogo(Tab *tabuleiro)
{
    int opcao;

    printf("(1)Bloco\n(2)Blinker\n(3)Sapo\n(4)Glider\n(5)LWSS\nEntre com a opcao: ");
    scanf("%d",&opcao);
    switch(opcao)
    {
        case 1:   inicBloco(tabuleiro); break;
        case 2:   inicBlinker(tabuleiro); break;
        case 3:   inicSapo(tabuleiro); break;
        case 4:   inicGlider(tabuleiro); break;
        case 5:   inicLWSS(tabuleiro); break;
    }

    printf("Deseja ativar invasões? Se sim, digite \"S\" ou \"s\", caso não, digite qualquer caractere:");
    scanf("%c", tabuleiro->atvInvasoes);


    imprimeMatriz(tabuleiro);

    printf("Se inicializacao correta, digite a tecla ENTER para iniciar o jogo..."); 
    while(getchar()!='\n')
    ; 
    getchar();

}
void jogaJogoVida(Tab *tabuleiro)
{
    char **mAnt;
    int c;
    int cicloInvasao = rand() % (tabuleiro->ciclosVida) + 1;
    
    //imprimindo na tela a matriz inicial
    system(LIMPA);

    imprimeMatriz(tabuleiro);
    // getchar();
    sleep(TMP_ESPERA);

    mAnt = alocaMatriz(tabuleiro);

    for(c=1;c<=tabuleiro->ciclosVida;c++)
    {
        copiaMatriz(tabuleiro, mAnt);

        atualizaMat(tabuleiro, mAnt);//atualizaMat(mAtual,mAnt,nL,nC);
        
        if(c == cicloInvasao) insereInvasores(tabuleiro);
        
        system(LIMPA);
        imprimeMatriz(tabuleiro);
        
        sleep(TMP_ESPERA);
    }

    for (int i = 0; i < tabuleiro->dim1; i++)
        free(mAnt[i]);

    free(mAnt);
}

char **alocaMatriz(Tab *tabuleiro)
{
    char **matriz = (char **) malloc(tabuleiro->dim1 * sizeof(char *));

    for (int i = 0; i < tabuleiro->dim1; i++)
        matriz[i] = (char *) malloc(tabuleiro->dim2 * sizeof(char));


    return matriz;
}

void desalocaMatriz(Tab *tabuleiro)
{
    for (int i = 0; i < tabuleiro->dim1; i++)
        free(tabuleiro->m[i]);

    free(tabuleiro->m);
}

void imprimeMatriz(Tab *tabuleiro)
{
    for (int i = 0; i < tabuleiro->dim1; i++)
    {
        for (int j = 0; j < tabuleiro->dim2; j++)
            printf("%c", tabuleiro->m[i][j]);
        
        printf("\n");
    }
}

void copiaMatriz(Tab *tabuleiro, char **mAnterior)
{
    int l, c;
    for(l = 0; l < tabuleiro->dim1; l++)
        for(c = 0; c < tabuleiro->dim2; c++) mAnterior[l][c] = tabuleiro->m[l][c];
}
void atualizaMat(Tab *tabuleiro, char **mAnterior)
{
    int pos_possiveis[2][3], tamanhos[2], pos_atual[2];

    for (int i = 0; i < tabuleiro->dim1; i++)
    {
        vizinhos_possiveis(&tamanhos[0], i,tabuleiro, pos_possiveis[0]);
        for (int j = 0; j < tabuleiro->dim2; j++)
        {
            vizinhos_possiveis(&tamanhos[1], j, tabuleiro, pos_possiveis[1]);
            pos_atual[0] = i, pos_atual[1] = j;
            tabuleiro->m[i][j] = calcula_vivo_morto(pos_possiveis, tamanhos, mAnterior, pos_atual);
        }
    }
}

void vizinhos_possiveis(int *tamanho, int pos_atual,  Tab *tabuleiro, int vizinhos[])
{
    // coloca em um array as posicoes vizinhas possiveis

    if ((pos_atual - 1) < 0)
        vizinhos[0] = pos_atual,
        vizinhos[1] = pos_atual + 1,
        *tamanho = 2;

    else if ((pos_atual + 1) > (tabuleiro->dim1 - 1))
        vizinhos[0] = pos_atual - 1,
        vizinhos[1] = pos_atual,
        *tamanho = 2;
    
    else
        vizinhos[0] = pos_atual - 1,
        vizinhos[1] = pos_atual,
        vizinhos[2] = pos_atual + 1,
        *tamanho = 3;
}

char calcula_vivo_morto(int pos_possiveis[2][3], int tamanhos[2], char **mAnterior, int pos_atual[2])
{
    /* Uma célula viva morre de solidão se tiver menos de duas vizinhas vivas.
    Uma célula viva morre por superpopulação se tiver mais que três vizinhas vivas.
    Uma célula viva sobrevive se tiver duas ou três vizinhas vivas.
    Uma célula morta ganha vida se tiver exatamente três vizinhas vivas */
    int vivos = 0;

    for (int i = 0; i < tamanhos[0]; i++)
        for (int j = 0; j < tamanhos[1]; j++)
        {
            if (pos_possiveis[0][i] == pos_atual[0] && pos_possiveis[1][j] == pos_atual[1]) continue;
            if (mAnterior[pos_possiveis[0][i]][pos_possiveis[1][j]] == 'X') vivos++;
        }

    if (mAnterior[pos_atual[0]][pos_atual[1]] == 'X')
        return (vivos > 3 || vivos < 2) ? '.' : 'X';

    return vivos == 3 ? 'X' : '.';
    
}

void insereInvasores(Tab *tabuleiro)
{
    int varreLin, varreCol, geraInvasor; // Gera invasores a partir de uma chance (definida como 1 em 5 / 20%), em espaços onde não há um organismo.
    for(varreLin = 0; varreLin < tabuleiro->dim1; varreLin++)
        for(varreCol = 0; varreCol < tabuleiro->dim2; varreCol++)
        {
            if (tabuleiro->m[varreLin][varreCol] == '.')
            {
                geraInvasor = rand() % 5;
                if(geraInvasor == 0) tabuleiro->m[varreLin][varreCol] = 'X';
            }
        }
}