#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_PRODUCTS 5

// Shared data structure
typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t parts_ready;
    pthread_cond_t new_product;
    int current_product;
    int parts_completed;
} AssemblyControl;

AssemblyControl assembly = {
    .lock = PTHREAD_MUTEX_INITIALIZER,
    .parts_ready = PTHREAD_COND_INITIALIZER,
    .new_product = PTHREAD_COND_INITIALIZER,
    .current_product = 0,
    .parts_completed = 0
};

void* part_a_worker(void* arg) {
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        pthread_mutex_lock(&assembly.lock);
        
        // Wait until new product starts
        while (assembly.current_product != i) {
            pthread_cond_wait(&assembly.new_product, &assembly.lock);
        }
        
        printf("Part A prepared for product %d\n", i+1);
        assembly.parts_completed++;
        
        if (assembly.parts_completed == 3) {
            pthread_cond_signal(&assembly.parts_ready);
        }
        
        pthread_mutex_unlock(&assembly.lock);
    }
    return NULL;
}

void* part_b_worker(void* arg) {
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        pthread_mutex_lock(&assembly.lock);
        // Wait until new product starts
        while (assembly.current_product != i) {
            pthread_cond_wait(&assembly.new_product, &assembly.lock);
        }
        printf("Part B prepared for product %d\n", i+1);
        assembly.parts_completed++;
        if (assembly.parts_completed == 3) {
            pthread_cond_signal(&assembly.parts_ready);
        }
        pthread_mutex_unlock(&assembly.lock);
    }
    return NULL;
}

void* part_c_worker(void* arg) {
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        pthread_mutex_lock(&assembly.lock);
        // Wait until new product starts
        while (assembly.current_product != i) {
            pthread_cond_wait(&assembly.new_product, &assembly.lock);
        }
        printf("Part C prepared for product %d\n", i+1);
        assembly.parts_completed++;
        if (assembly.parts_completed == 3) {
            pthread_cond_signal(&assembly.parts_ready);
        }
        pthread_mutex_unlock(&assembly.lock);
    }
    return NULL;
}

int main() {
    pthread_t workers[3];
    
    // Create worker threads
    pthread_create(&workers[0], NULL, part_a_worker, NULL);
    pthread_create(&workers[1], NULL, part_b_worker, NULL);
    pthread_create(&workers[2], NULL, part_c_worker, NULL);

    for (int i = 0; i < NUM_PRODUCTS; i++) {
        pthread_mutex_lock(&assembly.lock);
        
        // Signal new product start
        assembly.current_product = i;
        pthread_cond_broadcast(&assembly.new_product);
        
        // Wait for all parts to be ready
        while (assembly.parts_completed < 3) {
            pthread_cond_wait(&assembly.parts_ready, &assembly.lock);
        }
        
        printf("Assembling product %d\n", i+1);
        assembly.parts_completed = 0;  // Reset for next product
        
        pthread_mutex_unlock(&assembly.lock);
    }

    // Cleanup
    for (int i = 0; i < 3; i++) {
        pthread_join(workers[i], NULL);
    }

    return 0;
}