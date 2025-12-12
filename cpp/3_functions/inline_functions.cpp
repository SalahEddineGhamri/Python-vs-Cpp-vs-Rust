#include <iostream>
#include <cmath>

// ================================================================
// 1) inline DOES NOT guarantee inlining.
//    It gives:
//      • ODR relaxation (multiple definitions across TUs allowed)
//      • A hint for inlining (compiler decides)
// ================================================================

// Classical inline candidate: tiny + header-friendly
inline int add(int a, int b) {
    return a + b;
}

// inline for functions with internal state
inline double cached_sin(double x) {
    // Before ODR violation for each TU we get last_x and last_val
    // internal static inside inline → one instance per program (since C++17)
    static double last_x   = 0.0;
    static double last_val = 0.0;

    if (x == last_x) return last_val;
    last_x = x;
    return last_val = std::sin(x);
}

// ---------------------------------------------------------------
// BAD EXAMPLE — inline used incorrectly
// (Large functions should not be forced inline; compiler will ignore)
// ---------------------------------------------------------------
inline void huge_inline_mistake() {
    // Pretend expensive loop
    for (volatile int i = 0; i < 10'000'000; ++i) {
    }
    // Compiler will likely NOT inline this.
}

// ---------------------------------------------------------------
// GOOD EXAMPLE — inline for small utility logic
// ---------------------------------------------------------------
inline bool between(int x, int lo, int hi) noexcept {
    return (x >= lo) && (x <= hi);
}

// ---------------------------------------------------------------
int main() {
    std::cout << "=== inline demo ===\n";

    std::cout << "add(3,4) = " << add(3,4) << "\n";

    std::cout << "\ncached_sin(1.57) first call  = "
              << cached_sin(1.57) << "\n";
    std::cout << "cached_sin(1.57) second call = "
              << cached_sin(1.57) << "\n";   // cached value

    std::cout << "\nbetween(5,1,10) = " << between(5,1,10) << "\n";

    std::cout << "\nCalling huge_inline_mistake()...\n";
    huge_inline_mistake();   // Not actually inlined (almost always)

    return 0;
}
