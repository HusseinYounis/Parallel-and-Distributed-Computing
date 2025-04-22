# Multithreading Example in C++

This C++ program demonstrates the use of POSIX threads (pthreads) to create and manage multiple threads. Each thread simulates work by sleeping for a short duration before printing its thread number.

## Code Overview

### Includes

```cpp
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <iostream>
#include <unistd.h>
```

- `#define HAVE_STRUCT_TIMESPEC`: Ensures compatibility with certain systems regarding the `timespec` structure.
- `#include <pthread.h>`: Provides the necessary functions and types for using POSIX threads.
- `#include <iostream>`: Allows the use of input and output streams.
- `#include <unistd.h>`: Provides access to the POSIX operating system API, including the `usleep` function.

### Thread Function

```cpp
void* thread_job(void* ptr) {
    int thread_num = *((int*)ptr);  // Get the thread number
    usleep(200000); // Simulate some work
    std::cout << "Thread number " << thread_num << " is running." << std::endl;
    return nullptr;
}
```

- **Function**: `thread_job`
  - **Parameters**: `void* ptr` (pointer to the thread number)
  - **Returns**: `nullptr`
  - **Description**: This function simulates work by sleeping for 200 milliseconds and then prints the thread number.

### Main Function

```cpp
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
```

- **Variables**:
  - `int num_threads`: Number of threads to create, input by the user.
  - `pthread_t* threads`: Dynamic array to hold thread identifiers.
  - `int* thread_args`: Dynamic array to store arguments for each thread.

- **Thread Creation**:
  - The `for` loop iterates from `0` to `num_threads - 1`, creating a new thread with `pthread_create(&threads[i], nullptr, thread_job, &thread_args[i])` for each iteration.
  - Each thread receives its number as an argument.

- **Thread Joining**:
  - A second `for` loop waits for each created thread to finish executing using `pthread_join(threads[i], nullptr)`.

- **Cleanup**:
  - Dynamically allocated arrays for threads and arguments are deleted to free memory.

- **Output**:
  - The program prompts the user for the number of threads, creates the specified number of threads, and prints messages indicating the creation and execution of each thread.
