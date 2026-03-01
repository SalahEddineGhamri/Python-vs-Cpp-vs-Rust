/*
============================================================
S F I N A E   T U T O R I A L   (C++17 ONLY)
============================================================

Compile:
  g++ -std=c++17 sfinae_tutorial.cpp

------------------------------------------------------------
*/

#include <iostream>
#include <type_traits>
#include <utility>   // std::declval

// ============================================================
// 1. WHAT IS SFINAE?
// ============================================================
//
// SFINAE = Substitution Failure Is Not An Error
//
// During template overload resolution:
// - The compiler substitutes template arguments
// - If substitution makes a declaration ill-formed
// - That declaration is REMOVED from the overload set
// - Compilation continues without error
//
// This only applies to the *immediate context* of substitution.
// Anything instantiated as a side-effect is a HARD error.
//


// ============================================================
// 2. BASIC SFINAE WITH std::enable_if
// ============================================================
//
// Two overloads of the same function.
// Only one survives substitution.
//

template <typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
print(T value)
{
    std::cout << "[integral] " << value << '\n';
}

template <typename T>
typename std::enable_if<!std::is_integral<T>::value, void>::type
print(T value)
{
    std::cout << "[non-integral] " << value << '\n';
}

// If enable_if<false, void>::type is used → invalid type
// → substitution failure → overload discarded


// ============================================================
// 3. TYPE SFINAE — DETECTING A NESTED TYPE
// ============================================================
//
// Goal: detect whether T::value_type exists
//

template <typename T, typename = void>
struct has_value_type : std::false_type {};

// Specialization only participates if T::value_type is valid
template <typename T>
struct has_value_type<T, std::void_t<typename T::value_type>>
    : std::true_type {};

struct A {};
struct B { using value_type = int; };


// ============================================================
// 4. EXPRESSION SFINAE — DETECTING OPERATORS
// ============================================================
//
// Detect whether T + T is a valid expression
//

template <typename T, typename = void>
struct has_plus : std::false_type {};

template <typename T>
struct has_plus<T,
    std::void_t<decltype(std::declval<T>() + std::declval<T>())>>
    : std::true_type {};

struct NoPlus {};

struct WithPlus {
    int x;
};

WithPlus operator+(WithPlus a, WithPlus b)
{
    return {a.x + b.x};
}


// ============================================================
// 5. FUNCTION OVERLOAD SELECTION VIA EXPRESSION SFINAE
// ============================================================
//
// If T + T is ill-formed, this overload is removed.
//

template <typename T>
auto add(T a, T b) -> decltype(a + b)
{
    return a + b;
}

// Fallback overload
int add(int a, int b)
{
    return a + b;
}


// ============================================================
// 6. IMMEDIATE CONTEXT RULE (IMPORTANT)
// ============================================================
//
// Only these participate in SFINAE:
// - function return type
// - parameter types
// - template parameter types
//
// Anything instantiated inside the function body
// causes a HARD compile-time error.
//

template <typename T>
struct Explode {
    static_assert(sizeof(T) == 0, "Hard error (not SFINAE)");
};

// Uncommenting below WILL BREAK compilation (correctly)
//
// template <typename T>
// typename std::enable_if<true, void>::type bad()
// {
//     Explode<T> x; // NOT SFINAE — hard error
// }


// ============================================================
// 7. PARTIAL SPECIALIZATION SFINAE
// ============================================================
//
// Used heavily by the standard library
//

template <typename T, typename = void>
struct add_lvalue_reference
{
    using type = T;
};

template <typename T>
struct add_lvalue_reference<T, std::void_t<T&>>
{
    using type = T&;
};


// ============================================================
// 8. CLEANER C++17 ALTERNATIVE: if constexpr
// ============================================================
//
// Often preferable to SFINAE for branching logic
//

template <typename T>
void print_if_constexpr(T value)
{
    if constexpr (std::is_integral<T>::value)
        std::cout << "[if constexpr integral] " << value << '\n';
    else
        std::cout << "[if constexpr non-integral] " << value << '\n';
}


// ============================================================
// 9. MAIN — DEMONSTRATION
// ============================================================

int main()
{
    std::cout << "=== enable_if overloads ===\n";
    print(42);
    print(3.14);

    std::cout << "\n=== type SFINAE ===\n";
    std::cout << "A has value_type: " << has_value_type<A>::value << '\n';
    std::cout << "B has value_type: " << has_value_type<B>::value << '\n';

    std::cout << "\n=== expression SFINAE ===\n";
    std::cout << "NoPlus has + : " << has_plus<NoPlus>::value << '\n';
    std::cout << "WithPlus has + : " << has_plus<WithPlus>::value << '\n';

    std::cout << "\n=== overload via expression SFINAE ===\n";
    std::cout << add(1, 2) << '\n';

    std::cout << "\n=== if constexpr (C++17) ===\n";
    print_if_constexpr(7);
    print_if_constexpr(2.5);
}
