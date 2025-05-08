#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 10

// Global variables
int array[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int left_sum = 0;
int right_sum = 0;
int total_sum = 0;
int left_Ready = 0;
int right_ready = 0;

pthread_mutex_t mutex1, mutex2;

// Function to sum the left half of the array
void *sum_left(void *arg)
{
    for (int i = 0; i < ARRAY_SIZE / 2; i++)
    {
        pthread_mutex_lock(&mutex1);
        left_sum += array[i];
        pthread_mutex_unlock(&mutex1);
    }

    pthread_mutex_lock(&mutex2);
    left_Ready = 1;
    pthread_mutex_unlock(&mutex2);
    return NULL;
}

// Function to sum the right half of the array
void *sum_right(void *arg)
{
    for (int i = ARRAY_SIZE / 2; i < ARRAY_SIZE; i++)
    {
        pthread_mutex_lock(&mutex1);
        right_sum += array[i];
        pthread_mutex_unlock(&mutex1);
    }
    pthread_mutex_lock(&mutex2);
    right_ready = 1;
    pthread_mutex_unlock(&mutex2);
    return NULL;
}

// Function to sum the results from both halves
void *sum_total(void *arg)
{
    // Wait for the left and right sums to be calculated
    while (1)
    {
        pthread_mutex_lock(&mutex2);
        if (right_ready == 1 && left_Ready == 1)
        {
            pthread_mutex_lock(&mutex1);
            total_sum = left_sum + right_sum;
            pthread_mutex_unlock(&mutex1);
            pthread_mutex_unlock(&mutex2);
            break;
        }
        pthread_mutex_unlock(&mutex2);
    }

    return NULL;
}

int main()
{
    pthread_t threads[3];

    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    pthread_create(&threads[0], NULL, sum_left, NULL);
    pthread_create(&threads[1], NULL, sum_right, NULL);
    pthread_create(&threads[2], NULL, sum_total, NULL);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);

    printf("Left Sum: %d\n", left_sum);
    printf("Right Sum: %d\n", right_sum);
    printf("Total Sum: %d\n", total_sum);

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}