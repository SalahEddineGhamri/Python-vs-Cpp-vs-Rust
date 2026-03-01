// function_templates_demo.cpp
// Complete C++17 tutorial and demonstration of function templates.
//
// Covers:
// - Basic function template syntax and instantiation
// - Template type deduction
// - Explicit template arguments
// - Overloading function templates
// - Common pitfalls (e.g., no implicit conversions)
// - Constraints with concepts (C++20, but shown with C++17 alternatives)
// - Non-type template parameters
// - Good usage patterns and modern alternatives
//
// Compile: g++ -std=c++17 -Wall -Wextra -pedantic -O2 function_templates_demo.cpp -o function_templates_demo

#include <iostream>
#include <typeinfo>
#include <string>

// ------------------------------------------------------------------
// 1. Basic function template
// ------------------------------------------------------------------
/*
    A function template allows us to write a single function that works with many types.
    The compiler generates a concrete function for each unique set of template arguments
    used in the program (instantiation).
*/

template <typename T>
T maximum(T a, T b)
{
    std::cout << "maximum called with type: " << typeid(T).name() << "\n";
    return (a > b) ? a : b;
}

// ------------------------------------------------------------------
// 2. Template with multiple parameters
// ------------------------------------------------------------------
template <typename T, typename U>
auto flexible_max(T a, U b)
{
    std::cout << "flexible_max called with types: "
              << typeid(T).name() << " and " << typeid(U).name() << "\n";
    return (a > b) ? a : b;  // requires operator> between T and U
}

// ------------------------------------------------------------------
// 3. Explicit template arguments
// ------------------------------------------------------------------
// U instead of ReturnType works too
template <typename ReturnType, typename T>
ReturnType safe_cast(T value)
{
    std::cout << "safe_cast<" << typeid(ReturnType).name() << ">(" << value << ")\n";
    return static_cast<ReturnType>(value);
}

// ------------------------------------------------------------------
// 4. Non-type template parameter (C++17 allows auto)
// ------------------------------------------------------------------
template <auto N>  // works in C++17: N can be int, char, enum, pointer, etc.
constexpr auto array_size()
{
    return N;
}

// Older C++11/14 style (still common):
template <int N>
constexpr int old_array_size()
{
    return N;
}

// ------------------------------------------------------------------
// 5. Overloading function templates
// ------------------------------------------------------------------
template <typename T>
void print(const T& value)
{
    std::cout << "Generic print: " << value << "\n";
}

// Specialization-like overload for const char*
void print(const char* str)
{
    std::cout << "Specialized print for C-string: " << (str ? str : "nullptr") << "\n";
}

// ------------------------------------------------------------------
// 6. Common pitfall demo: no implicit conversions in deduction
// ------------------------------------------------------------------
void demo_pitfalls()
{
    std::cout << "\n=== Pitfalls ===\n";

    int i = 42;
    double d = 3.14;

    // maximum(i, d);  // ERROR: template argument deduction fails
    // Both arguments must be exactly the same type T

    // Solutions:
    std::cout << "Correct ways:\n";
    std::cout << maximum<double>(i, d) << "\n";  // explicit type
    std::cout << maximum(i, static_cast<int>(d)) << "\n";

    // flexible_max works because it has two different template parameters
    std::cout << flexible_max(i, d) << "\n";
}

// ------------------------------------------------------------------
// Main demo
// ------------------------------------------------------------------
int main()
{
    std::cout << "Function Templates Tutorial & Demo\n";
    std::cout << "==================================\n\n";

    // 1. Basic usage with automatic deduction
    std::cout << "=== Basic maximum ===\n";
    std::cout << maximum(10, 20) << "\n";           // T = int
    std::cout << maximum(3.5, 2.8) << "\n";         // T = double
    std::cout << maximum('A', 'Z') << "\n";         // T = char

    // 2. Different types
    std::cout << "\n=== flexible_max ===\n";
    std::cout << flexible_max(100, 3.14) << "\n";   // T=int, U=double

    // 3. Explicit template arguments
    std::cout << "\n=== Explicit instantiation ===\n";
    std::cout << safe_cast<double>(42) << "\n";
    std::cout << safe_cast<int>(3.99) << "\n";     // truncates!

    // 4. Non-type parameters
    std::cout << "\n=== Non-type template parameter ===\n";
    std::cout << "array_size<5>() = " << array_size<5>() << "\n";
    std::cout << "array_size<'X'>() = " << array_size<'X'>() << "\n";

    // 5. Overloading
    std::cout << "\n=== Template overloading ===\n";
    print(42);
    print(3.14);
    print("Hello template");
    print(static_cast<const char*>(nullptr));

    // 6. Pitfalls
    demo_pitfalls();

    std::cout << "\n=== Summary ===\n";
    std::cout << "- Function templates provide generic programming without runtime cost.\n";
    std::cout << "- Deduction works only when all template parameters can be determined.\n";
    std::cout << "- No implicit conversions during deduction → be careful with mixed types.\n";
    std::cout << "- Use multiple template parameters or explicit arguments for mixed types.\n";
    std::cout << "- Overload templates carefully — the char* overload wins over generic.\n";
    std::cout << "- C++20 concepts make constraints clearer (e.g., requires std::totally_ordered<T>).\n";
    std::cout << "\nInteresting fact:\n";
    std::cout << "The compiler generates a separate function for each unique type combination.\n";
    std::cout << "This leads to excellent performance but can increase compile time and binary size.\n";
    std::cout << "Modern trick: Prefer constexpr/if constexpr and auto return types for cleaner templates.\n";

    return 0;
}
