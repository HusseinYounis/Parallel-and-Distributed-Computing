#include <iostream>
#include <pthread.h>

void* fun(void* arg) {
    std::cout << "Thread ID from threadFunction: " << pthread_self() << std::endl;
    return NULL;
}
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