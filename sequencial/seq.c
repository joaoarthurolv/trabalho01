#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

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
    char current[100] = "";
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
            strncat(current, &c, 1);
        }
    }

    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            if(j == (colunas - 1))
                printf("%d\n", m[i][j]);
            else
                printf("%d ", m[i][j]);
        }
    }
}



void produto(struct timeval *inicio, int m1_lin, int m1_col, int m2_lin, int m2_col, int m1[m1_lin][m1_col], int m2[m2_lin][m2_col], int m3[m1_lin][m2_col]){
    FILE *pont;

    int aux, i, j, x;


    pont = fopen("arq3.txt", "w");

    //Realiza a multiplicacao das matrizes

    for(i = 0; i < m1_lin; i++) {
        for(j = 0; j < m2_col; j++) {
            
            m3[i][j] = 0;
            for(x = 0; x < m2_lin; x++) {
                aux +=  m1[i][x] * m2[x][j];
            }

            m3[i][j] = aux;
            aux = 0;
        }
    }
    
    struct timeval fim;
    gettimeofday(&fim, NULL);

    //Insere os valores no arquivo criado
    fprintf(pont, "%d %d \n", m1_lin, m2_col);

    for(i=0;i<m1_lin;i++){
        for(j=0; j<m2_col;j++){
            fprintf(pont, "C%d%d %d\n", i, j, m3[i][j]);       
        }
    }

    // fprintf(pont, "%0.8f sec\n", &fim-&inicio);
    fclose(pont);

    printf("Resultado da multuplicacao:\n");

    for(i = 0; i < m1_lin; i++) {
        for(j = 0; j < m2_col; j++) {
            printf("%d ", m3[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[]){
    FILE *file1,*file2;
    char c, num1[2], num2[2];
    clock_t t;

    char* first_file_name = argv[1];

    file1 = fopen(argv[1], "r");
    file2 = fopen(argv[2], "r");

    int p = atoi(argv[3]);    
    
    int m1_col = contadorColunas(file1);
    int m1_lin = contadorLinhas(file1);

    int m2_col = contadorColunas(file2);
    int m2_lin = contadorLinhas(file2);

    int m1[m1_lin][m1_col];

    int m2[m2_lin][m2_col];

    transformaEmMatriz(m1_lin, m1_col, m1, file1);
    printf("\n");
    transformaEmMatriz(m2_lin, m2_col, m2, file2);

    

}