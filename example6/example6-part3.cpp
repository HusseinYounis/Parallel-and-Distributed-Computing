#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <iostream>
#include <string>


const int NUM_THREADS = 5; // Number of threads
std::string messages[NUM_THREADS];
std::string destinations[NUM_THREADS];


// Thread function
void* thread_job(void* ptr) {
    int* id = static_cast<int*>(ptr); // Cast the argument to ThreadData*
    int next_thread = (*id + 1) % NUM_THREADS;
    destinations[next_thread] = messages[*id]; // Pass the message to the next thread
    std::cout << "Thread " << *id << " passed message to Thread " << next_thread << ": " << messages[*id] << std::endl;
    return nullptr;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_idX[NUM_THREADS];
    

    // Initialize messages
    for (int i = 0; i < NUM_THREADS; ++i) {
        messages[i] = "Message from thread " + std::to_string(i);
    }

    // Create threads
    for (int i = 0; i < NUM_THREADS; ++i) {
       
        thread_idX[i] = i;
        if (pthread_create(&threads[i], nullptr, thread_job, &thread_idX[i]) != 0) {
            std::cerr << "Error creating thread " << i << std::endl;
            return 1;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    // Print final messages in destinations
    std::cout << "\nFinal messages in destinations array:" << std::endl;
    for (int i = 0; i < NUM_THREADS; ++i) {
        std::cout << "Thread " << i << " received: " << destinations[i] << std::endl;
    }

    return 0;
}