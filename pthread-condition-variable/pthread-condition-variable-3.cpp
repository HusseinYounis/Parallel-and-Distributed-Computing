#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 10

// Global variables
int array[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int left_sum = 0;
int right_sum = 0;
int total_sum = 0;

pthread_mutex_t mutex;
pthread_cond_t cond_var;
int ready = 0; // Condition variable flag

// Function to sum the left half of the array
void* sum_left(void* arg) {
    for (int i = 0; i < ARRAY_SIZE / 2; i++) {
        left_sum += array[i];
    }
    
    pthread_mutex_lock(&mutex);
    ready++;
    pthread_cond_signal(&cond_var); // Signal that left sum is ready
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

// Function to sum the right half of the array
void* sum_right(void* arg) {
    for (int i = ARRAY_SIZE / 2; i < ARRAY_SIZE; i++) {
        right_sum += array[i];
    }
    
    pthread_mutex_lock(&mutex);
    ready++;
    pthread_cond_signal(&cond_var); // Signal that right sum is ready
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

// Function to sum the results from both halves
void* sum_total(void* arg) {
    pthread_mutex_lock(&mutex);
    while (ready < 2) { // Wait until both sums are ready
        pthread_cond_wait(&cond_var, &mutex);
    }
    total_sum = left_sum + right_sum;
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

int main() {
    pthread_t threads[3];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_var, NULL);

    // Create threads for summing left and right halves
    pthread_create(&threads[0], NULL, sum_left, NULL);
    pthread_create(&threads[1], NULL, sum_right, NULL);
    pthread_create(&threads[2], NULL, sum_total, NULL);

    // Join threads
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Left Sum: %d\n", left_sum);
    printf("Right Sum: %d\n", right_sum);
    printf("Total Sum: %d\n", total_sum);

    // Clean up
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);

    return 0;
}