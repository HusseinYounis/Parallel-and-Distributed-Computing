#include <omp.h>
#include <iostream>
#include <chrono>

void parallel_work() {
    #pragma omp parallel
    {
        // CPU-intensive work
        volatile double sum = 0;
        for(int i=0; i<100000000; i++) {
            sum += i * 0.1;
        }
    }
}

int main() {
    const int max_threads = omp_get_max_threads();
    
    // Case 1: Dynamic enabled
    omp_set_dynamic(1);
    omp_set_num_threads(max_threads + 1);
    
    auto start1 = std::chrono::high_resolution_clock::now();
    parallel_work();
    auto end1 = std::chrono::high_resolution_clock::now();
    
    // Case 2: Dynamic disabled
    omp_set_dynamic(0);
    omp_set_num_threads(max_threads + 1);
    
    auto start2 = std::chrono::high_resolution_clock::now();
    parallel_work();
    auto end2 = std::chrono::high_resolution_clock::now();
    
    // Results
    std::cout << "Dynamic enabled (20 threads): " 
              << std::chrono::duration<double>(end1 - start1).count() 
              << "s\n";
    std::cout << "Dynamic disabled (21 threads): " 
              << std::chrono::duration<double>(end2 - start2).count() 
              << "s\n";
}