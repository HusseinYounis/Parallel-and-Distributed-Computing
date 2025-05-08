#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>  // Requires C11 mode!

#define NUM_THREADS 4
#define ARRAY_SIZE 100

int array[ARRAY_SIZE];
atomic_int global_max;  // Use C11 atomic type

typedef struct {
    int start;
    int end;
} ThreadArgs;

void* find_max(void* args) {
    ThreadArgs* targs = (ThreadArgs*)args;
    int local_max = array[targs->start];

    for (int i = targs->start; i < targs->end; i++) {
        if (array[i] > local_max) {
            local_max = array[i];
        }
    }

    // Atomic update using CAS
    int current_max;
    do {
        current_max = atomic_load(&global_max);
        if (local_max <= current_max) break;
    } while (!atomic_compare_exchange_strong(&global_max, &current_max, local_max));

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadArgs targs[NUM_THREADS];
    int chunk_size = ARRAY_SIZE / NUM_THREADS;

    // Initialize array and global_max
    for (int i = 0; i < ARRAY_SIZE; i++) array[i] = rand() % 1000;
    atomic_init(&global_max, 0);  // Explicit initialization

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        targs[i].start = i * chunk_size;
        targs[i].end = (i == NUM_THREADS - 1) ? ARRAY_SIZE : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, find_max, &targs[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) pthread_join(threads[i], NULL);

    printf("Global maximum: %d\n", global_max);
    return 0;
}