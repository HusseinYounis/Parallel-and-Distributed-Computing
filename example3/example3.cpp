#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <iostream>
#include <unistd.h> 
void* thread_job(void* ptr) {
    int thread_num = *((int*)ptr);  // Get the thread number
    usleep(200000); // Simulate some work
    std::cout << "Thread number " << thread_num << " is running." << std::endl;
    return nullptr;
}

int main() {
    int num_threads;
    std::cout << "Enter the number of threads: ";
    std::cin >> num_threads;

    pthread_t* threads = new pthread_t[num_threads];  // Dynamic array of threads
    int* thread_args = new int[num_threads];  // Array to store arguments for threads

    // Create the threads
    for (int i = 0; i < num_threads; ++i) {
        thread_args[i] = i + 1;  // Thread number (or any other argument you want to pass)
        if (pthread_create(&threads[i], nullptr, thread_job, &thread_args[i]) != 0) {
            std::cerr << "Error creating thread " << i + 1 << std::endl;
        }
        else {
            std::cout << "Thread " << i + 1 << " created successfully." << std::endl;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    // Cleanup
    delete[] threads;
    delete[] thread_args;

    return 0;
}