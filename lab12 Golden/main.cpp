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

#define e 0.0001 //(10^-5)
#define h_od 1

#define WIDTH 1920.f
#define HEIGHT 1080.f
#define SCALE_X 50
#define SCALE_Y 90
#define CENTR_X WIDTH/2
#define CENTR_Y HEIGHT/2-400
#define RADIUS 5


double f(double x){
    return pow(x,2) - 6*x;
    //return sqrt(x);
}
double e_usech (double *Array_X,double **Array_Y, int n, double x){
    double result = 0;

    for(int i = 1; i < n; i++){
        for(int j = 0, lenght = n-i; j < lenght; j++){
            Array_Y[j][i] = Array_Y[j+1][i-1] - Array_Y[j][i-1];
        }
    }

    double q = (x - Array_X[0])/h_od;
    double numerator = q, denominator = 1;
    for(int i = 1; i < n; i++){
        numerator *= (q-i);
        denominator *= i;
    }
    denominator *= n;
    result = (Array_Y[0][n-1] * numerator) /denominator;

    return result > 0 ? result : -result;
}

double golden_Ratio(double a, double b, int n){

    double result = 0, l1, l2, f_a, f_b;
    int step = 1;
    bool take_a = false;
    l1 = a + (3-sqrt(5.f))/2*(b-a);
    l2 = a + (sqrt(5.f)-1)/2*(b-a);
    f_a = f(l1);
    f_b = f(l2);
    //Если вычисляем до определенной точности
    if(n == 0){
        if(!(abs(b-(a)) < e)){
            do{
                
                if(f_a > f_b){
                    a = l1;
                    l1 = l2;
                    f_a = f_b;
                    l2 = a + (sqrt(5.f)-1)/2*(b-a);
                    f_b = f(l2);
                }
                else{
                    b = l2;
                    l2 = l1;
                    f_b = f_a;
                    l1 = a + (3-sqrt(5.f))/2*(b-a);
                    f_a = f(l1);
                }
                printf("Step: %d, a = %.20f, b = %.20f\n",step, a, b);
                

                step++;
            }while(!(abs(b-a) < e));
        }
        

        result = (a+b)/2;
        printf("x = %.20f\nf(x) = %.20f\n",result, f(result));

        return result;
    }

    //Если вычисляем до определенного колличества шагов
    for(int i = 0; i < n; i++){
        
        if(f_a > f_b){
            a = l1;
            l1 = l2;
            f_a = f_b;
            l2 = a + (sqrt(5.f)-1)/2*(b-a);
            f_b = f(l2);
        }
        else{
            b = l2;
            l2 = l1;
            f_b = f_a;
            l1 = a + (3-sqrt(5.f))/2*(b-a);
            f_a = f(l1);
        }
        printf("Step: %d, a = %.20f, b = %.20f\n",step, a, b);
        step++;
    }
    result = (a+b)/2;
    printf("x = %.20f\nf(x) = %.20f\n",result, f(result));
    return result;
}





int main () {
    setlocale(LC_ALL,"Russian");
    srand(time(NULL));
    
    ifstream point_file("./point.txt");
    int n = 0;
    double a = 0, b = 0, x = 0;

    point_file >> a >> b >> n;
    point_file.close();
    

    
    x = golden_Ratio(a, b, n);
    
    ofstream res("./result.txt");
    
    res <<"Point: = "<< x << "\n";
    res.close();

    
    


    
    return 0;
}