#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

typedef struct {
    char nomeJogo[TAM]; //o nome do jogo deve ser relativo ao padrao
    //de inicializacao. Por exemplo, JogoSapo ou JogoBloco
    int ciclosVida; // Define quanto cada jogo vai rodar (ciclos)
    int dim1,dim2; //dimensoes do tabuleiro linhas x colunas
    char **m;
    char atvInvasoes;
} Tab;

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

char **alocaMatriz(Tab *tabuleiro);
void desalocaMatriz(Tab *tabuleiro);
void limpaMatriz(Tab *tabuleiro);
void menuInicJogo(Tab *tabuleiro);
void imprimeMatriz(Tab *tabuleiro);
void copiaMatriz(Tab *tabuleiro, char **mAnterior);
void jogaJogoVida(Tab *tabuleiro);
void atualizaMat(Tab *tabuleiro, char **mAnterior);
void vizinhos_possiveis(int *tamanho, int pos_atual,  Tab *tabuleiro, int vizinhos[]);
char calcula_vivo_morto(int pos_possiveis[2][3], int tamanhos[2], char **mAnterior, int pos_atual[2]);
void monta_arquivo(Tab *tabuleiro);
void insereInvasores(Tab *tabuleiro);


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////



int main()
{
    srand(time(NULL));
    char **mat, jogarNovamente;
    Tab tabuleiro;

    do
    {  
        printf("Digite o numero de linhas da matriz, o numero  de colunas da matriz, e o numero de ciclos, separados por espacos (valores inteiros): ");

        scanf(" %d %d %d", &tabuleiro.dim1, &tabuleiro.dim2, &tabuleiro.ciclosVida);

        tabuleiro.m = alocaMatriz(&tabuleiro);

        menuInicJogo(&tabuleiro);
        jogaJogoVida(&tabuleiro);

        printf("Modo de jogo: %s\n", tabuleiro.nomeJogo);
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


void menuInicJogo(Tab *tabuleiro)
{
    int opcao;

    printf("(1)Bloco\n(2)Blinker\n(3)Sapo\n(4)Glider\n(5)LWSS\n(6)Customizado\nEntre com a opcao: ");
    scanf("%d", &opcao);

    switch (opcao)
    {
        case 1:
            sprintf(tabuleiro->nomeJogo, "Bloco"); break;

        case 2:
            sprintf(tabuleiro->nomeJogo, "Blinker"); break;

        case 3:
            sprintf(tabuleiro->nomeJogo, "Sapo"); break;

        case 4:
            sprintf(tabuleiro->nomeJogo, "Glider"); break;

        case 5:
            sprintf(tabuleiro->nomeJogo, "LWSS"); break;

        case 6:
            printf("Por favor, coloque seu arquivo csv customizado no diretorio \"iniciacoes\" e digite *apenas* seu nome (sem a extensão .csv): ");
            getchar(); // limpando o stdin
            scanf("%s", tabuleiro->nomeJogo); break;

        default:
            printf("Opcao invalida! Saindo...\n"); 
            exit(1);
    }

    printf("Deseja ativar invasoes? Se sim, digite \"S\" ou \"s\", caso nao, digite qualquer caractere: ");
    scanf(" %c", &tabuleiro->atvInvasoes);

    limpaMatriz(tabuleiro);
    monta_arquivo(tabuleiro);
    imprimeMatriz(tabuleiro);

    printf("Se inicializacao correta, digite a tecla ENTER para iniciar o jogo..."); 
    while(getchar()!='\n')
    ; 
    getchar();

}

void jogaJogoVida(Tab *tabuleiro)
{
    char **mAnt;
    int c, cicloInvasao = rand() % (tabuleiro->ciclosVida) + 1;
    
    system(LIMPA);

    imprimeMatriz(tabuleiro);
    sleep(TMP_ESPERA);

    mAnt = alocaMatriz(tabuleiro);

    for(c=1;c<=tabuleiro->ciclosVida;c++)
    {
        copiaMatriz(tabuleiro, mAnt);

        atualizaMat(tabuleiro, mAnt);
        
        if ((tabuleiro->atvInvasoes == 'S' || tabuleiro->atvInvasoes == 's') && c == cicloInvasao) insereInvasores(tabuleiro);
        
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
void monta_arquivo(Tab *tabuleiro) // Lê o arquivo csv e gera o padrão a partir dele.
{
    char path[100], c;
    int linha=0;

    sprintf(path, "iniciacoes/%s.csv", tabuleiro->nomeJogo);

    FILE *arquivo = fopen(path, "r");
    if (arquivo == NULL)
    {
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }

    do
    {
        // Posição do organismo determinada pela linha em que está o caractere no .csv (linha), e pelo inteiro ao qual o caractere remete (coluna).
        c = fgetc(arquivo);
        while(c!='\n' && c != EOF){
        
            if(c!=',')
                tabuleiro->m[linha][atoi(&c)-1]=ORG;
                
            c = fgetc(arquivo);
        }
        linha+=1;
    } while (c != EOF);
}

void insereInvasores(Tab *tabuleiro)
{
    int varreLin, varreCol, geraInvasor; // Gera invasores a partir de uma chance (definida como 1 em 10 / 10%), em espaços onde não há um organismo.
    
    for(varreLin = 0; varreLin < tabuleiro->dim1; varreLin++)
        for(varreCol = 0; varreCol < tabuleiro->dim2; varreCol++)
            if (tabuleiro->m[varreLin][varreCol] == '.')
            {
                geraInvasor = rand() % 10;
                if (geraInvasor == 0) tabuleiro->m[varreLin][varreCol] = 'X';
            }
}
