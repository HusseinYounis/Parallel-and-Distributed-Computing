#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <iostream>
#include <string>


const int NUM_THREADS = 5; // Number of threads

// Structure to hold thread data
struct ThreadData {
    int thread_id;
    std::string*  message;
    std::string* destination;
};

// Thread function
void* thread_job(void* ptr) {
    ThreadData* data = static_cast<ThreadData*>(ptr); // Cast the argument to ThreadData*
    *(data->destination) = *(data->message); // Pass the message to the next thread
    std::cout << "Thread " << data->thread_id << " passed message to Thread " << (data->thread_id + 1) % NUM_THREADS << ": " << *(data->message) << std::endl;
    return nullptr;
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    std::string messages[NUM_THREADS];
    std::string destinations[NUM_THREADS];

    // Initialize messages
    for (int i = 0; i < NUM_THREADS; ++i) {
        messages[i] = "Message from thread " + std::to_string(i);
    }

    // Create threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        int next_thread = (i + 1) % NUM_THREADS;
        thread_data[i] = {i, &messages[i], &destinations[next_thread]};
        if (pthread_create(&threads[i], nullptr, thread_job, &thread_data[i]) != 0) {
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