/*
1.Вывести квадратную матрицу по заданному N.
*/
#include <stdio.h>
#define n 5

int main(void){
    int matrix[n][n] = {0};
    int x = 1;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            matrix[i][j] = x;
            x++;
        }
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}