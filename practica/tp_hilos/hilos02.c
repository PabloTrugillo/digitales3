/*  
 * Ejercicio 2 del TP Hilos
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int t = 0 ;//la variable entera t la inicializa en cero
int arre[5]={0};//declara un arreglo de 5 caracteres inicializado con cero caracteres

void *hola(void * nro) {//esta es la funcion hola que va a ser llamada al momento de crear el hilo
   sleep(2);
   printf("Hola, soy el hilo %d\n", * (int*) nro);
   pthread_exit(NULL);//aca termina solamente el hilo
}

int main() {
pthread_t hilo[5];//define una variable tipo hilo con un maximo de 5 hilos 
int rc;

  for(t=0; t < 5 ; t++){//hago un for de 0 a 4 para la variable t

     printf("Main creando el hilo nro %d\n", t);
     arre[t] = t;//va asignando la posicion en el arreglo del valor que va tomando la variable t.
     //el codigo funciona comentando la linea 29 y descomentando la 30 xq el argumento de la creacion del hilo al estar dentro de un for necesita un arreglo que incremente la posicion como es en la linea 30 y no una variable entera que se va a ir pisando como en la linea 29
     //rc = pthread_create(&hilo[t], NULL, hola , (void *)(&t)  );
     rc = pthread_create(&hilo[t], NULL, hola , (void *) (&arre[t]) );
     
     if (rc != 0){
         printf("ERROR; pthread_create() = %d\n", rc);
         exit(-1);        }
   }
  
   pthread_exit(NULL);
	//la linea 39 no aparece xq esta despues del exit de hilo x lo tanto no se ejecuta nada que venga abajo de esa linea 
   printf("Esto no aparece!!!\n");
   
}
