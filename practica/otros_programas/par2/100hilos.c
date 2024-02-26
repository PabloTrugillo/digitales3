//En el programa el hilo main crea 100 hilos.
//1- Los hilos realizan una suma en la variable total y se sincronizan con un mutex.
//2- El hilo main espera a que los hilos terminen, muestra el valor de la variable total y luengo termina él.
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int total, a[5]={0};
pthread_t h[100];
[pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;]
void *hilo1 (void * nro) {
	int numero, t;
	numero = *(int*)nro;
	pthread_mutex_lock(&m);
	for(t=0; t < 20000 ; t++){
	[//linea en blanco]
	total=total+numero;
	[//linea en blanco]
	} 
	[pthread_mutex_unlock(&m);]
	pthread_exit(NULL);
} 
int main() {
int t;
	total=5000000;
	a[0]=10;
	[//linea en blanco]
	for(t=0; t< 100 ; t++){
	[pthread_create(&h[t], NULL, hilo1, (void *) (&a[0]));]
	} 
	for(t=0; t< 100 ; t++){
	[pthread_join(h[t],NULL);]
	}
	 printf("Total= %d\n", total);
pthread_exit(NULL);
} 
/*
En consola vemos
Total= [25000000]
*/
