#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 100

int array[ARRAY_SIZE];
int global_max = 0;  // Initialize to minimum possible value
pthread_mutex_t max_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int start;
    int end;
} ThreadArgs;

void* find_max(void* args) {
    ThreadArgs* targs = (ThreadArgs*)args;
    int local_max = array[targs->start];
    // Find local max in the thread's chunk
    for (int i = targs->start; i < targs->end; i++) {
        if (array[i] > local_max) {
            local_max = array[i];
        }
    }
    // Safely update global max
    pthread_mutex_lock(&max_mutex);
    if (local_max > global_max) {
        global_max = local_max;
    }
    pthread_mutex_unlock(&max_mutex);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadArgs targs[NUM_THREADS];
    int chunk_size = ARRAY_SIZE / NUM_THREADS;

    // Initialize array with random values
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 1000;
    }

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        targs[i].start = i * chunk_size;
        targs[i].end = (i == NUM_THREADS - 1) ? ARRAY_SIZE : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, find_max, &targs[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Global maximum: %d\n", global_max);
    return 0;
}