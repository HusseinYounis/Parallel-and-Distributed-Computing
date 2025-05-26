#include <omp.h>
#include <iostream>

int main()
{
int counter = 0;
#pragma omp parallel shared(counter)
{
    #pragma omp atomic
    counter++; // Requires atomic to avoid race conditions
}
printf("Counter: %d\n", counter); // Output: Number of threads
    return 0;
}