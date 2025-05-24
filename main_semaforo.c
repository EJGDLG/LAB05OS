#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TOTAL_RECURSOS 3
#define NUM_THREADS 5
#define ITERACIONES 3

int recursos_disponibles = TOTAL_RECURSOS;
sem_t semaforo;

void *usar_recurso(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < ITERACIONES; i++) {
        printf("%d - Esperando semáforo...\n", id);
        sem_wait(&semaforo); // bloquear si no hay recurso

        printf("%d - (!) Recurso tomado\n", id);
        recursos_disponibles--;
        printf("%d - Recursos disponibles: %d\n", id, recursos_disponibles);

        sleep(rand() % 2 + 1); // simula uso

        recursos_disponibles++;
        printf("%d - Recurso devuelto :)\n", id);
        sem_post(&semaforo); // liberar semáforo
    }
    pthread_exit(NULL);
}

int main() {
    printf("Iniciando programa\n");
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    sem_init(&semaforo, 0, TOTAL_RECURSOS);

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, usar_recurso, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&semaforo);
    return 0;
}
