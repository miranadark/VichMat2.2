#include <stdio.h>
#include <time.h>
#include <math.h>
#include <locale.h>
#include <fstream>
using namespace std; 
#define e 0.0001 //(-4)

void show(double **matrix_C, double *matrix_B, int x, int y){
    printf("Матрица С:\n");
    for(int i = 0; i < y; i++){
        printf("%.5f ", matrix_B[i]);
    }
    printf("\nМатрица B:\n");
    for(int i = 0; i < y; i++){
        for(int j = 0; j < x; j++){
            printf("%.5f ", matrix_C[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
bool check_e(double **x, int y){
    for(int j = 0; j < y; j++){ 
       if(abs(x[1][j]-x[0][j]) < e){
           if(x[1][j] != x[0][j]){
               return true;
           }
       }
    }
    return false;
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
    

    //Метод Простых итераций!
    
    
    for(int i = 0; i < y; i++){
        //обмен позициями для улучшения вычислений
        double temp_number = abs(matrix[i][i]);
        int temp_position = i;
        for(int j = i+1; j < y; j++){
            if(abs(matrix[j][i]) > temp_number){
                temp_number = abs(matrix[j][i]);
                temp_position = j;
            }
        }
        double *t = matrix[i];
        matrix[i] = matrix[temp_position];
        matrix[temp_position] = t;

        //Деление на диагональный элемент
        temp_number = matrix[i][i];
        for(int j = 0; j < x; j++){
            matrix[i][j] /= temp_number; 
            //show(matrix,x,y);
        }
        matrix[i][i] = 0.f;
    }

    double* matrix_B = new double [y]; //матрица значений
    double** matrix_C = new double *[y]; //матрица коэффициентов
    //переписываем значения
    for(int i = 0; i < y; i++){ 
        matrix_B[i] = matrix[i][x-1];
    }
    //переписываем коэффициенты
    for(int i = 0; i < y; i++){ 
        matrix_C[i] = new double [x-1];
        for(int j = 0; j < x-1; j++){
            matrix_C[i][j] = matrix[i][j];
        }
    }
    //удаляем исходну матрицу
    for(int i = 0; i < y; i++){ 
        delete[] matrix[i];
    }
    delete[] matrix;
    //уменьшаем ширину матрицы
    x--; 

    double** matrix_end = new double *[2];
    matrix_end[0] = new double [y];
    matrix_end[1] = new double [y];

    for(int j = 0; j < y; j++){ //Первый ход
        matrix_end[0][j] = 0;
        matrix_end[1][j] = matrix_B[j];
    }
    
    if(!check_e(matrix_end,y)){
        do{
            for(int i = 0; i < y; i++){
                printf("X%d: %f\n",i+1,matrix_end[0][i]); 
            }
            printf("\n");
            double temp_matrix[y];
            for(int i = 0; i < y; i++){
                temp_matrix[i] = matrix_end[0][i];
            }
            for(int i = 0; i < y; i++){
                double temp = 0;
                for(int j = 0; j < x; j++){
                    temp += matrix_C[i][j]*temp_matrix[j]; //перемножаем матрицы
                }
                matrix_end[1][i] = matrix_B[i]-temp; //отнимаем
                temp_matrix[i] = matrix_end[1][i]; //запоминаем только что вычесленный элемент
            }
            
            
            for(int i = 0; i < y; i++){
                matrix_end[0][i] = matrix_end[1][i]; //передвигаем результаты
                matrix_end[1][i] = 0;
            }
        }while(!check_e(matrix_end,y));
        
    }
    

    ofstream matrix_out_file("matrix_answer.txt");
    for(int i = 0; i < y; i++){
        matrix_out_file << "X" << i+1 << ": " << matrix_end[0][i] << "\n"; 
    }

    
    //show(matrix_C,matrix_B,x,y);
    return 0;
}