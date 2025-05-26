#include <omp.h>
#include <iostream>

int main()
{
int base = 10;
#pragma omp parallel firstprivate(base)
{
    base += omp_get_thread_num(); // Starts from 10 in each thread
    printf("Thread %d: base=%d\n", omp_get_thread_num(), base);
}
    return 0;
}