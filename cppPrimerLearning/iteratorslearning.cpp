//
// Created by ethan on 10-Aug-26.
//

#include <iostream>
#include <vector>

int main() {
    std::vector<double> v = {10.5, 20.0, 3.25, 7.0};

    double total = 0.0;   // <-- you write the loop below this line
    // Use an iterator loop (begin/end/*it/++it) to add every element into total.

    for (auto it=v.begin(); it != v.end(); ++it){
        total+=*it;}

    std::cout << "sum = " << total << '\n';
    return 0;
}