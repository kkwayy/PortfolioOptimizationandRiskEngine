#include <iostream>
#include <memory>

int main() {
    // PART A
    std::unique_ptr<int> a = std::make_unique<int>(42);
    std::cout << *a << '\n';

    // PART B
    // std::unique_ptr<int> q = a;   // ERROR: use of deleted function (can't copy unique_ptr)
    std::unique_ptr<int> q = std::move(a);
    std::cout << "moved value: " << *q << '\n';

    if (a) {
        std::cout << "old pointer still owns something\n";
    } else {
        std::cout << "old pointer is now null\n";
    }

    // PART C
    std::shared_ptr<int> c = std::make_shared<int>(99);
    std::shared_ptr<int> d = c;                 // copying a shared_ptr IS allowed
    std::cout << "use_count: " << c.use_count() << '\n';   // 2

    return 0;
}