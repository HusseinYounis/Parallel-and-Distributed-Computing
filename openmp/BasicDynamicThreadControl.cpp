#include <omp.h>
#include <iostream>

int main() {
    omp_set_dynamic(1);  // Enable dynamic adjustment
    omp_set_num_threads(21);
    
    #pragma omp parallel
    {
        #pragma omp critical
        std::cout << "Dynamic ON: Thread " << omp_get_thread_num() 
                  << "/" << omp_get_num_threads() << "\n";
    }

    omp_set_dynamic(0);  // Disable dynamic adjustment
    omp_set_num_threads(21);
    
    #pragma omp parallel
    {
        #pragma omp critical
        std::cout << "Dynamic OFF: Thread " << omp_get_thread_num() 
                  << "/" << omp_get_num_threads() << "\n";
    }
}