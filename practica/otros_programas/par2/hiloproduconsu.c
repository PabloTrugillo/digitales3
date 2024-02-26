//Este programa es la implementación productor-consumidor sincronizado con semáforos sin nombre.
//El hilo main crea al hilo consumidor y es el hilo productor

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#define C 2
#define V 10
static int b[C] = {0};
pthread_t th1, th2;
sem_t sem_dato, sem_lugar;
void prd(void) {
int dato, i, pos;
pos = 0;
dato = 100;
for(i=0; i < V; i++ ) {
[//linea en blanco]
sem_wait(&sem_lugar);
dato++;
b[pos] = dato;
pos++;
if (pos >= C) {pos=0;}
printf("Produce dato: %d\n", dato);
[sem_post(&sem_dato);]
} p
thread_exit(0);
} i
nt main() {
int dato, i, pos;
[sem_init(&sem_lugar, 0, C);] //inicilizar semáforo sem_lugar
[sem_init(&sem_dato, 0, 0);] //inicilizar semáforo sem_dato
pthread_create(&th1, NULL, (void*)&prd, NULL);
pos = 0;
for(i=0; i < V; i++ ) {
[sem_wait(&sem_dato);]
dato = b[pos];
pos++ ;
if (pos>= C) {pos=0;}
printf("Consume dato: %d\n", dato);
sem_post(&sem_lugar);
[//linea en blanco]
} p
thread_exit(0);
}
