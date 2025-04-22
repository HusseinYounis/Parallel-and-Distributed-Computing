#include <iostream>
#include <pthread.h>

void* fun(void* arg) {
    std::cout << "Thread ID from threadFunction: " << pthread_self() << std::endl;
    return NULL;
}
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