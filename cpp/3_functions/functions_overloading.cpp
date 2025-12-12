#include <iostream>
#include <string>

using std::cout;
using std::string;

// ================================================================
// 1) SIMPLE OVERLOADS
// ================================================================

void print(int x) {
    cout << "int: " << x << "\n";
}

void print(double x) {
    cout << "double: " << x << "\n";
}

void print(const string& s) {
    cout << "string: " << s << "\n";
}

// ================================================================
// 2) CONST & REF OVERLOADS
// ================================================================

void modify(int& x) {
    x += 10;
    cout << "[ref] x=" << x << "\n";
}

void modify(const int& x) {
    cout << "[const ref] x=" << x << "\n";
}

// ================================================================
// 3) RVALUE OVERLOAD
// ================================================================

void consume(string&& s) {
    cout << "[rvalue] consumed: " << s << "\n";
}

// ================================================================
// 4) DEFAULT ARG & AMBIGUITY PITFALL
// ================================================================

void f(int x, int y = 10) {
    cout << "f(int,int) x+y=" << x+y << "\n";
}

// Uncommenting this causes ambiguity error with default arg
// void f(int x) { cout << "f(int) x=" << x << "\n"; }

// ================================================================
// 5) MAIN — TEST CASES
// ================================================================

int main() {
    cout << "=== Basic overloading ===\n";
    print(42);          // exact int
    print(3.14);        // exact double
    print("hello");     // const char* → string via conversion

    cout << "\n=== const & ref ===\n";
    int a = 5;
    modify(a);          // int& match
    modify(100);        // const int& match (temporary)

    cout << "\n=== rvalue reference ===\n";
    string s = "temp";
    consume(std::move(s));      // rvalue reference
    // consume(s); // error: lvalue cannot bind to rvalue reference

    cout << "\n=== default argument ===\n";
    f(5);               // uses default y=10
    f(5,20);            // explicit y=20

    return 0;
}
