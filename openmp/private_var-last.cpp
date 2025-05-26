#include <omp.h>
#include <iostream>

int main()
{
int last_val = -1;
#pragma omp parallel for lastprivate(last_val)
for (int i = 0; i < 5; i++) {
    last_val = i; // Last iteration (i=4) sets last_val=4
}
printf("Last value: %d\n", last_val); // Output: 4
    return 0;
}