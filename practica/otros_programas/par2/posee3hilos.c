//gcc -o posee3hilos posee3hilos.c -lpthread
//Un proceso posee 3 hilos: main, hilo0, hilo1. Los 3 hilos incrementan las variables total y total1.
//1- El hilo main crea los hilos hilo0 e hilo1.
//2- El hilo1 debe esperar al hilo0 antes de acceder a las variables total y total1,
//3- El hilo main debe esperar al hilo1 antes de acceder a las variables total y total1.
//4- El hilo main antes de terminar muestra el valor de las variables total y total1.
//Completar:
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
pthread_t h[2];
int total, total1;
void *hilo0 (void * nro) {
	int numero, t;
	numero = *(int*)nro;
	[//linea en blanco]
	for(t=0; t < 100000 ; t++){
		total=total+numero;
		total1=total1+1;
	}
	pthread_exit(NULL);
	} 
void *hilo1 (void * nro) {
	int numero, t;
	numero = *(int*)nro;
	[pthread_join(h[0],NULL);]
	for(t=0; t < 10000 ; t++){
		total=total+1;
		total1=total1+numero;
	} 
	pthread_exit(NULL);
int main() {
	int j, a[5]={0};
	total=10000;
	total1=100000;
	a[0]=10;
	a[1]=2;
	a[3]=1;
	[//linea en blanco]
	pthread_create(&h[0], NULL, hilo0, (void *) (&a[0]) );
	pthread_create(&h[1], NULL, hilo1, (void *) (&a[1]) );
	[pthread_join(h[1],NULL);]
	for(j=0; j < 100000 ; j++){
		total=total+a[3];
		total1=total1+1;
	}
	printf("Total= %d, Total1= %d\n",total,total1);
	pthread_exit(NULL);
} 
/*
En consola vemos
Total= [1120000] ,Total1= [320000]
*/
