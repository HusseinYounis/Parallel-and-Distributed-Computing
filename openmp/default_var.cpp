#include <omp.h>
#include <iostream>

int main()
{
int a = 1, b = 2;
#pragma omp parallel default(none) shared(a) private(b)
{
    // Must declare all variables explicitly
    printf("a=%d\n", a); // OK: a is shared
    // printf("b=%d\n", b); // Error: b is private (uninitialized)
}
    return 0;
}