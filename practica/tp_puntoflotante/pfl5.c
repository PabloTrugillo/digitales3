/*Ejercicio 5. Excepciones
Con el código visto en el ejercicio 4, ex_04.c. genere los 4 tipo de excepciones estudiadas al
ejecutar diferentes operaciones matemáticas, en lugar de utilizar la función feraiseexcept().*/

//para compilar usar: gcc -Wall -O3 -std=c99 ex_05.c -o ex_05 -lm -march=corei7 -frounding-math -fsignaling-nans
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <fenv.h>

void show_fe_exceptions(void)// esta es la funcion que llama las excepciones
{   
    
    printf("excecion: ");
    if(fetestexcept(FE_DIVBYZERO)) printf(" FE_DIVBYZERO");
    if(fetestexcept(FE_INEXACT))   printf(" FE_INEXACT");
    if(fetestexcept(FE_INVALID))   printf(" FE_INVALID");
    if(fetestexcept(FE_OVERFLOW))  printf(" FE_OVERFLOW");
    if(fetestexcept(FE_UNDERFLOW)) printf(" FE_UNDERFLOW");
    feclearexcept(FE_ALL_EXCEPT);//intentará borrar las excepciones de punto flotante admitidas representadas por excepciones, devolverá cero si. De lo contrario, devolverá un valor distinto de cero.
    printf("\n");
//La función fetestexcept () determinará cuál de un subconjunto especificado de banderas de excepción de punto flotante está establecido actualmente. El argumento de excepciones especifica los indicadores de estado de punto flotante que se van a consultar,devolverá el valor del OR inclusivo bit a bit de las macros de excepción de coma flotante correspondientes a las excepciones de coma flotante establecidas actualmente incluidas en las excepciones.
}
 
int main(void)
{
	printf("\n");
	//math_errhandling:
    printf("MATH_ERREXCEPT is %s\n",math_errhandling & MATH_ERREXCEPT ? "set" : "not set");
//MATH_ERREXCEPT : indica que se utilizan execepciones de punto flotante 
//Si la implementación admite aritmética de coma flotante IEEE (IEC 60559), se requiere que math_errhandling & MATH_ERREXCEPT no sea cero.

//Se reconocen las siguientes condiciones de error en punto flotante. consecutivamente a cada error llamo a la funcion para que me identifique cada excepcion
    printf("0.0/0.0 = %f\n", 0.0/0.0);//deberia dar FE_INVALID
    show_fe_exceptions();//aca llamo a la funcion
 printf("\n");
    printf("5.0/0.0 = %f\n", 5.0/0.0);//deberia dar FE_DIVBYZERO
    show_fe_exceptions();//aca llamo a la funcion
 printf("\n");
    printf("1.0/100.0 = %f\n", 1.0/100.0);// no deberia dar excepcion
    show_fe_exceptions();//aca llamo a la funcion
 printf("\n");
    printf("sqrt(-1) = %f\n", sqrt(-1));//deberia dar FE_INVALID
    show_fe_exceptions();//aca llamo a la funcion
 printf("\n");
    printf("DBL_MAX*2.0 = %f\n", DBL_MAX*2.0); //Valor finito  maximo float, double,long double   me lo define Defined in header <float.h> y deberia dar infinito sin excepcion
    show_fe_exceptions();//aca llamo a la funcion
 
    printf("nextafter(DBL_MIN/pow(2.0,52),0.0) = %.1f\n",nextafter(DBL_MIN/pow(2.0,52),0.0)); //DBL_MIN: minimo, normalizado, valor flotante positivo, double y long double respectivamente
//Primero convierte  ambos  argumentos al tipo  de funcion luego  vuelve devuelve el siguiente valor representable de x la  direccion  y . si x es igual y , se devuelve y. Este deberia dar                                                                                                  
    show_fe_exceptions();//aca llamo a la funcion
    
    
}



