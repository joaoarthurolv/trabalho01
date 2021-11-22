#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

int status;
void *thread_return;
pthread_t *threads;
char filename[20];

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
    if((int)(size_t)tid > 0){
        pthread_join(threads[(size_t)tid-1], &thread_return);
        printf("Esta é a thread %d. A thread %d terminou \n", (int)(size_t)tid, (int)(size_t)tid-1);

    }
    else
        printf("Esta é a primeira thread.\n");
    pthread_exit(NULL);

}



void produto(int p, int m1_lin, int m1_col, int m2_lin, int m2_col, int m1[m1_lin][m1_col], int m2[m2_lin][m2_col]){
    int i, j, x, aux, a = 1, t = 0, time = 0;
    int m3[m1_lin][m2_col];

    int numElementos = m1_lin*m2_col;
    int qtd_t = ceil(numElementos/p);
    clock_t tempo[qtd_t];
    clock_t fullTime;
    FILE *pont[qtd_t+1];
    double tempos[qtd_t+1];

    for(i = 0; i<=qtd_t; i++){
        snprintf(filename, 10, "arq%d.txt", i+3);
        pont[i] = fopen(filename, "w");
        printf("%d\n",i);
    }

    threads = (pthread_t *) malloc(qtd_t * sizeof(pthread_t));

    tempo[0] = clock(); //Iniciando contagem
    fullTime = clock();
    printf("Processo principal criando thread #0 \n");
    status = pthread_create(&threads[t], NULL, hello_world, (void*)(size_t) t);
    usleep(1);
    if(status!=0)
        printf("Erro na criação da thread. Código de Erro:%d\n", status);




    t++;

    //Realiza a multiplicacao das matrizes

    for(i = 0; i < m1_lin; i++) {
        for(j = 0; j < m2_col; j++) {

            if(a==p && numElementos != p){
                tempo[time] = clock() - tempo[time];//Fim da contagem
                tempos[time] = ((double)tempo[time]) / ((CLOCKS_PER_SEC/1000));
                time++;
                tempo[time] = clock();//Inicio da contagem

                printf("Processo principal criando thread #%d\n", t);
                status = pthread_create(&threads[t], NULL, hello_world, (void*)(size_t) t);
                if(status!=0){
                    printf("Erro na criação da thread. Código de Erro:%d\n", status);
                }

                t++;

                if(numElementos%p==0 && p != 1){
                    a=0;
                }else{
                  a=1;
                }

            }else{
                a++;
            }


            m3[i][j] = 0;
            for(x = 0; x < m2_lin; x++) {
                aux +=  m1[i][x] * m2[x][j];
            }

            m3[i][j] = aux;
            aux = 0;

        }
    }

    printf("Esperando Thread %d finalizar.... \n",t-1);
    pthread_join(threads[t-1], &thread_return);
    printf("thread %d finalizada \n", t-1);




    tempo[time] = clock() - tempo[time];//Fim da contagem
    fullTime = clock() - fullTime;

    tempos[time]=((double)tempo[time]) / ((CLOCKS_PER_SEC/1000));

    //Insere os valores no arquivo criado
    for(i=0;i<=qtd_t;i++){
      fprintf(pont[i], "%d %d \n", m1_lin, m2_col);
    }


    a = 0;
    aux =0;

    for(i=0;i<m1_lin;i++){
        for(j=0; j<m2_col;j++){
            if(a==p){
              a=0;
              aux++;
            }
            fprintf(pont[aux], "C%d%d %d\n", i, j, m3[i][j]);
            a++;
        }
    }

    for(i=0;i<=qtd_t;i++){
      fprintf(pont[i], "%f seg\n", tempos[i]);
    }
    fprintf(pont[0], "Tempo total: %f seg\n", ((double)fullTime) / ((CLOCKS_PER_SEC/1000)));

    for(i=0;i<=qtd_t;i++){
      fclose(pont[i]);
    }


}

int main(int argc, char *argv[]){
    FILE *file1,*file2;



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

    if(m1_col == m2_lin){
        produto(p, m1_lin, m1_col, m2_lin, m2_col, m1, m2);
    }else{
        printf("Erro!");
    }
}
