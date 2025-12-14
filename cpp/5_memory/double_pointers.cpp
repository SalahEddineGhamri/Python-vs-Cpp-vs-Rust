// more of a legacy
#include <iostream>
#include <memory>

using std::cout;

/*
  T** = pointer to pointer.
  Typical uses:
    1) Modify a caller's pointer (out-parameter).
    2) C-style APIs / legacy interop.
    3) Multi-level dynamic structures (rare in modern C++).
*/

void allocate_raw(int** out) {
    // Pitfall-prone C-style API
    if (!out) return;
    *out = new int(42);     // caller must delete
}

void reset_raw(int** p) {
    if (p && *p) {
        delete *p;
        *p = nullptr;
    }
}

// Modern alternative: reference to pointer
void allocate_ref(int*& out) {
    out = new int(99);
}

int main() {
    cout << "=== Basic indirection ===\n";
    int x = 10;
    int* p = &x;
    int** pp = &p;

    cout << "x      = " << x << "\n";
    cout << "*p     = " << *p << "\n";
    cout << "**pp   = " << **pp << "\n";

    **pp = 20;              // writes to x
    cout << "x after = " << x << "\n\n";

    cout << "=== Modifying caller pointer (T**) ===\n";
    int* raw = nullptr;
    allocate_raw(&raw);     // pass pointer-to-pointer
    cout << "*raw    = " << *raw << "\n";
    reset_raw(&raw);        // deletes and nulls
    cout << "raw ptr = " << raw << "\n\n";

    cout << "=== Reference-to-pointer (preferred) ===\n";
    int* r = nullptr;
    allocate_ref(r);        // clearer, safer
    cout << "*r      = " << *r << "\n";
    delete r;               // still manual, but no T**
    r = nullptr;

    cout << "\n=== Best practice: smart pointers ===\n";
    std::unique_ptr<int> up;
    up = std::make_unique<int>(123); // no T**, no delete
    cout << "*up     = " << *up << "\n";

    // Pitfall: multiple owners with raw double pointers
    // int** shared;         // ❌ ownership unclear, leaks likely
}
