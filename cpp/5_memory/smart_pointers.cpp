#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>

struct Node {
    int value;
    Node(int v) : value(v) { std::cout << "Node " << value << " created\n"; }
    ~Node() { std::cout << "Node " << value << " destroyed\n"; }
};

int main() {
    std::cout << "\n=== std::unique_ptr ===\n";

    // Creation (preferred: make_unique)
    std::unique_ptr<Node> up1 = std::make_unique<Node>(1);

    // Ownership transfer (move semantics)
    std::unique_ptr<Node> up2 = std::move(up1);
    if (!up1) std::cout << "up1 is now null\n";

    // Resetting pointer (destroys old object)
    up2.reset(new Node(2));

    // Release ownership (dangerous if not deleted)
    Node* raw = up2.release();
    delete raw;

    // Pitfalls:
    // 1. Copying unique_ptr -> compile-time error (enforces uniqueness)
    // 2. Releasing without deleting -> memory leak
    // 3. Using after release -> undefined behavior

    // Clever C++17 tip: unique_ptr supports custom deleters
    auto up_custom = std::unique_ptr<Node, void(*)(Node*)>(
        new Node(42), [](Node* p){
            std::cout << "Custom deleting Node " << p->value << "\n";
            delete p;
        }
    );

    std::cout << "\n=== std::shared_ptr ===\n";

    auto sp1 = std::make_shared<Node>(3); // recommended over new
    std::shared_ptr<Node> sp2 = sp1;

    std::cout << "Use count: " << sp1.use_count() << "\n";

    sp2.reset(); // decreases use_count
    std::cout << "Use count after reset: " << sp1.use_count() << "\n";

    // Pitfalls:
    // 1. Circular references (A -> B -> A) will leak memory
    // 2. Overhead: reference counting adds atomic ops
    // 3. Not thread-safe if two threads modify same shared_ptr without sync

    // Clever tips:
    // - Use make_shared to avoid double allocation (object + ref count)
    // - Prefer weak_ptr to break cycles
    // - Can store shared_ptr in containers safely

    std::vector<std::shared_ptr<Node>> nodes;
    nodes.emplace_back(std::make_shared<Node>(4));
    nodes.emplace_back(std::make_shared<Node>(5));

    std::cout << "\n=== std::weak_ptr ===\n";

    std::weak_ptr<Node> wp = sp1; // does not increase use_count
    std::cout << "Weak lock success? " << (wp.lock() ? "yes" : "no") << "\n";

    sp1.reset(); // destroys Node(3)
    std::cout << "After sp1 reset, weak lock success? " << (wp.lock() ? "yes" : "no") << "\n";

    // Pitfalls:
    // 1. Using expired weak_ptr -> nullptr from lock, safe but must check
    // 2. Does not own object, cannot guarantee lifetime
    // 3. Frequent lock() calls are cheap, but don’t abuse in hot loops

    // Clever tip:
    // Can use weak_ptr in caches or observer patterns to avoid dangling references
    std::unordered_map<int, std::weak_ptr<Node>> cache;
    cache[1] = nodes[0]; // weak reference, does not extend lifetime
    cache[2] = nodes[1];

    // Nice-to-have checks
    for (auto& [k, wptr] : cache) {
        if (auto sp = wptr.lock()) {
            std::cout << "Cache node " << k << " alive: " << sp->value << "\n";
        } else {
            std::cout << "Cache node " << k << " expired\n";
        }
    }

    // Summary of best practices and pros
    /*
    Unique_ptr:
        - Lightweight, exclusive ownership
        - Avoid raw delete; use make_unique
        - Use custom deleters for special cleanup

    Shared_ptr:
        - Reference counting, automatic cleanup
        - Avoid cycles: use weak_ptr to break cycles
        - Thread-safe reference count
        - Prefer make_shared to reduce allocations

    Weak_ptr:
        - Observes without owning
        - Perfect for caches, observer patterns, cyclic structures
        - Always lock() and check for nullptr
    */

    return 0;
}
