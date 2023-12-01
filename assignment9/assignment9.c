#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int total_threads;
} pthread_barrier_t;

int pthread_barrier_init(pthread_barrier_t *barrier, int total_threads) {
    pthread_mutex_init(&barrier->mutex, NULL);
    pthread_cond_init(&barrier->cond, NULL);
    barrier->count = 0;
    barrier->total_threads = total_threads;
    return 0;
}

int pthread_barrier_wait(pthread_barrier_t *barrier) {
    pthread_mutex_lock(&barrier->mutex);
    barrier->count++;
    if (barrier->count == barrier->total_threads) {
        barrier->count = 0; // Reset for next use
        pthread_cond_broadcast(&barrier->cond);
    } else {
        while (pthread_cond_wait(&barrier->cond, &barrier->mutex) != 0);
    }
    pthread_mutex_unlock(&barrier->mutex);
    return 0;
}

int pthread_barrier_destroy(pthread_barrier_t *barrier) {
    pthread_mutex_destroy(&barrier->mutex);
    pthread_cond_destroy(&barrier->cond);
    return 0;
}

#define NUM_THREADS 5

pthread_barrier_t barrier;

void *thread_function(void *id) {
    pthread_barrier_wait(&barrier);
    printf("Thread %ld running\n", (long)pthread_self());
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    pthread_barrier_init(&barrier, NUM_THREADS);

    for(int i = 0; i < NUM_THREADS; i++) {
        printf("Starting thread %d\n", i);
        int rc = pthread_create(&threads[i], NULL, thread_function, NULL);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    
    return 0;
}
