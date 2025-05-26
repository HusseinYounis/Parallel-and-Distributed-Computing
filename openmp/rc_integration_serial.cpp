#include <iostream>
#include <math.h>   // For sin function
#include <numbers> // For mathematical constants
#include <chrono> 
// Function we are calculating the area under
double f(double x) {
    return sin(x);
}

int main(int argc, char** argv) {
    // Variables
    double a = 0.0, b = M_PI; // limits of integration
    int n = 1048576;                       // number of subdivisions = 2^20
    double h = (b - a) / n;                // width of each subdivision
    double integral;                       // accumulates answer

    integral = (f(a) + f(b)) / 2.0; // initial value

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();
    // Sum up all the trapezoids
    for (int i = 1; i < n; i++) {
        integral += f(a + i * h);
    }
 // End timing
 auto end = std::chrono::high_resolution_clock::now();
    integral = integral * h;
    std::cout << "With " << n << " trapezoids, our estimate of the integral from " << std::endl;
    std::cout << a << " to " << b << " is " << integral << std::endl;
    
    
     // Calculate duration
     std::chrono::duration<double> duration = end - start;
 
     std::cout << "Execution time: " << duration.count() << " seconds." << std::endl;

    return 0;
}
