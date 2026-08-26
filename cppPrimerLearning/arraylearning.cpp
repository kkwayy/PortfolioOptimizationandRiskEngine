//
// Created by ethan on 10-Aug-26.
//

#include <iostream>

int main() {
    double a[4] = {10.5, 20.0, 3.25, 7.0};

    double total = 0.0;
    // Write the loop here: use a double* as the position,
    // walk from the front to the one-past-the-end marker,
    // and add each element into total.

    for (double* p =a ; p != a+4 ; ++p)
        total+= *p;



    std::cout << "sum = " << total << '\n';   // expect 40.75
    return 0;
}