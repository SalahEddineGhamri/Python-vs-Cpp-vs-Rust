#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <type_traits>
#include <utility>
#include <tuple>
#include <functional>

using namespace std;

/* ============================================================
   1. CLASSIC RECURSIVE VARIADIC (for historical context)
   ============================================================*/
void print_all() { cout << "\n"; }
template<typename T, typename... Args>
void print_all(const T& first, const Args&... rest) {
    cout << first << " ";
    print_all(rest...);
}

/* ============================================================
   2. C++17 FOLD EXPRESSIONS — THE MODERN WAY
   ============================================================*/
template<typename... Args>
void print_fold(const Args&... args) {
    ((cout << args << " "), ...);  // left fold over comma operator
    cout << "\n";
}

// Bonus: fold over operator<< directly (cleaner!)
template<typename... Args>
void print_fold_clean(const Args&... args) {
    (cout << ... << args) << "\n";  // RIGHT fold over << (more natural)
    // Equivalent to: cout << a << b << c << ...
}

/* ============================================================
   3. PERFECT FORWARDING WRAPPER (universal + forwarding refs)
   ============================================================*/
template<typename F, typename... Args>
auto call(F&& fn, Args&&... args)
    -> decltype(std::forward<F>(fn)(std::forward<Args>(args)...))
{
    return std::forward<F>(fn)(std::forward<Args>(args)...);
}

/* ============================================================
   4. FOLD EXPRESSIONS IN ACTION: sum, product, all_of, etc.
   ============================================================*/
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);                    // left fold
    // return (0 + ... + args);             // force start value
}

template<typename... Args>
auto product(Args... args) {
    return (args * ... * 1);                // right fold with identity
}

template<typename... Bools>
bool all_true(Bools... bools) {
    return (... && bools);                  // fold over &&
}

/* ============================================================
   5. SFINAE → C++20 CONCEPTS PREVIEW (but still C++17 compatible)
   ============================================================*/
template<typename... Args>
std::enable_if_t<(std::is_arithmetic_v<Args> && ...), double>  // conjunction fold!
mean(Args... args)
{
    return (static_cast<double>(args) + ...) / sizeof...(args);
}

/* ============================================================
   6. INDEXED ACCESS WITH std::tuple + fold + integer sequence
   ============================================================*/
template<typename... Args>
struct TuplePrinter {
    static void print(const tuple<Args...>& t) {
        apply([&](const auto&... elems) {
            ((cout << elems << " "), ...);
            cout << "\n";
        }, t);
    }
};

// Bonus: print with indices
template<size_t... I, typename... Args>
void print_tuple_with_index(const tuple<Args...>& t, index_sequence<I...>) {
    ((cout << I << ": " << get<I>(t) << "  "), ...);
    cout << "\n";
}

template<typename... Args>
void print_tuple_indexed(const tuple<Args...>& t) {
    print_tuple_with_index(t, make_index_sequence<sizeof...(Args)>{});
}

/* ============================================================
   7. VARIADIC CLASS TEMPLATE (heterogeneous container)
   ============================================================*/
template<typename... Types>
struct VariantContainer {
    tuple<Types...> data;

    template<typename... Args>
    VariantContainer(Args&&... args) : data(std::forward<Args>(args)...) {}

    void print() const {
        apply([](const auto&... elems) {
            ((cout << elems << " "), ...);
            cout << "\n";
        }, data);
    }
};

/* ============================================================
   8. VARIADIC METHOD + INITIALIZER LIST TRICK (emplace_back all)
   ============================================================*/
struct Logger {
    vector<string> messages;

    template<typename... Args>
    void log(Args&&... args) {
        // Convert each arg to string and store
        (messages.emplace_back(to_string(std::forward<Args>(args))), ...);
    }

private:
    // Overloads for common types (including const char*)
    static string to_string(const string& s) { return s; }
    static string to_string(string&& s) { return std::move(s); }
    static string to_string(const char* s) { return s; }
    template<typename T>
    static string to_string(T&& v) {
        return std::to_string(std::forward<T>(v));
    }
};

/* ============================================================
   9. EXPANDING INTO INITIALIZER LIST (classic trick)
   ============================================================*/
template<typename... Args>
auto make_vector(Args&&... args) {
    vector<common_type_t<Args...>> vec;
    vec.reserve(sizeof...(args));
    (vec.emplace_back(std::forward<Args>(args)), ...);
    return vec;
}

/* ============================================================
   10. C++20 CONCEPTS (optional bonus — compile with -std=c++20)
   ============================================================*/
// template<typename... Args>
// concept ArithmeticPack = (std::is_arithmetic_v<Args> && ...);
//
// template<ArithmeticPack... Args>
// double mean_modern(Args... args) {
//     return (args + ...) / sizeof...(args);
// }

/* ============================================================
   MAIN: DEMO EVERYTHING
   ============================================================*/
int main() {
    cout << "=== 1. Recursive Print ===\n";
    print_all(1, "hello", 3.14, 'X');

    cout << "\n=== 2. Fold Print (clean) ===\n";
    print_fold_clean("A", 42, 3.14, "wow");

    cout << "\n=== 3. Perfect Forwarding Call ===\n";
    auto mul = [](auto a, auto b) { return a * b; };
    cout << call(mul, 8, 9) << "\n";

    cout << "\n=== 4. Sum / Product / Logic ===\n";
    cout << "sum = " << sum(1,2,3,4,5) << "\n";
    cout << "product = " << product(2,3,4) << "\n";
    cout << "all_true = " << boolalpha << all_true(true, 42 != 0, 5 > 0) << "\n";

    cout << "\n=== 5. Constrained Mean ===\n";
    cout << "mean(10, 20, 30) = " << mean(10, 20, 30) << "\n";
    // mean("error"); // compile error

    cout << "\n=== 6. Tuple with indices ===\n";
    auto t = make_tuple(10, "hello", 3.14);
    print_tuple_indexed(t);

    cout << "\n=== 7. Variadic Container ===\n";
    VariantContainer<int, string, double> vc(42, "test"s, 1.618);
    vc.print();

    cout << "\n=== 8. Logger with auto to_string ===\n";
    Logger log;
    log.log("Error #", 404, " at line ", 123, " [", true, "]");
    for (const auto& s : log.messages) cout << s << " ";
    cout << "\n";

    cout << "\n=== 9. make_vector from pack ===\n";
    auto vec = make_vector(1, 2, 3, 4, 5);
    for (auto x : vec) cout << x << " ";
    cout << "\n";

    cout << "\nAll variadic techniques demonstrated!\n";
    return 0;
}
