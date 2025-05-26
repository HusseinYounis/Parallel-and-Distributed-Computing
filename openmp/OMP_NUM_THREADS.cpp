#include <omp.h>
#include <iostream>

int main() {
    #pragma omp parallel
    {
        #pragma omp critical
        std::cout << "Thread " << omp_get_thread_num() << "/" 
                  << omp_get_num_threads() << " is active\n";
    }
}