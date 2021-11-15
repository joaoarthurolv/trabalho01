#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

#define NTHEADS 5

pthread_t threads[NTHEADS];
int status, i;
void *thread_return;

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

}

void* hello_world(void *tid){
    if((int) (size_t)tid > 0){
        pthread_join(threads[(size_t)tid-1], &thread_return);
        printf("Esta é a thread %d. A thread %d terminou", (int)(size_t)tid, (int)(size_t)tid-1);
    }
    else
        printf("Esta é a primeira thread. ");

    pthread_exit(NULL);
}


void produto( int m1_lin, int m1_col, int m2_lin, int m2_col, int m1[m1_lin][m1_col], int m2[m2_lin][m2_col], int m3[m1_lin][m2_col]){
    FILE *pont;

    int aux, i, j, x, t;


    pont = fopen("arq3.txt", "w");

    //Realiza a multiplicacao das matrizes

    for(i = 0; i < m1_lin; i++) {
        for(j = 0; j < m2_col; j++) {
            
            printf("Processo principal criando thread #%d\n", i);
            status = pthread_create(&threads[i], NULL, hello_world, (void*)(size_t) i);

            if(status!=0){
                printf("Erro na criação da thread. Código de Erro:%d\n", status);
            }

            m3[i][j] = 0;
            for(x = 0; x < m2_lin; x++) {
                aux +=  m1[i][x] * m2[x][j];
            }

            m3[i][j] = aux;
            aux = 0;

            printf("Esperando Thread %d finalizar.... \n",i-1);
            pthread_join(threads[i-1], &thread_return);
            printf("thread %d finalizada \n", i-1);
        }
    }
    
    //Insere os valores no arquivo criado
    fprintf(pont, "%d %d \n", m1_lin, m2_col);

    for(i=0;i<m1_lin;i++){
        for(j=0; j<m2_col;j++){
            fprintf(pont, "C%d%d %d\n", i, j, m3[i][j]);       
        }
    }

    fclose(pont);    
}

int main(int argc, char *argv[]){
    FILE *file1,*file2;
    char c, num1[2], num2[2];
    clock_t t;

    char* first_file_name = argv[1];

    file1 = fopen(argv[1], "r");
    file2 = fopen(argv[2], "r");    
    
    int m1_col = contadorColunas(file1);
    int m1_lin = contadorLinhas(file1);

    int m2_col = contadorColunas(file2);
    int m2_lin = contadorLinhas(file2);

    int m1[m1_lin][m1_col];

    int m2[m2_lin][m2_col];

    transformaEmMatriz(m1_lin, m1_col, m1, file1);
    printf("\n");
    transformaEmMatriz(m2_lin, m2_col, m2, file2);

   
    int m3[m1_lin][m2_col];


    if(m1_col == m2_lin){
        t = clock();
        produto( m1_lin, m1_col, m2_lin, m2_col, m1, m2, m3);
        t = clock() - t;
    }else{
        printf("Erro!");
    }

    FILE *teste = fopen("arq3.txt", "a");
    fprintf(teste, "%lf sec\n", ((double)t) / ((CLOCKS_PER_SEC/1000)));
}