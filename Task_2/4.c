/*
4.Заполнить матрицу числами от 1 до N^2 улиткой.
*/

#include <stdio.h>
#define n 5

int main(void){
    int matrix[n][n]={0};
    int row_up = 0, col_right = n - 1, row_down = n - 1, col_left = 0, zap = 0;

    while(row_up != n){
        for(int i = row_up; i < col_right; i++){
            matrix[row_up][i] = ++zap;
        }
        row_up++;
        
        for(int i = col_left; i < row_down; i++){
            matrix[i][col_right] = ++zap;
        }
        row_down--;

        for(int i = col_right; i >= col_left; i--){
            matrix[col_right][i] = ++zap;
        }
        col_right--;

        for(int i = col_right; i >= row_up; i--){
            matrix[i][col_left] = ++zap;
        }
        col_left++;
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
    return 0;
}