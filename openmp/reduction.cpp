#include <omp.h>
#include <iostream>

int main()
{
int sum = 0;
#pragma omp parallel for reduction(+:sum)
for (int i = 0; i < 100; i++) {
    sum += i; // Each thread computes partial sum, combined at end
}
printf("Sum: %d\n", sum); // Correct result: 4950
    return 0;
}