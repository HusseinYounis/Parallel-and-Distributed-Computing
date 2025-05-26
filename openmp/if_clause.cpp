#include <omp.h>
#include <iostream>

int main() {
    int condition = 0; // 0 means false
    
    #pragma omp parallel if(condition) 
    {
        #pragma omp critical
        std::cout << "Thread " << omp_get_thread_num() << "/" 
                  << omp_get_num_threads() << " is active\n";
    }
    
    condition = 1; // Now true
    #pragma omp parallel if(condition) 
    {
        #pragma omp critical
        std::cout << "Thread " << omp_get_thread_num() << "/" 
                  << omp_get_num_threads() << " is active\n";
    }
}