#include <stdio.h>
#include <time.h>
#include <math.h>
#include <locale.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include "SFML/Graphics.hpp"

using namespace std; 
using namespace sf;

#define e 0.000005 //(10^-5)
#define h_od 1

#define WIDTH 1920.f
#define HEIGHT 1080.f
#define SCALE_X 100
#define SCALE_Y 100
#define CENTR_X WIDTH/2
#define CENTR_Y HEIGHT/2
#define RADIUS 5

double comp1 (const void * a, const void * b)
{
  return ( *(double*)a - *(double*)b );
}


double g_0(double x){
    return 1;
}
double g_1(double x){
    return x;
}
double g_2(double x){
    //return pow(x,2);
    return sqrt(x);
}
double f(double x){
    
    return x*x;
    //return sqrt(x);
}
double f_Approksim(double a, double b, double c, double x){
    return a*g_0(x) + b*g_1(x) + c*g_2(x);
}

Vertex* calcPointForFunc(double point_x_start, double point_x_end, double h, int *t_n, int mode, double a = 0, double b = 0, double c = 0){
    int n = (int)( ceil( (abs(point_x_start) + abs(point_x_end)) / (h) ) );
    *t_n = n;
    Vertex *func_point = new Vertex [n];
    //printf("%d", n);
    double temp = point_x_start;
    
    for(int i = 0; i < n; i++){
        if(mode == 1){
            func_point[i].position =  Vector2f(CENTR_X + temp * SCALE_X, HEIGHT - CENTR_Y - f(temp) * SCALE_Y);
            func_point[i].color = Color::White;
        }
        else{
            func_point[i].position =  Vector2f(CENTR_X + temp * SCALE_X, HEIGHT - CENTR_Y - f_Approksim(a,b,c,temp) * SCALE_Y);
            func_point[i].color = Color::Green;
        }
        
        
        temp += h;
        
    }
    return func_point;
}

Vertex* calcShape(double *Point_X, double *Point_Y, int n, int *t_g){
    Vertex *point = new Vertex [n];
    *t_g = n;

    double **temp_Point = new double *[n+1];

    for(int i = 0; i < n+1; i++){
        temp_Point[i] = new double [2];
        temp_Point[i][0] = Point_X[i];
        temp_Point[i][1] = Point_Y[i]; 
    }
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (temp_Point[j][0] > temp_Point[j + 1][0]) {
                // меняем элементы местами
                double *temp = temp_Point[j];
                temp_Point[j] = temp_Point[j + 1];
                temp_Point[j + 1] = temp;
            }
        }
    }


    for(int i = 0; i < n; i++){
        point[i].position = Vector2f(CENTR_X + temp_Point[i][0] * SCALE_X, HEIGHT - CENTR_Y - temp_Point[i][1] * SCALE_Y);
        point[i].color = Color::Green;
        
    }
    
  

    return point;
}

Vertex** Markup (int *t_x, int *t_y){

    int n_x = (int)(WIDTH/SCALE_X);
    int n_y = (int)(HEIGHT/SCALE_Y);
    *t_x = n_x;
    *t_y = n_y;

    Vertex **markup = new Vertex *[4];
    markup[0] = new Vertex [n_x];
    markup[1] = new Vertex [n_x];
    markup[2] = new Vertex [n_y];
    markup[3] = new Vertex [n_y];


    int t_scale = 0;
    for(int i = 0; i < n_x-1; i+=2){
        markup[0][i] = Vector2f(CENTR_X - t_scale*SCALE_X, CENTR_Y - 5);
        markup[0][i+1] = Vector2f(CENTR_X - t_scale*SCALE_X, CENTR_Y + 5);
        t_scale++;
    }
    t_scale = 0;
    for(int i = 0; i < n_x-1; i+=2){
        markup[1][i] = Vector2f(CENTR_X + t_scale*SCALE_X, CENTR_Y - 5);
        markup[1][i+1] = Vector2f(CENTR_X + t_scale*SCALE_X, CENTR_Y + 5);
        t_scale++;
    }
    t_scale = 0;
    for(int i = 0; i < n_y-1; i+=2){
        markup[2][i] = Vector2f(CENTR_X - 5, CENTR_Y - t_scale*SCALE_Y);
        markup[2][i+1] = Vector2f(CENTR_X + 5, CENTR_Y - t_scale*SCALE_Y);
        t_scale++;
    }
    t_scale = 0;
    for(int i = 0; i < n_y-1; i+=2){
        markup[3][i] = Vector2f(CENTR_X - 5, CENTR_Y + t_scale*SCALE_Y);
        markup[3][i+1] = Vector2f(CENTR_X + 5, CENTR_Y + t_scale*SCALE_Y);
        t_scale++;
    }

    return markup;
}

