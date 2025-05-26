#include <omp.h>
#include <iostream>

int main() {
    #pragma omp parallel for schedule(static) num_threads(2)
for (int i = 0; i < 10; i++) {
    printf("Thread %d: i=%d\n", omp_get_thread_num(), i);
}
    return 0;
}