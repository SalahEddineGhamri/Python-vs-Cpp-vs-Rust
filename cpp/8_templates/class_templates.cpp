// class_templates_demo.cpp
// Complete C++17 tutorial and demonstration of class templates.
//
// Covers:
// - Basic class template definition and instantiation
// - Multiple template parameters
// - Default template arguments
// - Template specialization (full and partial)
// - Member templates
// - Non-type template parameters
// - Common pitfalls and best practices
// - Comparison with std::vector and std::pair
//
// Compile: g++ -std=c++17 -Wall -Wextra -pedantic -O2 class_templates_demo.cpp -o class_templates_demo

#include <iostream>
#include <string>
#include <vector>

// ------------------------------------------------------------------
// 1. Basic class template
// ------------------------------------------------------------------
/*
    A class template defines a family of classes parameterized by one or more types
    or values. The compiler generates a concrete class only when you instantiate it.
*/

template <typename T>
class Box
{
private:
    T value;

public:
    explicit Box(T v = T{}) : value(v) {}   // default constructs T if no argument

    void set(T v) { value = v; }
    T get() const { return value; }

    void print() const
    {
        std::cout << "Box contains: " << value << "\n";
    }
};

// ------------------------------------------------------------------
// 2. Class template with multiple parameters and defaults
// ------------------------------------------------------------------
template <typename T, typename Allocator = std::allocator<T>>
class MyVector
{
    // In real code this would use Allocator to manage memory.
    // Here we just wrap std::vector for simplicity.
    std::vector<T, Allocator> data;

public:
    void push_back(const T& v) { data.push_back(v); }
    void push_back(T&& v)      { data.push_back(std::move(v)); }

    void print() const
    {
        std::cout << "MyVector [ ";
        for (const auto& elem : data) std::cout << elem << ' ';
        std::cout << "]\n";
    }
};

// ------------------------------------------------------------------
// 3. Non-type template parameter
// ------------------------------------------------------------------
template <typename T, int Size>
class FixedArray
{
    T arr[Size];

public:
    FixedArray() = default;

    T& operator[](int i) { return arr[i]; }
    const T& operator[](int i) const { return arr[i]; }

    static constexpr int size() { return Size; }

    void print() const
    {
        std::cout << "FixedArray<" << Size << "> [ ";
        for (int i = 0; i < Size; ++i) std::cout << arr[i] << ' ';
        std::cout << "]\n";
    }
};

// ------------------------------------------------------------------
// 4. Full template specialization
// ------------------------------------------------------------------
template <>
class Box<std::string>
{
    std::string value;

public:
    explicit Box(std::string v = "") : value(std::move(v)) {}

    void set(std::string v) { value = std::move(v); }
    std::string get() const { return value; }

    void print() const
    {
        std::cout << "Specialized Box<string>: \"" << value << "\"\n";
    }
};

// ------------------------------------------------------------------
// 5. Partial specialization
// ------------------------------------------------------------------
template <typename T>
class Box<T*>
{
    T* ptr;

public:
    explicit Box(T* p = nullptr) : ptr(p) {}

    void set(T* p) { ptr = p; }
    T* get() const { return ptr; }

    void print() const
    {
        if (ptr)
            std::cout << "Box<pointer> holds address " << ptr
                      << " pointing to " << *ptr << "\n";
        else
            std::cout << "Box<pointer> holds nullptr\n";
    }
};

// ------------------------------------------------------------------
// 6. Member template (template function inside a class)
// ------------------------------------------------------------------
template <typename T>
class Wrapper
{
    T obj;

public:
    Wrapper(T o) : obj(std::move(o)) {}

    // Member template: convert to another type if possible
    template <typename U>
    U convert_to() const
    {
        return static_cast<U>(obj);
    }

    void print() const { std::cout << "Wrapper holds: " << obj << "\n"; }
};

// ------------------------------------------------------------------
// Main demonstration
// ------------------------------------------------------------------
int main()
{
    std::cout << "Class Templates Tutorial & Demo\n";
    std::cout << "================================\n\n";

    // 1. Basic instantiation
    std::cout << "=== 1. Basic Box ===\n";
    Box<int> intBox(42);
    intBox.print();

    Box<double> doubleBox(3.14);
    doubleBox.print();

    // 2. Multiple parameters + default
    std::cout << "\n=== 2. MyVector ===\n";
    MyVector<std::string> names;
    names.push_back("Alice");
    names.push_back("Bob");
    names.print();

    // 3. Non-type parameter
    std::cout << "\n=== 3. FixedArray ===\n";
    FixedArray<int, 5> fiveInts;
    for (int i = 0; i < fiveInts.size(); ++i) fiveInts[i] = i * 10;
    fiveInts.print();

    // 4. Full specialization
    std::cout << "\n=== 4. Specialized Box<string> ===\n";
    Box<std::string> strBox("Hello template specialization!");
    strBox.print();

    // 5. Partial specialization for pointers
    std::cout << "\n=== 5. Box for pointers ===\n";
    int x = 100;
    Box<int*> ptrBox(&x);
    ptrBox.print();

    Box<char*> nullBox(nullptr);
    nullBox.print();

    // 6. Member template
    std::cout << "\n=== 6. Member template conversion ===\n";
    Wrapper<int> w(123);
    w.print();
    double d = w.convert_to<double>();
    std::cout << "Converted to double: " << d << "\n";

    std::cout << "\n=== Summary ===\n";
    std::cout << "- Class templates generate type-safe families of classes.\n";
    std::cout << "- Use default template arguments for flexibility (like std::vector).\n";
    std::cout << "- Non-type parameters enable compile-time sizes (C++20 improves this with class types).\n";
    std::cout << "- Specializations let you optimize for specific types (e.g., pointers, strings).\n";
    std::cout << "- Member templates are useful for generic conversion or construction.\n";
    std::cout << "\nInteresting facts & tricks:\n";
    std::cout << "- std::vector, std::pair, std::tuple, std::optional are all class templates.\n";
    std::cout << "- Explicit instantiation (template class Box<int>;) can reduce compile time in large projects.\n";
    std::cout << "- Avoid deep template nesting — it hurts compile times and error messages.\n";
    std::cout << "- C++20 concepts can constrain template parameters for clearer errors.\n";

    return 0;
}
