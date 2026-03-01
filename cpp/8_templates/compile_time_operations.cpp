// compile_time_array_ops.cpp
// A complete tutorial and demonstration of compile-time array operations in C++17
// using std::array and template metaprogramming.
//
// This single file is self-contained, uses only standard library headers,
// and compiles with: g++ -std=c++17 -Wall -Wextra -O2 compile_time_array_ops.cpp -o demo
//
// Key concepts covered:
// - std::array as a compile-time-sized container
// - Basic operations via constexpr functions
// - Template-based operations (sum, min, max, reverse, sort)
// - Compile-time evaluation guarantees
// - Common pitfalls and constraints
// - Interesting tricks (fold expressions, index sequences)

#include <array>
#include <utility>
#include <iostream>
#include <cstddef>
#include <algorithm>  // for std::sort in one example

/*
  std::array<T, N> is a fixed-size array whose size is part of its type.
  Because the size is known at compile time, many operations can be performed
  entirely at compile time using constexpr functions and templates.

  All examples below are evaluated at compile time when possible.
*/

template<typename T, std::size_t N>
constexpr auto sum(const std::array<T, N>& arr)
{
    T result = T{};
    for (std::size_t i = 0; i < N; ++i)
        result += arr[i];
    return result;
}

// C++17 makes this even cleaner with fold expressions
template<typename T, std::size_t N, std::size_t... I>
constexpr T sum_fold_impl(const std::array<T, N>& arr, std::index_sequence<I...>) {
    return (arr[I] + ...);  // fold over the elements
}

template<typename T, std::size_t N>
constexpr T sum_fold(const std::array<T, N>& arr) {
    return sum_fold_impl(arr, std::make_index_sequence<N>{});
}
// ----------------------------------------------------------

template<typename T, std::size_t N>
constexpr T min_element(const std::array<T, N>& arr)
{
    T m = arr[0];
    for (std::size_t i = 1; i < N; ++i)
        if (arr[i] < m) m = arr[i];
    return m;
}

template<typename T, std::size_t N>
constexpr T max_element(const std::array<T, N>& arr)
{
    T m = arr[0];
    for (std::size_t i = 1; i < N; ++i)
        if (arr[i] > m) m = arr[i];
    return m;
}

// Compile-time reverse: returns a new std::array
template<typename T, std::size_t N, std::size_t... I>
constexpr std::array<T, N> reverse_helper(const std::array<T, N>& arr, std::index_sequence<I...>)
{
    return { arr[N - 1 - I]... };
}

template<typename T, std::size_t N>
constexpr std::array<T, N> reverse(const std::array<T, N>& arr)
{
    return reverse_helper(arr, std::make_index_sequence<N>{});
}

// Compile-time selection sort (simple but fully constexpr)
// Returns a new sorted array

template<typename T>
constexpr void cswap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

template<typename T, std::size_t N, std::size_t... I>
constexpr std::array<T, N> sort_helper(const std::array<T, N>& arr, std::index_sequence<I...>)
{
    auto temp = arr;
    for (std::size_t i = 0; i < N; ++i)
    {
        std::size_t min_idx = i;
        for (std::size_t j = i + 1; j < N; ++j)
            if (temp[j] < temp[min_idx])
                min_idx = j;

        if (min_idx != i)
            cswap(temp[i], temp[min_idx]);
    }
    return temp;
}

template<typename T, std::size_t N>
constexpr std::array<T, N> sort_array(const std::array<T, N>& arr)
{
    return sort_helper<T, N>(arr, std::make_index_sequence<N>{});
}

// Helper to print an array at runtime (for demonstration only)
template<typename T, std::size_t N>
void print(const std::array<T, N>& arr, const char* name)
{
    std::cout << name << ": { ";
    for (std::size_t i = 0; i < N; ++i)
    {
        std::cout << arr[i];
        if (i + 1 < N) std::cout << ", ";
    }
    std::cout << " }\n";
}

int main()
{
    // All these arrays are constexpr → operations happen at compile time
    constexpr std::array<int, 6> data{ 5, 2, 9, 1, 5, 6 };

    // Basic compile-time operations
    constexpr int total          = sum(data);
    constexpr int total_fold     = sum_fold(data);
    constexpr int smallest       = min_element(data);
    constexpr int largest        = max_element(data);

    // More advanced: reverse and sort return new arrays
    constexpr auto reversed      = reverse(data);
    constexpr auto sorted        = sort_array(data);

    // Demonstrate results (runtime printing of compile-time results)
    std::cout << "Original array";
    print(data, "");

    std::cout << "\nCompile-time computed values:\n";
    std::cout << "Sum (loop)          : " << total << '\n';
    std::cout << "Sum (fold expr)     : " << total_fold << '\n';
    std::cout << "Minimum element     : " << smallest << '\n';
    std::cout << "Maximum element     : " << largest << '\n';

    std::cout << "\nCompile-time transformed arrays:\n";
    print(reversed, "Reversed");
    print(sorted,   "Sorted  ");

    /*
      Pitfalls and constraints:

      1. All functions must be marked constexpr and use only constexpr-friendly operations.
         std::swap is constexpr since C++14, so it's safe here.

      2. The array size N must be known at compile time.
         You cannot use std::array with a runtime-determined size for these operations.

      3. Complex algorithms (e.g., quicksort) become very long compile-time recursions
         and can quickly hit template instantiation depth limits (often ~900-1024).
         Selection sort used above is simple and safe for small N.

      4. Returning a new std::array from a constexpr function creates a compile-time object.
         No runtime allocation occurs.

      5. Interesting trick: std::index_sequence + parameter pack expansion lets us
         manipulate indices in very powerful ways (reverse, mapping, etc.).

      6. Fold expressions (C++17) dramatically simplify reductions like sum/product.
    */

    return 0;
}
