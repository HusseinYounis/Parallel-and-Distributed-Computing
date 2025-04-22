# Multithreading Example in C++

This C++ program demonstrates the use of POSIX threads (pthreads) to create and manage multiple threads. It includes a function that prints the thread ID of each created thread.

## Code Overview

### Includes

```cpp
#include <iostream>
#include <pthread.h>
```

- `#include <iostream>`: Allows the use of input and output streams.
- `#include <pthread.h>`: Provides the necessary functions and types for using POSIX threads.

### Thread Function

```cpp
void* fun(void* arg) {
    std::cout << "Thread ID from threadFunction: " << pthread_self() << std::endl;
    return NULL;
}
```

- **Function**: `fun`
  - **Parameters**: `void* arg` (not used in this example)
  - **Returns**: `NULL`
  - **Description**: This function prints the ID of the thread that is executing it using `pthread_self()`.

### Main Function

```cpp
int main() {
    const int numThreads = 5;
    pthread_t threads[numThreads];
    for (int i = 0; i < numThreads; ++i) {
        pthread_create(&threads[i], NULL, fun, NULL);
    }

    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Main thread exiting.\n");
    std::cout << "Main thread ID " << pthread_self() << std::endl;
    return 0;
}
```

- **Variables**:
  - `const int numThreads = 5`: Number of threads to create.
  - `pthread_t threads[numThreads]`: Array to hold thread identifiers for the created threads.
  
- **Thread Creation**:
  - The `for` loop iterates from `0` to `numThreads - 1`, creating a new thread with `pthread_create(&threads[i], NULL, fun, NULL)` for each iteration.

- **Thread Joining**:
  - The second `for` loop waits for each created thread to finish executing using `pthread_join(threads[i], NULL)`.

- **Output**:
  - The program prints the ID of each thread as it runs and the ID of the main thread before exiting.
