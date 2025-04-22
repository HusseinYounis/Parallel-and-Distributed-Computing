# Multithreading Example in C++

This C++ program demonstrates the use of POSIX threads (pthreads) to create and manage multiple threads. It includes a simple function that prints the thread ID of each created thread.

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
    pthread_t th1;
    pthread_t th2;
    pthread_t th3;

    pthread_create(&th1, NULL, fun, NULL);
    pthread_create(&th2, NULL, fun, NULL);
    pthread_create(&th3, NULL, fun, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    printf("Main thread exiting.\n");
    std::cout << "Main thread ID " << pthread_self() << std::endl;
    return 0;
}
```

- **Variables**:
  - `pthread_t th1, th2, th3`: Thread identifiers for the three threads to be created.
  
- **Thread Creation**:
  - `pthread_create(&th1, NULL, fun, NULL)`: Creates the first thread running the `fun` function.
  - `pthread_create(&th2, NULL, fun, NULL)`: Creates the second thread.
  - `pthread_create(&th3, NULL, fun, NULL)`: Creates the third thread.

- **Thread Joining**:
  - `pthread_join(th1, NULL)`: Waits for the first thread to finish.
  - `pthread_join(th2, NULL)`: Waits for the second thread to finish.
  - `pthread_join(th3, NULL)`: Waits for the third thread to finish.

- **Output**:
  - The program prints the ID of each thread as it runs and the ID of the main thread before exiting.