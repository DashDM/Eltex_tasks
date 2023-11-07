/*
3.Заполнить верхний треугольник матрицы 1, а нижний 0.
*/

#include <stdio.h>
#define n 5

int main(void){
    int matrix[n][n] = {0};
    int x = 0;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n - x; j++){
            matrix[i][j] = 1;
        }
        for(int j = n - x; j < n; j++){
            matrix[i][j] = 0;
        }
        x++;
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}