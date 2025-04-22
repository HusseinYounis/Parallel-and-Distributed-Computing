# Thread Creation and Joining with Global Variable Modification

This example illustrates how to create a thread in C++ using POSIX threads (pthreads) and modify a global variable from within that thread.

## Code Overview

### Includes

```cpp
#include <pthread.h>
#include <iostream>
#include <unistd.h>
```

- `#include <pthread.h>`: Provides the necessary functions and types for using POSIX threads.
- `#include <iostream>`: Allows the use of input and output streams.
- `#include <unistd.h>`: Provides access to the POSIX operating system API, including the `usleep` function.

### Global Variable

```cpp
int global_var = 0;  // Global variable to be modified by the thread
```

- **Description**: A global variable `global_var` initialized to `0`, which will be modified by the thread.

### Thread Function

```cpp
void* thread_job(void* ptr) {
    usleep(2000000);  // Sleep for 2 seconds to simulate work
    global_var += 10;  // Modify the global variable
    return nullptr;
}
```

- **Function**: `thread_job`
  - **Parameters**: `void* ptr` (not used in this example)
  - **Returns**: `nullptr`
  - **Description**: This function simulates work by sleeping for 2 seconds and then increments the `global_var` by `10`.

### Main Function

```cpp
int main() {
    pthread_t thread1;

    // Create the thread
    if (pthread_create(&thread1, nullptr, thread_job, nullptr) != 0) {
        std::cerr << "Error creating thread" << std::endl;
        return 1;
    }

    std::cout << "The value of global_var is " << global_var << std::endl;

    // Wait for the thread to finish its job
    pthread_join(thread1, nullptr);

    // Print the value of global_var after the thread has finished
    std::cout << "After thread join: global_var = " << global_var << std::endl;

    return 0;
}
```

- **Thread Creation**:
  - The `pthread_create` function creates a new thread that executes the `thread_job` function.
  
- **Output Before Joining**:
  - The initial value of `global_var` is printed before the thread modifies it.

- **Thread Joining**:
  - The `pthread_join` function waits for the created thread to finish execution.

- **Output After Joining**:
  - The updated value of `global_var` is printed after the thread has completed its work.

## Output Example

When the program runs, it may produce output similar to the following:

```
The value of global_var is 0
After thread join: global_var = 10
```

This demonstrates the modification of the global variable by the thread after it has completed its execution.
