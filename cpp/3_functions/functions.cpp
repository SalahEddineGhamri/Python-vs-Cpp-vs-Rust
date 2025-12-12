#include <iostream>
#include <vector>
#include <memory>

using std::cout;

// ---------------------------------------------------------------
// 1) BASIC FUNCTION RETURNING A VALUE
// ---------------------------------------------------------------
int square(int x) {          // cheap type → pass-by-value OK
    return x * x;            // return value: NRVO + move elision
}

// ---------------------------------------------------------------
// 2) PASSING STRATEGIES
// ---------------------------------------------------------------

// --- Pass-by-VALUE ---------------------------------------------
// Copy parameter → modifications affect only the local copy.
// Use for cheap types or when you WANT a copy.
void increment_value(int v) {
    v += 10;                 // no effect on caller
    cout << "[value ] local=" << v << "\n";
}

// --- Pass-by-REFERENCE -----------------------------------------
// No copy. Direct alias.
// Use for large objects, or when mutation must reflect externally.
void increment_ref(int& r) {
    r += 10;                 // affects caller
    cout << "[ref   ] local=" << r << "\n";
}

// --- Pass-by-POINTER -------------------------------------------
// C-style alternative. Allows null → requires checks.
// Mutates pointed object.
// Use only when "nullable" semantics deliberately required.
// Best practice: prefer references unless null is meaningful.
void increment_ptr(int* p) {
    if (!p) {
        cout << "[ptr   ] null received — ignoring\n";
        return;
    }
    *p += 10;
    cout << "[ptr   ] local=" << *p << "\n";
}

// --- Pass-by-const-reference -----------------------------------
// Best for read-only large objects → avoids copy.
int sum_const_ref(const std::vector<int>& v) {
    int acc = 0;
    for (int x : v) acc += x;
    return acc;
}

// ---------------------------------------------------------------
// 3) MAIN — Demonstrates all cases, prints differences.
// ---------------------------------------------------------------
int main() {

    cout << "=== Basic returning value ===\n";
    cout << "square(5) = " << square(5) << "\n\n";

    int x = 1;

    cout << "=== Pass-by-value / reference / pointer ===\n";

    increment_value(x);      // x unchanged
    cout << "after increment_value x=" << x << "\n";

    increment_ref(x);        // x modified
    cout << "after increment_ref   x=" << x << "\n";

    increment_ptr(&x);       // x modified
    cout << "after increment_ptr   x=" << x << "\n";

    increment_ptr(nullptr);  // demonstrates pointer pitfall

    cout << "\n=== const-reference best practice ===\n";
    std::vector<int> v {1,2,3,4};
    cout << "sum = " << sum_const_ref(v) << "\n";

    return 0;
}
