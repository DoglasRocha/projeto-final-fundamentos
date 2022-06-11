#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 1024



int main(){

    FILE *arquivo = fopen("sla.csv", "r");
    char c;
    if (arquivo == NULL)
    {
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }
    rewind(arquivo);

    do
{
    //faz a leitura do caracter no arquivo apontado por pont_arq
    c = fgetc(arquivo);
    
    //exibe o caracter lido na tela
    printf("%c" , c);    
    if(c=='\n'){
        printf("\npulou\n");
    }
    
}while (c != EOF);
}