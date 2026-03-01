/*
================================================================================
COMPLETE C++17 TUTORIAL: std::map vs std::unordered_map
================================================================================

compile: !g++ -std=c++17 -Wall -Wextra -O2 % -o %<

This file is:
- Executable
- Self-contained
- Covers common + advanced use cases
- Demonstrates pitfalls
- Shows best practices
- Teaches idiomatic C++17

Core Differences
----------------
std::map:
    - Ordered (red-black tree)
    - O(log n)
    - Stable iteration order
    - Supports lower_bound / upper_bound

std::unordered_map:
    - Hash table
    - Average O(1)
    - No ordering
    - Requires hash + equality
    - May rehash → iterator invalidation

When to choose:
---------------
Need ordering or range queries? → std::map
Need fastest lookup? → std::unordered_map

================================================================================
*/

#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <cassert>
#include <chrono>

// Utility printer
template<typename Map>
void print_map(const Map& m, const std::string& name)
{
    std::cout << "\n" << name << ":\n";
    for (const auto& [k, v] : m)
        std::cout << "  " << k << " => " << v << "\n";
}

/*
================================================================================
1. BASIC USAGE
================================================================================
*/
void basic_usage()
{
    std::map<std::string, int> ordered;
    std::unordered_map<std::string, int> unordered;

    ordered["apple"] = 3;
    ordered["banana"] = 5;

    unordered["apple"] = 3;
    unordered["banana"] = 5;

    print_map(ordered, "std::map (ordered)");
    print_map(unordered, "std::unordered_map (no order guarantee)");
}

/*
================================================================================
2. INSERTION METHODS
================================================================================
*/
void insertion_methods()
{
    std::map<int, std::string> m;

    // insert (safe, no overwrite)
    m.insert({1, "one"});
    m.insert(std::make_pair(2, "two"));

    // emplace (construct in-place)
    m.emplace(3, "three");

    // try_emplace (C++17) – avoids unnecessary construction
    m.try_emplace(3, "THREEE"); // will NOT overwrite

    // insert_or_assign (C++17)
    m.insert_or_assign(4, "Four"); // overwrites
    m.insert_or_assign(4, "FOUR"); // overwrites

    print_map(m, "Insertion methods");

    // Pitfall: operator[] inserts default value if key missing
    auto value = m[100]; // creates key=100 with default string ""
    (void)value;
}

/*
================================================================================
3. FINDING ELEMENTS (AVOID operator[] FOR READ-ONLY)
================================================================================
*/
void lookup()
{
    std::map<int, std::string> m = {
        {1, "one"},
        {2, "two"}
    };

    if (auto it = m.find(2); it != m.end())
        std::cout << "\nFound: " << it->second << "\n";

    // Safer than operator[]
    std::optional<std::string> result;
    if (auto it = m.find(3); it != m.end())
        result = it->second;

    assert(!result.has_value());
}

/*
================================================================================
4. ORDER-SPECIFIC FEATURES (ONLY std::map)
================================================================================
   - lower_bound: first element which >= x
   - range: [lower_bound, upper_bound]
   - Upper_bound: first element strickly > x
*/
void ordered_features()
{
    std::map<int, std::string> m = {
        {1, "one"},
        {3, "three"},
        {5, "five"}
    };

    auto it = m.lower_bound(3);
    if (it != m.end())
        std::cout << "\nlower_bound(3): " << it->first << "\n";

    auto range = m.equal_range(3);
    if (range.first != m.end())
        std::cout << "equal_range(3): " << range.first->first << "\n";
}

/*
================================================================================
5. CUSTOM COMPARATOR (std::map)
================================================================================
*/
void custom_comparator()
{
    std::map<int, std::string, std::greater<>> m;
    m[1] = "one";
    m[2] = "two";

    print_map(m, "std::map with descending order");
}

/*
================================================================================
6. CUSTOM HASH (std::unordered_map)
================================================================================
*/
struct Point
{
    int x, y;

