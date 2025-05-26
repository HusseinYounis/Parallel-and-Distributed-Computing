#include <omp.h>
#include <iostream>

int main()
{
int count = 0;
#pragma omp parallel private(count)
{
    count = omp_get_thread_num(); // Each thread initializes its own copy
    printf("Thread %d: count=%d\n", omp_get_thread_num(), count);
}
printf("Final count: %d\n", count); // Output: 0 (original unchanged)
    return 0;
}