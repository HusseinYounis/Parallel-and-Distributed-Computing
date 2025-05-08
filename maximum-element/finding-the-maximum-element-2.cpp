#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 100

int array[ARRAY_SIZE];
int global_max = 0;  // Initialize to minimum possible value

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
    // Atomically update global_max using CAS (GCC built-in)
    int current_max;
    do {
        current_max = global_max;
        if (local_max <= current_max) break;
    } while (!__sync_bool_compare_and_swap(&global_max, current_max, local_max));
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