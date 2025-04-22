// Example 3: Thread Creation and Joining with Global Variable Modification
#include <pthread.h>
#include <iostream>
#include <unistd.h>

int global_var = 0;  // Global variable to be modified by the thread

void* thread_job(void* ptr) {
    
    usleep(2000000);  // Sleep for 2 seconds to simulate work
    global_var += 10;  // Modify the global variable
    return nullptr;
}

int main() {
    pthread_t thread1;

    // Create the thread
    if (pthread_create(&thread1, nullptr, thread_job, nullptr) != 0) {
        std::cerr << "Error creating thread" << std::endl;
        return 1;
    }

    std::cout << "The value of global_var is " << global_var << std::endl;

    // Wait for the thread to finish its job
    pthread_join(thread1, nullptr);

    // Print the value of global_var after the thread has finished
    std::cout << "After thread join: global_var = " << global_var << std::endl;

    return 0;
}