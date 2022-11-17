#include <stdio.h>
#include <time.h>
#include <math.h>
#include <locale.h>
#include <fstream>
using namespace std; 

void show(double **matrix, int x, int y){
    for(int i = 0; i < y; i++){
        for(int j = 0; j < x; j++){
            printf("%.5f ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main () {
    setlocale(LC_ALL,"Russian");
    srand(time(NULL));
    
    ifstream matrix_file("matrix.txt");
    int x = 0, y = 0;
    matrix_file >> y >> x; // Записываем размер матрицы
    if(x == 0 || y == 0){ // Проверяем размер матрицы
        printf("Матрица неправильной формы! X == 0 или Y = 0\n");
        return -1;
    }
    else if(x - y < 1 || x - y > 1){ // Проверяем размер матрицы
        printf("Матрица неправильной формы! X - Y < 1 или X - Y > 1\n");
        return -1;
    }
    
    //Создаём матрицу
    double **matrix = new double *[y];
    for(int i = 0; i < y; i++){
        matrix[i] = new double [x];
        
    }

    //записываем значения из матрицы
    for(int i = 0; i < y && !matrix_file.eof(); i++){
        for(int j = 0; j < x && !matrix_file.eof(); j++){
            matrix_file >> matrix[i][j];   
        }
    }
    matrix_file.close();
    show(matrix,x,y);

    //Метод гауса!
    double temp_numerator, temp_denominator;
    
    for(int k = 0; k < x-1; k++){ //Идём по главной диагонали
         //Запоминаем знаменатель
        double temp_number = abs(matrix[k][k]);
        int temp_position = k;
        show(matrix,x,y);
        
        for(int i = k+1; i < y; i++){
            if(abs(matrix[i][k]) > temp_number){
                temp_number = abs(matrix[i][k]);
                temp_position = i;
            }
        }
        double *t = matrix[k];
        matrix[k] = matrix[temp_position];
        matrix[temp_position] = t;
        printf("\nПереставил %d строку c %d строкой\n", k+1, temp_position+1);
        temp_denominator = matrix[k][k];
        
        show(matrix,x,y);
        
        for(int i = k+1; i < y; i++){ //Идём по y
            temp_numerator = matrix[i][k]; //Запонимаем числитель
            for(int j = k+1; j < x; j++){ //Идём по x
                matrix[i][j] -= matrix[k][j]*temp_numerator/temp_denominator;
            }
            matrix[i][k] = 0;
        }
    }
    
    //show(matrix,x,y);

    for(int k = x-2; k > 0; k--){  //Идём по главной диагонали
        temp_denominator = matrix[k][k]; //Запоминаем знаменатель
        for(int i = k-1; i > -1; i--){ //Идём по y
            temp_numerator = matrix[i][k]; //Запонимаем числитель
            for(int j = x-1; j > k-1; j--){ //Идём по x
                matrix[i][j] -= matrix[k][j]*temp_numerator/temp_denominator;
            }
            matrix[i][k] = 0;
        }
    }

    show(matrix,x,y);

    ofstream matrix_out_file("matrix_answer.txt");

    for(int k = 0; k < x-1; k++){
        matrix[k][x-1] /= matrix[k][k];
        matrix[k][k] = 1; 
        matrix_out_file << "X" << k+1 << ": " << matrix[k][x-1] << "\n"; 
    }
    show(matrix,x,y);
    return 0;
}