    bool operator==(const Point& other) const noexcept
    {
        return x == other.x && y == other.y;
    }
};

struct PointHash
{
    std::size_t operator()(const Point& p) const noexcept
    {
        return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 1);
    }
};

void custom_hash()
{
    std::unordered_map<Point, std::string, PointHash> m;
    m[{1,2}] = "A";

    std::cout << "\nCustom hash lookup: "
              << m[{1,2}] << "\n";
}

/*
================================================================================
7. PERFORMANCE PITFALLS
================================================================================
*/
void performance_notes()
{
    std::unordered_map<int,int> um;
    um.reserve(1000); // best practice to avoid rehashing

    for (int i = 0; i < 1000; ++i)
        um.emplace(i, i);

    // Iterator invalidation demo
    auto it = um.begin();
    (void)it;
    um.rehash(5000); // invalidates iterators
    // it is now invalid → undefined behavior if used
}

/*
================================================================================
8. ERASE PATTERNS
================================================================================
*/
void erase_patterns()
{
    std::map<int,int> m = {{1,1},{2,2},{3,3}};

    // erase by key
    m.erase(2);

    // erase while iterating (safe idiom)
    for (auto it = m.begin(); it != m.end(); )
    {
        if (it->first % 2 == 1)
            it = m.erase(it);
        else
            ++it;
    }

    print_map(m, "After erase");
}

/*
================================================================================
9. COUNTING / FREQUENCY MAP (COMMON PATTERN)
================================================================================
*/
void frequency_count()
{
    std::vector<int> data = {1,2,2,3,3,3};
    std::unordered_map<int,int> freq;

    for (int x : data)
        ++freq[x]; // operator[] is idiomatic here

    print_map(freq, "Frequency map");
}

/*
================================================================================
10. ADVANCED TIPS
================================================================================
*/

void advanced_tips()
{
    // Structured binding (C++17)
    std::map<int,std::string> m = {{1,"one"}};

    for (auto& [key,value] : m)
        value += "!";

    print_map(m, "Structured binding");

    // Heterogeneous lookup (transparent comparator)
    std::map<std::string,int,std::less<>> transparent;
    transparent["hello"] = 1;

    // No temporary std::string constructed:
    auto it = transparent.find("hello");
    assert(it != transparent.end());
}

/*
================================================================================
11. BENCHMARK DEMO (ROUGH)
================================================================================
*/
void benchmark()
{
    constexpr int N = 100000;

    {
        std::map<int,int> m;
        auto start = std::chrono::high_resolution_clock::now();
        for(int i=0;i<N;++i) m.emplace(i,i);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "\nmap insert time: "
                  << std::chrono::duration<double>(end-start).count()
                  << "s\n";
    }

    {
        std::unordered_map<int,int> m;
        m.reserve(N);
        auto start = std::chrono::high_resolution_clock::now();
        for(int i=0;i<N;++i) m.emplace(i,i);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "unordered_map insert time: "
                  << std::chrono::duration<double>(end-start).count()
                  << "s\n";
    }
}

/*
================================================================================
12. BEST PRACTICES SUMMARY
================================================================================

1. Prefer emplace / try_emplace.\
2. Avoid operator[] for read-only lookup.\
3. Reserve for unordered_map.\
4. Use transparent comparators.\
5. Understand iterator invalidation.\
6. Prefer structured bindings.\
7. Consider:\
      - std::pmr::map (polymorphic allocators)\
      - boost::flat_map (cache friendly)\
      - robin_hood hashing libraries\
================================================================================
*/

int main()
{
    basic_usage();
    insertion_methods();
    lookup();
    ordered_features();
    custom_comparator();
    //custom_hash();
    //performance_notes();
    //erase_patterns();
    //frequency_count();
    //advanced_tips();
    //benchmark();

    std::cout << "\nAll demonstrations completed.\n";
}
