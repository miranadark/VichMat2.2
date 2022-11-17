#include <stdio.h>
#include <time.h>
#include <math.h>
#include <locale.h>
#include <fstream>
using namespace std; 

#define e 0.000000001 //(10^-4)


double func(double x){
    return pow(x,2)-3;
}
double func_p1(double x){
    return 2*x;
}
double func_p2(){
    return 2;
}

bool check_e(double e_fact){
    if(e_fact < e){
        return true;
    }
    return false;
}
void division (double a, double b){ //половинное деление
    int i = 0;
    double e_fact = abs(b-a)/2;
    double x = (a+b)/2;   
    i++;
    printf("X = %.20f  e = %.20f i = %d\n",x, e_fact, i);
    while(!check_e(e_fact)){
        if( func(a) * func(x) < 0 ){
            b = x;
        }
        else if( func(x) * func(b) < 0 ){
            a = x;
        }
        else{printf("Оба больше '0'!");}

        e_fact = abs(b-a)/2;
        x = (a+b)/2;
        i++;
        printf("X = %.20f  e = %.20f i = %d\n",x, e_fact, i);
    }
}
void hord(double a, double b){
    int i = 0;
    double e_fact = abs(b-a)/2;
    double x = ( a*func(b) - b*func(a) ) / ( func(b)-func(a) );  
    i++; 
    printf("X = %f  e = %f i = %d\n",x, e_fact, i);
    while(!check_e(e_fact)){
        if( func(a) * func(x) < 0 ){
            b = x;
        }
        else if( func(x) * func(b) < 0 ){
            a = x;
        }
        else{printf("Оба больше '0'!");}

        e_fact = abs(b-a)/2;
        x = ( a*func(b) - b*func(a) ) / ( func(b)-func(a) );
        i++;
        printf("X = %.20f  e = %.20f i = %d\n",x, e_fact, i);
    }
}
void newton(){
    int i = 0;
    double x_one = func_p2();
    double x_two = x_one - ( func(x_one) / func_p1(x_one) );
    i++;
    printf("X = %.20f  e = %.20f i = %d\n",x_two, x_one - x_two, i);
    while( !((x_one - x_two) < e)){

        x_one = x_two;
        x_two = x_one - ( func(x_one) / func_p1(x_one) );
        i++;

        printf( "X = %.20f  e = %.20f i = %d\n",x_two, x_one - x_two, i );
    }
     
}
int main () {
    setlocale(LC_ALL,"Russian");
    srand(time(NULL));
    
    ifstream file("matrix.txt");
    double a = 0, b = 0;
    file >> a >> b; //записываем промежуток
    
    file.close();
    

    //Метод Половинного деления
    printf("Метод половинного деления:\n");
    division(a,b);
    printf("Метод хорд\n");
    hord(a,b);
    printf("Метод Ньютона\n");
    newton();
    
    return 0;
}