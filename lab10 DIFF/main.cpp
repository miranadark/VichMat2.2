#include <stdio.h>
#include <time.h>
#include <cmath>
#include <locale.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include "SFML/Graphics.hpp"

using namespace std; 
using namespace sf;

#define e 0.0000000001 //(10^-5)
#define h_od 0.1

#define WIDTH 1000.f
#define HEIGHT 1000.f
#define SCALE 50
#define CENTR_X WIDTH/2
#define CENTR_Y HEIGHT/2
#define RADIUS 5

//Интегрирование с двойным пересчетом


double f(double x){
    return 1/x;
    //return x != 0 ? log(x) : 0;
    //return sqrt(x);
}


Vertex* calcPointForFunc(double point_x_start, double point_x_end, double h, int *t_n){
    int n = (int)( ceil( (abs(point_x_start) + abs(point_x_end)) / (h) ) );
    *t_n = n;
    Vertex *func_point = new Vertex [n];
    //printf("%d", n);
    double temp = point_x_start;
    
    for(int i = 0; i < n; i++){
        
        func_point[i].position =  Vector2f(CENTR_X + temp * SCALE, HEIGHT - CENTR_Y - f(temp) * SCALE);
        func_point[i].color = Color::White;
        temp += h;
        
    }
    return func_point;
}

Vertex** calcShape(double *Point_X, double *Point_Y, double *Point_Mnim, int n, int *t_g){
    Vertex **point = new Vertex *[2];
    point[0] = new Vertex [n];
    point[1] = new Vertex [n];
    *t_g = n;

    double **temp_Point = new double *[n+1];
    double **temp_Point_Mnim = new double *[n+1];
    for(int i = 0; i < n+1; i++){
        temp_Point[i] = new double [2];
        temp_Point[i][0] = Point_X[i];
        temp_Point[i][1] = Point_Y[i]; 

        temp_Point_Mnim[i] = new double [2];
        temp_Point_Mnim[i][0] = Point_X[i];
        temp_Point_Mnim[i][1] = Point_Mnim[i];
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
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (temp_Point_Mnim[j][0] > temp_Point_Mnim[j + 1][0]) {
                // меняем элементы местами
                double *temp = temp_Point_Mnim[j];
                temp_Point_Mnim[j] = temp_Point_Mnim[j + 1];
                temp_Point_Mnim[j + 1] = temp;
            }
        }
    }


    for(int i = 0; i < n; i++){
        point[0][i].position = Vector2f(CENTR_X + temp_Point[i][0] * SCALE, HEIGHT - CENTR_Y - temp_Point[i][1] * SCALE);
        point[1][i].position = Vector2f(CENTR_X + temp_Point_Mnim[i][0] * SCALE, HEIGHT - CENTR_Y - temp_Point_Mnim[i][1] * SCALE);
        point[0][i].color = Color::Green;
        point[1][i].color = Color::Red; 
    }
    
    //point[n].position = Vector2f(CENTR_X + Point_X[n-1] * SCALE, HEIGHT - CENTR_Y - Point_Y[n-1][1] * SCALE - RADIUS);
    
    //point[n].color = Color::Green;
    
    //point[n-1].color = Color::Green;
    

    return point;
}

Vertex** Markup (int *t_x, int *t_y){

    int n_x = (int)((WIDTH/SCALE + 1)*2);
    int n_y = (int)((HEIGHT/SCALE + 1)*2);
    *t_x = n_x;
    *t_y = n_y;

    Vertex **markup = new Vertex *[2];
    markup[0] = new Vertex [n_x];
    markup[1] = new Vertex [n_y];

    int t_scale = 0;
    for(int i = 0; i < (n_x-1) /2; i+=2){
        markup[0][i] = Vector2f(CENTR_X - t_scale*SCALE, CENTR_Y - 5);
        markup[0][i+1] = Vector2f(CENTR_X - t_scale*SCALE, CENTR_Y + 5);
        t_scale++;
    }
    t_scale = 0;
    for(int i = (n_x) /2-1; i < n_x-1; i+=2){
        markup[0][i] = Vector2f(CENTR_X + t_scale*SCALE, CENTR_Y - 5);
        markup[0][i+1] = Vector2f(CENTR_X + t_scale*SCALE, CENTR_Y + 5);
        t_scale++;
    }
    t_scale = 0;
    for(int i = 0; i < (n_y-1)/2; i+=2){
        markup[1][i] = Vector2f(CENTR_X - 5, CENTR_Y - t_scale*SCALE);
        markup[1][i+1] = Vector2f(CENTR_X + 5, CENTR_Y - t_scale*SCALE);
        t_scale++;
    }
    t_scale = 0;
    for(int i = (n_y)/2-1; i < n_y-1; i+=2){
        markup[1][i] = Vector2f(CENTR_X - 5, CENTR_Y + t_scale*SCALE);
        markup[1][i+1] = Vector2f(CENTR_X + 5, CENTR_Y + t_scale*SCALE);
        t_scale++;
    }

    return markup;
}


