#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <iostream>


// Structure to hold the message for each thread
struct ThreadData {
    int thread_id;
    const char* message;
};

// Thread function that prints the message
void* thread_job(void* ptr) {
    ThreadData* data = static_cast<ThreadData*>(ptr);  // Cast the argument to ThreadData*
    std::cout << "Thread " << data->thread_id << ": " << data->message << std::endl;
    return nullptr;
}

int main() {
    pthread_t thread1, thread2;

    // Prepare messages for each thread
    ThreadData data1 = {1, "Hello from thread 1"};
    ThreadData data2 = {2, "Hello from thread 2"};

    // Create the first thread
    if (pthread_create(&thread1, nullptr, thread_job, &data1) != 0) {
        std::cerr << "Error creating thread 1" << std::endl;
        return 1;
    }

    // Create the second thread
    if (pthread_create(&thread2, nullptr, thread_job, &data2) != 0) {
        std::cerr << "Error creating thread 2" << std::endl;
        return 1;
    }

    // Wait for both threads to finish
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    return 0;
}