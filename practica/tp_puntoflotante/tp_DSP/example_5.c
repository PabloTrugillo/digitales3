#include <stdio.h>
#include <math.h>
#include <float.h>
#include <fenv.h>
 
#pragma STDC FENV_ACCESS ON // informa el programa el compilador que podría acceder al entorno de punto flotante para poner a prueba sus indicadores de estado (excepciones) o se ejecute en los modos de control distintos al uno por defecto.
void show_fe_exceptions(void)
{
    printf("exceptions raised:");
    if(fetestexcept(FE_DIVBYZERO)) printf(" FE_DIVBYZERO");
    if(fetestexcept(FE_INEXACT))   printf(" FE_INEXACT");
    if(fetestexcept(FE_INVALID))   printf(" FE_INVALID");
    if(fetestexcept(FE_OVERFLOW))  printf(" FE_OVERFLOW");
    if(fetestexcept(FE_UNDERFLOW)) printf(" FE_UNDERFLOW");
    feclearexcept(FE_ALL_EXCEPT);
    printf("\n");
}
 
int main(void)
{//math_errhandling:
    printf("MATH_ERREXCEPT is %s\n",math_errhandling & MATH_ERREXCEPT ? "set" : "not set");
//MATH_ERREXCEPT : indica que se utilizan execepciones de punto flotante 
//Si la implementación admite aritmética de coma flotante IEEE (IEC 60559), se requiere que math_errhandling & MATH_ERREXCEPT no sea cero.
//Se reconocen las siguientes condiciones de error en punto flotante
    printf("0.0/0.0 = %f\n", 0.0/0.0);
    show_fe_exceptions();
 
    printf("1.0/0.0 = %f\n", 1.0/0.0);
    show_fe_exceptions();
 
    printf("1.0/10.0 = %f\n", 1.0/10.0);
    show_fe_exceptions();
 
    printf("sqrt(-1) = %f\n", sqrt(-1));
    show_fe_exceptions();
 
    printf("DBL_MAX*2.0 = %f\n", DBL_MAX*2.0); //Valor finito  maximo float, double,long double   me lo define Defined in header <float.h>
    show_fe_exceptions();
 
    printf("nextafter(DBL_MIN/pow(2.0,52),0.0) = %.1f\n",nextafter(DBL_MIN/pow(2.0,52),0.0)); //DBL_MIN: minimum, normalized, positive value of float, double and long double respectively
//Primero convierte  ambos  argumentos al tipo  de funcion luego  vuelve devuelve el siguiente valor representable de x la  direccion  y . si x es igual y , se devuelve y
    show_fe_exceptions();
    
    
}

//La función feclearexcept () intentará borrar las excepciones de punto flotante admitidas representadas por excepciones .
//Si el argumento es cero o si todas las excepciones especificadas se borraron con éxito, feclearexcept () devolverá cero. De lo contrario, devolverá un valor distinto de cero.
//feraiseexcept() Intenta generar todas las excepciones de punto flotante enumeradas:q


//La función fetestexcept () determinará cuál de un subconjunto especificado de banderas de excepción de punto flotante está establecido actualmente. El argumento de excepciones especifica los indicadores de estado de punto flotante que se van a consultar.

//VALOR DEVUELTO
//La función fetestexcept () devolverá el valor del OR inclusivo bit a bit de las macros de excepción de coma flotante correspondientes a las excepciones de coma flotante establecidas actualmente incluidas en las excepciones

