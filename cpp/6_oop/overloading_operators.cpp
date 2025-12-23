/*
 * Lvalue and Rvalue Reference Explanation
 *
 * In C++, every expression is either an lvalue or an rvalue:
 *
 * 1. Lvalue:
 *    - Refers to an object that has a persistent memory location.
 *    - Can appear on the left-hand side of an assignment.
 *    - Example:
 *          int x = 5;  // 'x' is an lvalue
 *          x = 10;     // valid
 *    - Lvalue reference: T& ref = x;
 *
 * 2. Rvalue:
 *    - Refers to a temporary object or literal, usually short-lived.
 *    - Cannot be assigned to (no persistent memory location).
 *    - Example:
 *          int y = 2 + 3; // '2+3' is an rvalue
 *    - Rvalue reference: T&& ref = std::move(x); // can bind to temporaries
 *
 * Rvalue references (T&&) allow move semantics:
 *    - Transfer resources from temporary objects efficiently.
 *    - Enable move constructors and move assignment operators.
 *    - After moving, the source object is left in a valid but unspecified state.
 *
 * Summary:
 *    - Use T& to bind to existing objects (lvalues).
 *    - Use T&& to bind to temporaries (rvalues) for efficient moves.
 *
 * Example:
 *    MyResource a;
 *    MyResource b = std::move(a); // b takes ownership, a is left empty
 */

#include <iostream>
#include <utility>
#include <vector>

class MyResource {
    std::vector<int> data_;
public:
    // Default constructor
    MyResource() : data_(10, 0) {
        std::cout << "Default ctor\n";
    }

    // Destructor
    ~MyResource() {
        std::cout << "Destructor\n";
    }

    // Copy constructor
    MyResource(const MyResource& other) : data_(other.data_) {
        std::cout << "Copy ctor\n";
    }

    // Copy assignment
    MyResource& operator=(const MyResource& other) {
        std::cout << "Copy assignment\n";
        if (this != &other) {
            data_ = other.data_;
        }
        return *this;
    }

    // Move constructor
    MyResource(MyResource&& other) noexcept : data_(std::move(other.data_)) {
        std::cout << "Move ctor\n";
    }

    // Move assignment
    MyResource& operator=(MyResource&& other) noexcept {
        std::cout << "Move assignment\n";
        if (this != &other) {
            data_ = std::move(other.data_);
        }
        return *this;
    }

    void print() const {
        std::cout << "Size: " << data_.size() << "\n";
    }
};

int main() {
    std::cout << "--- Default construction ---\n";
    MyResource r1;
    r1.print();

    std::cout << "\n--- Copy construction ---\n";
    MyResource r2 = r1; // Copy ctor
    r2.print();

    std::cout << "\n--- Copy assignment ---\n";
    MyResource r3;
    r3 = r1; // Copy assignment
    r3.print();

    std::cout << "\n--- Move construction ---\n";
    MyResource r4 = std::move(r1); // Move ctor
    r4.print();

    std::cout << "\n--- Move assignment ---\n";
    MyResource r5;
    r5 = std::move(r2); // Move assignment
    r5.print();

    std::cout << "\n--- End of main ---\n";
}
