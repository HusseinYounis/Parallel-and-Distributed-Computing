# Thread Creation and Message Passing Example

This example demonstrates how to create multiple threads in C++ using POSIX threads (pthreads) and pass messages to each thread using a structured approach.

## Code Overview

### Includes

```cpp
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <iostream>
```

- `#define HAVE_STRUCT_TIMESPEC`: Ensures that the `timespec` structure is defined for compatibility.
- `#include <pthread.h>`: Provides the necessary functions and types for using POSIX threads.
- `#include <iostream>`: Allows the use of input and output streams.

### Thread Data Structure

```cpp
struct ThreadData {
    int thread_id;
    const char* message;
};
```

- **Description**: A structure `ThreadData` that holds the thread ID and the message to be printed by each thread.

### Thread Function

```cpp
void* thread_job(void* ptr) {
    ThreadData* data = static_cast<ThreadData*>(ptr);  // Cast the argument to ThreadData*
    std::cout << "Thread " << data->thread_id << ": " << data->message << std::endl;
    return nullptr;
}
```

- **Function**: `thread_job`
  - **Parameters**: `void* ptr` (pointer to `ThreadData`)
  - **Returns**: `nullptr`
  - **Description**: This function receives a pointer to `ThreadData`, casts it, and prints the thread ID along with the associated message.

### Main Function

```cpp
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
```

- **Thread Creation**:
  - Two threads are created using `pthread_create`, each assigned a unique message via the `ThreadData` structure.
  
- **Error Handling**:
  - If thread creation fails, an error message is printed, and the program exits.

- **Thread Joining**:
  - The `pthread_join` function is used to wait for both threads to finish execution before the main program exits.

## Output Example

When the program runs, it may produce output similar to the following:

```
Thread 1: Hello from thread 1
Thread 2: Hello from thread 2
```

This demonstrates the successful creation of threads and the passing of messages to each thread.
