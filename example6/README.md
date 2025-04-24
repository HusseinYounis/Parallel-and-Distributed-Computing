# Multithreading Example Documentation

This repository contains examples of multithreading in C++ using the POSIX threads (pthreads) library. Each file demonstrates different approaches to passing messages between threads.

## Files Overview

### 1. `example6-part1.cpp`
This file demonstrates a simple implementation of multithreading where each thread passes its message to the next thread in a circular manner.

#### Key Features:
- Uses a structure to hold thread data.
- Each thread sends its message to the next thread.
- Final messages are printed after all threads have completed execution.

```cpp
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


# Multithreading Example - Part 2

This document provides an overview of the `example6-part2.cpp` file, which demonstrates multithreading in C++ using the POSIX threads (pthreads) library. This implementation focuses on passing messages between threads using pointers.

## File Overview

### `example6-part2.cpp`

This file modifies the approach by using pointers to strings for message passing between threads.

#### Key Features:
- Utilizes pointers to manage messages and destinations.
- Each thread writes directly to the destination of the next thread.
- Similar message passing logic as in the previous example.

#### Code

```cpp
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <iostream>
#include <string>

const int NUM_THREADS = 5; // Number of threads

// Structure to hold thread data
struct ThreadData {
    int thread_id;
    std::string* message;
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

# Multithreading Example - Part 3

This document provides an overview of the `example6-part3.cpp` file, which demonstrates multithreading in C++ using the POSIX threads (pthreads) library. This implementation simplifies message passing by using global arrays.

## File Overview

### `example6-part3.cpp`

This file simplifies the message passing by using an array of integers to represent thread IDs.

#### Key Features:
- Directly uses global arrays for messages and destinations.
- Each thread passes its message to the next using the thread ID array.
- Maintains similar functionality as previous examples.

#### Code

```cpp
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <iostream>
#include <string>

const int NUM_THREADS = 5; // Number of threads
std::string messages[NUM_THREADS];
std::string destinations[NUM_THREADS];

// Thread function
void* thread_job(void* ptr) {
    int* id = static_cast<int*>(ptr); // Cast the argument to int*
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