double Simpson(double *Point_X, double *Point_Y, int n){
    double *Temp = new double[(n+1)/2];
    double Result = 0;
    for(int i = 0, j = 0, length = (n+1)/2; i < length; i++, j+=2){
        Temp[i] = (Point_X[j+2]-Point_X[j])*( (1.f/6.f)*Point_Y[j] + (2.f/3.f)*Point_Y[j+1] + (1.f/6.f)*Point_Y[j+2] );
        //printf("Temp %d: %f\n",i, Temp[i]);
        Result += Temp[i];
    }
    //printf("Result: %f\n", Result);
    return Result;
    delete[] Temp;
}




int main () {
    setlocale(LC_ALL,"Russian");
    srand(time(NULL));
    
    ifstream point_file("./point.txt");
    int k, flag;
    double x_start = 0, x_end = 0, h = 0, result = 0;
    point_file >> x_start >> x_end >> h;
    
    
    
    double *Point_Array_Y;
    double *Point_Array_X;

    point_file.close();
    int step = 1;

    printf("Step %d: %.10f",step, h);
    
    do{
        int n = (int)((x_end - x_start) / h);
        Point_Array_X = new double [n+1];
        Point_Array_Y = new double [n+1];

        for(int i = 0; i < n+1; i++){
            Point_Array_X[i] = x_start + h*i;
            Point_Array_Y[i] = f(Point_Array_X[i]);
        }
        double res_1 = Simpson( Point_Array_X,Point_Array_Y, n);
        if(step == 1){
            printf(" Int: %.20f\n", res_1);
        }
        delete[] Point_Array_X;
        delete[] Point_Array_Y;

        double h_2 = h/2;
        n = (int)((x_end - x_start) / h_2);
        Point_Array_X = new double [n+1];
        Point_Array_Y = new double [n+1];

        for(int i = 0; i < n+1; i++){
            Point_Array_X[i] = x_start + h_2*i;
            Point_Array_Y[i] = f(Point_Array_X[i]);
        }
        double res_2 = Simpson( Point_Array_X,Point_Array_Y, n);
        step++;
        
        
        if(abs(res_1 - res_2) < 3.f*e){
            result = res_2;
            break;
        }
        printf("Step %d: %.10f Int: %.20f\n",step, h_2, res_2);
        h = h_2;


        delete[] Point_Array_X;
        delete[] Point_Array_Y;

    }while(1);
    
    
    
   
    ofstream fout("result.txt");

    fout <<"Result: "<< result;

    fout.close();


    
    int t_n;
    Vertex *points_func = calcPointForFunc(-40.f, 40.f, 0.1, &t_n);
    int t_x, t_y, t_g;
    Vertex **marcup = Markup (&t_x,&t_y);
    //Vertex **point = calcShape(Point_Array_X,Point_Array_Y, Point_Mnim_Y, n+1, &t_g);

    //RenderWindow window(VideoMode(WIDTH, HEIGHT), "Newton");
    
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
    /*
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
        window.draw(marcup[1],t_y, Lines);
        window.draw(points_func,t_n, LineStrip);
        //window.draw(point[0],t_g,LineStrip);
        //window.draw(point[1],t_g,LineStrip);
        
        window.display();
        sleep(slp);
    }

    //delete[] Point_Mnim_Y;*/
    delete[] Point_Array_X;
    delete[] Point_Array_Y;
    return 0;
}