#include <iostream>
#include <omp.h>
#include <sstream>
#include <string>

int main() {
    const int N = 20; // Total number of iterations
    std::ostringstream output; // Create a string stream to collect output

    // Parallel for loop
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        int thread_id = omp_get_thread_num(); // Get the thread ID
        
        // Collect output in a thread-safe manner
        #pragma omp critical
        {
            output << "Thread " << thread_id << " processing index: " << i << std::endl;
        }
    }

    // Print the collected output
    std::cout << output.str() << std::endl;

    return 0;
}