#include <iostream>
#include <type_traits>   // std::enable_if, std::is_integral, std::true_type/false_type

// ===================================================================
// SFINAE (Substitution Failure Is Not An Error) example using std::enable_if
// ===================================================================
//
// Goal: Provide different implementations of a function called print()
// depending on whether the type T is an integral type (int, char, bool, etc.)
// or not (float, std::string, custom classes, etc.).
//
// We use std::enable_if to "remove" (disable) overloads during template
// instantiation if a condition is false. If substitution fails, it's NOT an error
// — the compiler simply discards that overload and tries the next one.
//
// This is a classic C++11/C++17 technique before C++20 concepts became available.

namespace sfinae_example {

// -------------------------------------------------------------------
// 1. Version for INTEGRAL types (int, long, bool, char, etc.)
// -------------------------------------------------------------------
// std::enable_if<condition, T>::type is defined ONLY if condition == true.
// If condition == false, ::type does not exist → substitution failure → overload discarded.

template <typename T>
typename std::enable_if<std::is_integral<T>::value>::type   // <-- SFINAE guard
print(const T& value)
{
    // This overload is selected only for integral types
    std::cout << "Integral value: " << value << " (size: " << sizeof(T) << " bytes)\n";
}

// -------------------------------------------------------------------
// 2. Version for NON-INTEGRAL (floating-point and everything else)
// -------------------------------------------------------------------
template <typename T>
typename std::enable_if<!std::is_integral<T>::value>::type  // <-- negated condition
print(const T& value)
{
    // This overload is selected for float, double, std::string, custom classes, etc.
    std::cout << "Non-integral value: " << value << "\n";
}

// -------------------------------------------------------------------
// Why does this work?
// -------------------------------------------------------------------
// During overload resolution the compiler tries to instantiate both templates:
//
//   • For T = int:
//        - First overload: std::is_integral<int>::value == true  → ::type exists → valid
//        - Second overload: !true == false → ::type does not exist → SFINAE removes it
//        → Only the first overload remains → perfect match.
//
//   • For T = double:
//        - First overload: std::is_integral<double>::value == false → ::type missing → discarded
//        - Second overload: !false == true → ::type exists → valid
//        → Only the second overload remains.
//
// No ambiguity, no compilation error.

// -------------------------------------------------------------------
// Modern C++17 improvement: default the return type and use void
// -------------------------------------------------------------------
// The old style returns "typename std::enable_if<...>::type" which is usually void.
// In C++14+ we can make it cleaner by specifying void as the second template argument:

template <typename T>
std::enable_if_t<std::is_integral<T>::value, void>   // std::enable_if_t is a typedef for ::type
print_integral_only(const T& value)
{
    std::cout << "[integral-only] " << value << '\n';
}

template <typename T>
std::enable_if_t<!std::is_integral<T>::value, void>
print_non_integral_only(const T& value)
{
    std::cout << "[non-integral-only] " << value << '\n';
}

} // namespace sfinae_example

// ===================================================================
// Demo in main()
// ===================================================================
int main()
{
    using namespace sfinae_example;

    std::cout << "=== Basic SFINAE overloads ===\n";
    print(42);          // → Integral version
    print(3.14);        // → Non-integral version
    print(true);        // → Integral (bool is integral)
    print("hello");     // → Non-integral (const char* decays to pointer)

    std::cout << "\n=== C++17 cleaner syntax ===\n";
    print_integral_only(100LL);     // long long is integral
    // print_integral_only(2.718);  // Compile error if uncommented — correctly disabled!
    print_non_integral_only(2.718); // OK

    return 0;
}
