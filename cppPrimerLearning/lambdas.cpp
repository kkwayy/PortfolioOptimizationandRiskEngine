//
// Created by ethan on 15-Aug-26.
//


#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<double> returns = {0.05, -0.02, 0.03, -0.01, 0.08, 0.01};

    // PART A — count how many returns exceed a threshold you capture.
    double threshold = 0.02;

    auto num_returns = std::count_if(returns.begin(),returns.end(),[threshold](double x){return x > threshold; }  ) ;
    // Use std::count_if with a lambda that captures `threshold` and
    // returns whether an element is greater than it. Print the count.


    // PART B — sort `returns` in DESCENDING order using std::sort with a
    // comparator lambda. std::sort calls your lambda with two elements (a, b);
    // return true when `a` should come BEFORE `b`. For descending, that's when a > b.
    // After sorting, print the vector.

    std::sort(returns.begin(), returns.end(), [](double a, double b) { return a > b; });

    for (double r : returns) {
        std::cout << r << ' ';
    }
    std::cout << '\n';

    return 0;
}