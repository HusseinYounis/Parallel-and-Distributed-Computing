#include <iostream>
#include <pthread.h>
#include <unistd.h>

int counter = 0;
pthread_mutex_t mutex;

void* increment_counter(void* arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex); // Lock the mutex
        counter++;                  // Critical section
        pthread_mutex_unlock(&mutex); // Unlock the mutex
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    pthread_mutex_init(&mutex, NULL); // Initialize mutex

    pthread_create(&thread1, NULL, increment_counter, NULL);
    pthread_create(&thread2, NULL, increment_counter, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&mutex); // Cleanup

    printf("Final counter value: %d\n", counter); // Correct: 200000
    return 0;
}