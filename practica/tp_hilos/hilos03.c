/*  
 * Ejercicio 3 del TP Hilos
 *   
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

pthread_t hilo[2];//aca definimos una variable de arreglo de dos posiciones de tipo hilo

void * hilo0()//aca se se abre la funcion para el hilo 0
{
  //pthread_detach(pthread_self());//esta sentencia es para que este hilo no pueda ser unido a ningun otro, en el argumento pongo q me devuelva el TID de este propio hilo.     
  printf ("Yo soy el hilo 0 \n");
  sleep(2);  
  pthread_exit (NULL);//finalizacion de este hilo
}

void * hilo1()//aca se se abre la funcion para el hilo 1
{
  printf ("Yo soy el hilo 1 \n");
  printf("Esperando al hilo %lx \n", hilo[0]);
  
  pthread_join(hilo[0],NULL);//esta sentencia es para unir el presente hilo al hilo colocado en el primer argumento, en este caso, hilo 0, en el segundo argumento colocamos un puntero nulo, o sea NULL, porque no nos importa el valor de retorno del hilo 0
  printf("Terminando hilo 1 \n");
  pthread_exit (NULL);
}

int main()
{
  int rc;
  
  //Crear hilo 0, dandole un puntero nulo a los atributos y al argumento
  rc = pthread_create (&hilo[0], NULL, hilo0, NULL);//como devuelve cero si tuvo exito y -1 error, entonces abajo preguntamos
  if (rc)
    {
      printf ("ERROR; pthread_create() = %d\n", rc);
      exit (-1);
    }    
  printf ("El main creando el hilo hola0, nro %lx\n",hilo[0]);
      
  //Crear hilo 1
  rc = pthread_create (&hilo[1], NULL, hilo1, NULL);  
  if (rc)
    {
      printf ("ERROR; pthread_create() = %d\n", rc);
      exit (-1);
    }    
  printf ("El main creando el hilo hola1, nro %lx\n",hilo[1]);
  
  pthread_join(hilo[1],NULL);//el hilo main espera al hilo 1 e imprime en pantalla
  printf ("Se ejecutaron todos los hilos\n");
  
  pthread_exit(NULL);
  //exit(0);
}
