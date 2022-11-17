#include <stdio.h>
#include <time.h>
#include <math.h>
#include <locale.h>
#include <fstream>
#include "SFML/Graphics.hpp"

using namespace std; 
using namespace sf;

#define e_okr 0.000005 //(10^-9)
#define WIDTH 1920.f
#define HEIGHT 1080.f
#define SCALE_X 200
#define SCALE_Y 300
#define CENTR_X WIDTH/2-150
#define CENTR_Y HEIGHT/2
#define RADIUS 5

double f(double x){
    return sqrt(x);
    //return x == 0 ? 0 : 10/x;
}

Vertex* calcPointForFunc(double point_x_start, double point_x_end, double h, int *t_n){
    int n = (int)( ceil( (abs(point_x_start) + abs(point_x_end)) / (h) ) );
    *t_n = n;
    Vertex *func_point = new Vertex [n];
    //printf("%d", n);
    double temp = point_x_start;
    
    for(int i = 0; i < n; i++){
        
        func_point[i].position =  Vector2f(CENTR_X + temp * SCALE_X, HEIGHT - CENTR_Y - f(temp) * SCALE_Y);
        func_point[i].color = Color::White;
        temp += h;
        
    }
    return func_point;
}
CircleShape* CalcPointShape(double *Point_X, double *Point_Y, int n){
    CircleShape *result = new CircleShape [n];

    for(int i = 0; i < n; i++){
        result[i].setPosition(Vector2f(CENTR_X + Point_X[i] * SCALE_X - RADIUS, CENTR_Y - Point_Y[i]*SCALE_Y - RADIUS));
        result[i].setRadius(RADIUS);
        result[i].setFillColor(Color::Red);
    }
    return result;
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


double f_4(double x){
    return (-15) / ( 16*sqrt( pow(x,7) ) );
}

double e_real (double x, double *Point_Array_X, int n){
    double e_usech = 0;
    double M = abs(f_4(Point_Array_X[0]));

    for(int i = 1; i < n; i++){
        double t = abs( f_4(Point_Array_X[i]) );
        if( t > M ){
            M = t;
        }
    }

    double temp = 1;
    for(int i = 0; i < n; i++){
        temp *= x-Point_Array_X[i];
    }
    temp = abs(temp);

    int denominator = 1;
    for(int i = 2; i < n+1; i++){
        denominator *= i;
    }

    e_usech = (M*temp)/denominator;

    return (e_okr + e_usech);
}

double g(int n, double *Point_Array_X, double x, int i){
    double numerator = 1, denominator = 1;
    for(int j = 0; j < n; j++){
        if(j != i){
            numerator *= (x-(Point_Array_X[j]));
            denominator *= (Point_Array_X[i])-(Point_Array_X[j]);
        }
    }
    if(denominator == 0){
        printf("\nDelenie NA NULL ");
        return -1;
    }
    return numerator/denominator;
}

double Lagrange (int n,double *Point_Array_X, double *Point_Array_Y, double x) {
    double result = 0, temp;
    
    for(int i = 0; i < n; i++){
        temp = g(n, Point_Array_X, x, i);
        result += Point_Array_Y[i]*temp;
        
    }
    
    return result;
}
Vertex* calcShape(double start, double end, double hod, int *t_g, double *Point_X, double *Point_Y, int n){
    int n_1 = (int)((abs(start) + abs(end))/hod);
    
    Vertex *point = new Vertex [n_1];
    *t_g = n_1;
    double temp_x = start;
    for(int i = 0; i < n_1; i++){
        double t = Lagrange(n,Point_X,Point_Y,temp_x);
        point[i].position = Vector2f(CENTR_X + temp_x*SCALE_X,CENTR_Y - t*SCALE_Y);
        point[i].color = Color::Green;
        temp_x += hod;
        
    }

    return point;

    
    
    //point[n].position = Vector2f(CENTR_X + Point_X[n-1] * SCALE, HEIGHT - CENTR_Y - Point_Y[n-1][1] * SCALE - RADIUS);
    
    //point[n].color = Color::Green;
    
    //point[n-1].color = Color::Green;
    

    return point;
}

int main () {
    setlocale(LC_ALL,"Russian");
    
    
    ifstream point_file("./point.txt");
    int n = 0;
    double x = 0;
    point_file >> n >> x;
    
    if(n == 0){
        printf("Error: Point = 0!");
        return -1;
    }
    double *Point_Array_Y = new double[n+1]; //y
    double *Point_Array_X = new double[n+1]; //x
    for(int i = 0; i < n; i++){
        
        point_file >> Point_Array_X[i] >> Point_Array_Y[i];
    }
    point_file.close();

    
    double result = Lagrange(n,Point_Array_X,Point_Array_Y,x);
    double tochnost = e_real(x,Point_Array_X,n);
    Point_Array_X[n] = x;
    Point_Array_Y[n] = result;

    ofstream res("./result.txt");
    res <<"y = "<< result << "\n";
    res << "e = " << tochnost;


    int t_n;
    Vertex *points_func = calcPointForFunc(-40.f, 40.f, 0.1, &t_n);
    int t_x, t_y, t_g;
    Vertex **marcup = Markup (&t_x,&t_y);
    CircleShape *Point_Int = CalcPointShape (Point_Array_X,Point_Array_Y, n);
    Vertex *point = calcShape(-40.f, 40.f, 0.1, &t_g, Point_Array_X, Point_Array_Y,n);

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Lagrange");
    
    Vertex *OsX = new Vertex[2];
    Vertex *OsY = new Vertex[2];

    OsX[0] = Vector2f(0,CENTR_Y);
    OsX[1] = Vector2f(WIDTH,CENTR_Y);

    OsY[0] = Vector2f(CENTR_X,0);
    OsY[1] = Vector2f(CENTR_X,HEIGHT);
    /*
    CircleShape shape(100.f);
    shape.setFillColor(Color::Green);
    */
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
        for(int i = 0; i < n; i++){
            window.draw(Point_Int[i]);
        }
        
        window.display();
        sleep(slp);
    }

    return 0;
}