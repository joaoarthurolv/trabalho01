#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int contadorColunas(FILE *file){
    char c, fim = '\n', espaco = ' ';
    int col = 0;

    while(fread (&c, sizeof(char), 1, file) && c != fim) {
        if(c == espaco){
            col++;
        }
    }
    return col; 
}

int contadorLinhas(FILE *file){
    char c, fim = '\n', espaco = ' ';
    int linha = 0;

    while(fread (&c, sizeof(char), 1, file)) {
        if(c == fim) {
            linha++;
        }
    }
    return linha + 1;
}

int main(int argc, char *argv[]){
    FILE *file1,*file2;
    char c, num1[2], num2[2];


    file1 = fopen(argv[1], "r");
    file2 = fopen(argv[2], "r");    
    
    int matriz1_colunas = contadorColunas(file1);
    int matriz1_linhas = contadorLinhas(file1);

    int matriz2_colunas = contadorColunas(file2);
    int matriz2_linhas = contadorLinhas(file2);

    int matrizA[matriz1_colunas*matriz1_linhas];
    int matrizB[matriz2_colunas*matriz2_linhas];
    int i=0;

    printf("Matriz 1: Colunas = %d\n", matriz1_colunas);
    printf("Matriz 1: Linhas = %d\n", matriz1_linhas);

    while(fread (&c, sizeof(char), 1, file1)) {
        if(c != ' '){                           // Verifica se o número acabou 
            size_t tamanho = strlen(num1);
            num1[tamanho] = c;
            num1[tamanho + 1] = '\0';
        }else{                                  //Com o espaço identificado, o número que se encontra no vetor num1 será armazenado no vetor (matrizA)
            matrizA[i] = 0;
            matrizA[i] = atoi(num1);
            i++;                                
            memset(num1,0,strlen(num1));        //vetor num1 é zerado
        }
    }

    i=0;
    
    while(fread (&c, sizeof(char), 1, file2)) {
        if(c != ' '){
            size_t tamanho = strlen(num2);
            num2[tamanho] = c;
            num2[tamanho + 1] = '\0';
        }else{
            matrizB[i] = 0;
            matrizB[i] = atoi(num2);
            i++;
            memset(num2,0,strlen(num2));
        }
    }
}