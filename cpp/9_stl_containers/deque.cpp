/*
std::deque (double-ended queue) summary:

- Storage: Non-contiguous chunks of memory; allows growth at both ends efficiently.
- Complexity:
    * Access by index: O(1) but slightly slower than std::vector due to non-contiguous storage.
    * Insertion/removal at ends: O(1)
    * Insertion/removal in middle: O(n)
- Pros:
    * Fast push/pop at both front and back.
    * Random access supported.
    * Safer than vector for frequent front insertions.
- Cons:
    * Slightly worse cache locality than vector.
    * Iterators may be invalidated on insertion at either end.
    * Middle insertions/deletions slower than vector for contiguous data.
- Best Use Cases:
    * Queue or deque structures where both ends are used.
    * When frequent front insertions/removals are required.
    * Mixed access patterns where occasional random access is needed.
*/
#include <deque>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <string>

int main() {
    using namespace std;

    // --- 1. Basic creation and initialization ---
    deque<int> d1;                          // empty deque
    deque<int> d2{1, 2, 3};                 // initializer list
    deque<int> d3(5, 42);                   // 5 elements, all 42

    cout << "Initial d2: ";
    for (auto n : d2) cout << n << ' ';
    cout << "\n";

    // --- 2. Insertion / Deletion ---
    d2.push_back(4);                        // append at end
    d2.push_front(0);                       // prepend at front
    d2.pop_back();                           // remove last
    d2.pop_front();                          // remove first

    cout << "After push/pop: ";
    for (auto n : d2) cout << n << ' ';
    cout << "\n";

    // --- 3. Random access ---
    cout << "First element: " << d2.front() << "\n";
    cout << "Last element: " << d2.back() << "\n";
    cout << "Element at index 1: " << d2[1] << "\n";  // no bounds check
    // cout << d2.at(10); // would throw out_of_range

    // --- 4. Iteration ---
    cout << "Forward iteration: ";
    for (auto it = d2.begin(); it != d2.end(); ++it)
        cout << *it << ' ';
    cout << "\n";

    cout << "Reverse iteration: ";
    for (auto rit = d2.rbegin(); rit != d2.rend(); ++rit)
        cout << *rit << ' ';
    cout << "\n";

    // --- 5. Algorithms ---
    deque<int> d4{3, 1, 4, 1, 5};
    sort(d4.begin(), d4.end());             // works like vector
    cout << "Sorted d4: ";
    for (auto n : d4) cout << n << ' ';
    cout << "\n";

    int sum = accumulate(d4.begin(), d4.end(), 0);
    cout << "Sum of d4: " << sum << "\n";

    // --- 6. Pitfalls ---
    cout << "\nPitfall: Deque iterator invalidation\n";
    auto it = d4.begin();
    d4.push_front(0);                        // may invalidate iterators!
    // cout << *it; // unsafe, iterator may be invalid

    // --- 7. Best practices ---
    // Use emplace_front/emplace_back for in-place construction
    deque<string> ds;
    ds.emplace_back("hello");
    ds.emplace_front(5, '*');               // "*****"
    cout << "Strings in deque: ";
    for (auto& s : ds) cout << s << ' ';
    cout << "\n";

    // --- 8. Other notes ---
    // deque allows fast insert/remove at both ends, unlike vector
    // Random access is slightly slower than vector (non-contiguous chunks)
    // Useful for queue-like structures with occasional random access
}
