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
    return col + 1; 
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

void transformaEmMatriz(int linhas, int colunas, int m[linhas][colunas], FILE *file){
    rewind(file);
    char c;
    int qtd = 0, lin = 0, col = 0;
    char current[100];
    memset(current, 0, 100);

    while(fread (&c, sizeof(char), 1, file)) {
        if(c == ' '){
            m[lin][col] = atoi(current);
            col++;
            memset(current, 0, 100);
        } 
        else if(c == '\n'){
            m[lin][col] = atoi(current);
            lin++;
            col = 0;
            memset(current, 0, 100);
        }
        else {
            strcat(current, &c);
        }
    }
    printf("%d\n", m[0][1]);

    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            if(j == (colunas - 1))
                printf("%d\n", m[i][j]);
            else
                printf("%d ", m[i][j]);
        }
    }
}

int main(int argc, char *argv[]){
    FILE *file1,*file2;
    char c, num1[2], num2[2];

    char* first_file_name = argv[1];

    file1 = fopen(argv[1], "r");
    file2 = fopen(argv[2], "r");    
    
    int m1_col = contadorColunas(file1);
    int m1_lin = contadorLinhas(file1);

    int m1[m1_lin][m1_col];

    transformaEmMatriz(m1_lin, m1_col, m1, file1);

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