/*  
 * Ejercicio 1 del TP Hilos
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *hola(void * nro) {
  
   sleep(2);
   printf("Hola, soy el hilo %d\n", * (int*) nro);
   pthread_exit(NULL);//aca termina el hilo
   //exit(0);

}

int main() {

    pthread_t hilo[1];
    int rc, t;

    t = 0;
     
    printf("Main creando el hilo nro %d\n", t);
    //aca voy a crear el hilo pasandole: en el primer campo el puntero del hilo, en el segundo los atributos o NULL para darle los atributos x defecto, en el tercero la funcion a la cual va a llamar al crear el hilo "hola" y el ultim el argumento, en este caso,"t"   
    rc = pthread_create(&hilo[0], NULL, hola , (void *)(&t)  );
     
    if (rc != 0){//aca se fija q no haya error en la creacion del hilo
         printf("ERROR; pthread_create() = %d\n", rc);
         exit(-1);        
         };
  
   printf("Espera a que termine hilo\n");
//hace un join al hilo cero que es el que acabo de crear, o sea q el programa va a ejecutar esa parte y en el segundo campo pongo NULL xq no me interesa el valor que me retorne
   pthread_join(hilo[0],NULL);

   printf("Termina hilo main\n");

   pthread_exit(NULL);
   
   return 0;
}
