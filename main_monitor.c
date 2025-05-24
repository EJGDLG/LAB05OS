#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TOTAL_RECURSOS 50
#define NUM_THREADS 5
#define ITERACIONES 2
#define CONSUMO 21

int recursos = TOTAL_RECURSOS;
pthread_mutex_t mutex;
pthread_cond_t cond;

int decrease_count(int count) {
    if (recursos < count)
        return -1;
    else {
        recursos -= count;
        return 0;
    }
}

void increase_count(int count) {
    recursos += count;
}

void *usar_monitor(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < ITERACIONES; i++) {
        pthread_mutex_lock(&mutex);
        printf("%d - Mutex adquirido, entrando al monitor\n", id);

        while (decrease_count(CONSUMO) == -1) {
            printf("NUAY! Recursos actuales: %d\n", recursos);
            pthread_cond_wait(&cond, &mutex);
        }

        printf("%d - Se consumieron %d recursos\n", id, CONSUMO);
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 3 + 1);

        pthread_mutex_lock(&mutex);
        increase_count(CONSUMO);
        pthread_cond_signal(&cond);
        printf("%d - Recursos devueltos: %d\n", id, CONSUMO);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main() {
    printf("Iniciando programa\n");
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, usar_monitor, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
