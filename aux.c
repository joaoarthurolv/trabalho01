#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
    int n1,n2,m1,m2,i,j;
    FILE *file1, *file2;

    file1 = fopen("arq1.txt", "w");
    file2 = fopen("arq2.txt", "w");

    n1 = atoi(argv[1]);
    m1 = atoi(argv[2]);
    n2 = atoi(argv[3]);
    m2 = atoi(argv[4]);
    
    int matriz1[n1][m1];
    int matriz2[n2][m2];

    srand(time(NULL));

    for(i=0;i<n1;i++){
        for(j=0;j<m1;j++){
            matriz1[i][j] = rand() % 100;
        }
    }

    for(i=0;i<n2;i++){
        for(j=0;j<m2;j++){
            matriz2[i][j] = rand() % 100;
        }
    }

    for(i=0;i<n2;i++){
        for(j=0;j<m2;j++){
            if(j == (m2-1)){
                fprintf(file1, "%d\n", matriz1[i][j]);
            }
            else{
                fprintf(file1, "%d ", matriz1[i][j]);
            }
        }
    }

    for(i=0;i<n2;i++){
        for(j=0;j<m2;j++){
            if(j == (m2-1)){
                fprintf(file2, "%d\n", matriz2[i][j]);
            }
            else{
                fprintf(file2, "%d ", matriz2[i][j]);
            }
        }
    }
}