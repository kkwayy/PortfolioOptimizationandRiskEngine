//
// Created by ethan on 15-Aug-26.
//

#include <iostream>
#include <vector>

int main() {
    std::vector<double> v;
    std::vector<double> v2;
    v2.reserve(20);


    for (int i = 0; i < 20; ++i) {
        v.push_back(i);
        v2.push_back(i);
        std::cout << "size=" << v.size() << "  capacity=" << v.capacity() << '\n';
        std::cout << " v2: size=" << v2.size() << " v2  capacity=" << v2.capacity() << '\n';
    }

    return 0;
}