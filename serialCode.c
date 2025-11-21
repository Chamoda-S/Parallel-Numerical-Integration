#include <stdio.h>
#include <math.h>

double f(double x) {
    return sin(x); // Example function
}

double trapezoidal_rule(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = (f(a) + f(b)) / 2.0;

    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        sum += f(x);
    }

    return sum * h;
}

int main() {
    double a = 0.0, b = M_PI;
    int n = 1000000;
    double result = trapezoidal_rule(a, b, n);
    printf("Approximate integral: %f\n", result);
    return 0;
}