int modifiedGaus(double **matrix, int w, int h){
    int x = w, y = h;
    //Метод гауса!
    double temp_numerator, temp_denominator;
    
    for(int k = 0; k < x-1; k++){ //Идём по главной диагонали
         //Запоминаем знаменатель
        double temp_number = abs(matrix[k][k]);
        int temp_position = k;
        //show(matrix,x,y);
        
        for(int i = k+1; i < y; i++){
            if(abs(matrix[i][k]) > temp_number){
                temp_number = abs(matrix[i][k]);
                temp_position = i;
            }
        }
        double *t = matrix[k];
        matrix[k] = matrix[temp_position];
        matrix[temp_position] = t;
        //printf("\nПереставил %d строку c %d строкой\n", k+1, temp_position+1);
        temp_denominator = matrix[k][k];
        
        //show(matrix,x,y);
        
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
    for(int k = 0; k < x-1; k++){
        matrix[k][x-1] /= matrix[k][k];
        matrix[k][k] = 1; 
    }
}

//Метод наименьших квадратов
void Approk_LSM(double *Point_X, double *Point_Y, int n, double &a, double &b, double &c){

    int m = 3; //Мерность

    //Матрица для вычисления коффициентов
    double **C = new double *[m];
    for(int i = 0; i < m; i++){
        C[i] = new double [m+1];
        for(int j = 0; j < m+1; j++){
            C[i][j] = 0;
        }
    }

    //Вычисляем суммы
    
    for(int i = 0; i < n; i++){
        C[0][0] += g_0(Point_X[i]) * g_0(Point_X[i]);
        C[0][1] += g_0(Point_X[i]) * g_1(Point_X[i]);
        C[0][2] += g_0(Point_X[i]) * g_2(Point_X[i]);
        C[1][0] += g_1(Point_X[i]) * g_0(Point_X[i]);
        C[1][1] += g_1(Point_X[i]) * g_1(Point_X[i]);
        C[1][2] += g_1(Point_X[i]) * g_2(Point_X[i]);
        C[2][0] += g_2(Point_X[i]) * g_0(Point_X[i]);
        C[2][1] += g_2(Point_X[i]) * g_1(Point_X[i]);
        C[2][2] += g_2(Point_X[i]) * g_2(Point_X[i]);
        
        C[0][3] += g_0(Point_X[i]) * Point_Y[i];
        C[1][3] += g_1(Point_X[i]) * Point_Y[i];
        C[2][3] += g_2(Point_X[i]) * Point_Y[i];
    }

    

    modifiedGaus(C, m+1, m);
    
    a = C[0][3];
    b = C[1][3];
    c = C[2][3];
    
}





int main () {
    setlocale(LC_ALL,"Russian");
    srand(time(NULL));
    
    ifstream point_file("./point.txt");
    int n = 0, k;
    
    point_file >> n;
    
    if(n == 0){
        printf("Error: Point = 0!");
        return -1;
    }
    double *Point_Array_Y = new double [n]; //y
    double *Point_Array_X = new double [n]; //x


    for(int i = 0; i < n; i++){
        point_file >> Point_Array_X[i] >> Point_Array_Y[i];
    }
    point_file.close();
    k = n;

    double a = 0, b = 0, c = 0;
    Approk_LSM(Point_Array_X, Point_Array_Y, n, a, b, c);
    
    ofstream res("./result.txt");
    
    res <<"a: = "<< a << "\n";
    res <<"b: = "<< b << "\n";
    res <<"c: = "<< c << "\n";
    res.close();

    
    
    int t_n;
    Vertex *points_func = calcPointForFunc(-40.f, 40.f, 0.1, &t_n, 1);
    int t_x, t_y, t_g;
    Vertex **marcup = Markup (&t_x,&t_y);
    Vertex *point = calcPointForFunc(-40.f, 40.f, 0.1, &t_g, 2, a, b, c);

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Aprocsimation");
    
    Vertex *OsX = new Vertex[2];
    Vertex *OsY = new Vertex[2];

    //Ось X
    OsX[0] = Vector2f(0,CENTR_Y);
    OsX[1] = Vector2f(WIDTH,CENTR_Y);

    //Ось Y
    OsY[0] = Vector2f(CENTR_X,0);
    OsY[1] = Vector2f(CENTR_X,HEIGHT);
    
    //частота обновления экрана
    Time slp = seconds(1.f); 
    
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        
        window.clear();
        
        window.draw(OsX,2, Lines);
        window.draw(OsY,2, Lines);
        window.draw(marcup[0],t_x, Lines);
        window.draw(marcup[1],t_x, Lines);
        window.draw(marcup[2],t_y, Lines);
        window.draw(marcup[3],t_y, Lines);
        window.draw(points_func,t_n, LineStrip);
        window.draw(point,t_g,LineStrip);
        
        window.display();
        sleep(slp);
    }


    delete[] Point_Array_X;
    delete[] Point_Array_Y;
    return 0;
}