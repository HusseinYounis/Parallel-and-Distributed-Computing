#include <iostream>
#include <vector>
#include <chrono> 
#include <omp.h>
#define N 10 // size of the array

int main() {
    // Use std::vector for automatic memory management
    std::vector<int> array(N); // declare array of size N

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();


    // Populate array
   // omp_set_num_threads(12);
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        std::cout <<i << std::endl;
        array[i] = i + 1;
    }

     // End timing
     auto end = std::chrono::high_resolution_clock::now();
    
     // Calculate duration
     std::chrono::duration<double> duration = end - start;
 
     std::cout << "Done populating " << N << " elements!" << std::endl;
     std::cout << "Filling time: " << duration.count() << " seconds." << std::endl;

     return 0;
}