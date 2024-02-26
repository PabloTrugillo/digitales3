//gcc -o hilos_incrementa_total hilos_incrementa_total.c -lpthread
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
int t, total, c;
sem_t sem;
void *hilo0 (void * nro) {
	int numero, j;
	numero = *(int*)nro;
	//linea en blanco
	for(j=0; j < c; j++){
		sem_wait(&sem);
		total = total + numero;
		sem_post(&sem);
	}
	//printf("\nel valor de la variable numero es:%d\n",numero);
	//printf("\nel valor de la variable total es:%d\n",total);
	pthread_exit(NULL);
}
int main() {
	int nh,j,a[1];
	a[0]=10;
	nh = 10;
	pthread_t h[nh];
	total = 10000;
	c = 10000;
	sem_init(&sem, 0, 1);
	for(t=0; t < nh; t++){
		pthread_create(&h[t], NULL, hilo0, (void *) (&a[0]));
	}
	for(t=0; t < nh; t++){
		pthread_join(h[t],NULL);
	}
	sem_destroy(&sem);
	printf("total = %d\n",total);
	pthread_exit(NULL);
}
/*
En consola vemos
total= [1010000]
*/
