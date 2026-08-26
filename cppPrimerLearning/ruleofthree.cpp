#include <iostream>
#include <utility>   // for std::move

class Tracer {
public:
    Tracer() { std::cout << "constructor\n"; }
    Tracer(const Tracer& other) { std::cout << "COPY constructor\n"; }
    Tracer(Tracer&& other) { std::cout << "MOVE constructor\n"; }   // NEW — note the &&
    Tracer& operator=(const Tracer& other) {
        std::cout << "copy ASSIGNMENT\n";
        return *this;
    }
    ~Tracer() { std::cout << "destructor\n"; }
};

Tracer makeTracer() {
    Tracer local;
    return local;         // ?
}

int main() {
    std::cout << "-- line A --\n";
    Tracer x;                        // constructor

    std::cout << "-- line B --\n";
    Tracer y = x;                    // copy constructor

    std::cout << "-- line C --\n";
    Tracer z = std::move(x);         //  move constructor

    std::cout << "-- line D --\n";
    Tracer w = makeTracer();         // constructor

    std::cout << "-- line E --\n";
    return 0;                           // destructor for all
}