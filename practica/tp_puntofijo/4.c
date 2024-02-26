/*  
    Implemente la suma de dos números con aritmética de saturación
    int32_t saturation(int32_t a, int32_t b);
    1. Utilice las constantes INT_MAX e INT_MIN definidas en la biblioteca limits.h para la 
    2. Opere con una serie de número para verificar que saturation() funciona correctamente.
*/

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

int32_t saturation(int32_t a, int32_t b)
{
//donde a y b son los números que sumo y tienen signo por lo que la suma puede estar resultando en una resta
    int32_t suma = 0;
    suma = a + b;
    if(a>0 && b>0){
        if(suma < 0)//si ambos son positivos y la suma de ellos es negativa, hago forzar el resultado de la suma al valor de INT_MAX
        suma = INT_MAX;
    }
    if(a<0 && b<0){
        if(suma>0){//si ambos son negativos y la suma de ellos es positiva, hago forzar el resultado de la suma al valor de INT_MIN
            suma=INT_MIN;
        }
    }
    return(suma);
}

int main(){
    int32_t nro1,nro2,sat;
	
    printf("\nINT_MAX = %d\n",INT_MAX);//2147483647
    printf("INT_MIN = %d\n",INT_MIN);//-2147483648

    //ambos positivos chicos

    nro1 = 11;
    nro2 = 42;

    sat = saturation(nro1, nro2);

    printf("\ncon dos numeros positivos chicos nro1 = %d, nro2 = %d, sat = %d\n",nro1,nro2, sat);

    //ambos negativos chicos

    nro1 = -21; 
    nro2 = -42;

    sat = saturation(nro1, nro2);

    printf("\ncon dos numeros negativos chicos nro1 = %d, nro2 = %d, sat = %d\n\n",nro1,nro2, sat);


    //positivo chico mas negativo chico

    nro1 = -21; 
    nro2 =  73;

    sat = saturation(nro1, nro2);

    printf("\ncon uno positivo chico y uno negativo chico nro1 = %d, nro2 = %d, sat = %d\n\n",nro1,nro2, sat);
    //ambos positivos grandes

    nro1 = 2147483641; //cercano al límite superior
    nro2 = 42;

    sat = saturation(nro1, nro2);

    printf("\ncon dos numeros positivos grandes nro1 = %d, nro2 = %d, sat = %d\n",nro1,nro2, sat);

    //ambos negativos grandes

    nro1 = -2147483641; //cercano al límite superior
    nro2 = -42;

    sat = saturation(nro1, nro2);

    printf("\ncon dos numeros negativos grandes nro1 = %d, nro2 = %d, sat = %d\n\n",nro1,nro2, sat);
    return(0);
}
