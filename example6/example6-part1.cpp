#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <iostream>
#include <vector>
#include <string>


const int NUM_THREADS = 5; // Number of threads

// Structure to hold thread data
struct ThreadData {
    int thread_id;
    std::string message;
    std::string* destination;
};

// Thread function
void* thread_job(void* ptr) {
    ThreadData* data = static_cast<ThreadData*>(ptr); // Cast the argument to ThreadData*
    int next_thread = (data->thread_id + 1) % NUM_THREADS; // Calculate the next thread index
    data->destination[next_thread] = data->message; // Pass the message to the next thread
    std::cout << "Thread " << data->thread_id << " passed message to Thread " << next_thread << ": " << data->message << std::endl;
    return nullptr;
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    std::vector<std::string> messages(NUM_THREADS);
    std::vector<std::string> destinations(NUM_THREADS);

    // Initialize messages
    for (int i = 0; i < NUM_THREADS; ++i) {
        messages[i] = "Message from thread " + std::to_string(i);
    }

    // Create threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_data[i] = {i, messages[i], destinations.data()};
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