#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// ===================================================================
// CRTP (Curiously Recurring Template Pattern) – Full Example
// ===================================================================
//
// What is CRTP?
// -----------------
// CRTP is an idiomatic C++ technique where a class Derived inherits
// from a template base class that takes Derived itself as a template
// parameter:
//
//     template <typename Derived>
//     struct Base { ... };
//
//     struct MyType : Base<MyType> { ... };
//
// This looks "curious" (recurring), but it gives the base class
// knowledge of the exact derived type at compile time.
//
// Why use CRTP?
// -------------
// 1. Static polymorphism – avoid virtual function overhead (zero-cost)
// 2. Enable the base to call derived-class methods efficiently
// 3. Share common code between derived classes without dynamic dispatch
// 4. Implement mixins and policy-based design
//
// Key trick: the base casts `this` to the derived type using
// static_cast<Derived&>(*this) – completely safe and resolved at compile time.

namespace crtp_example {

// -------------------------------------------------------------------
// Example 1: Static interface (zero-cost alternative to virtual functions)
// -------------------------------------------------------------------
template <typename Derived>
struct ShapeBase
{
    // Public interface that forwards to the derived class
    void draw() const
    {
        // static_cast is resolved at compile time → zero runtime cost
        static_cast<const Derived&>(*this).draw_impl();
    }

    void rotate(double angle)
    {
        static_cast<Derived&>(*this).rotate_impl(angle);
    }

    // We can also provide common functionality here
    void print_info() const
    {
        std::cout << "Shape type: ";
        static_cast<const Derived&>(*this).name();
        std::cout << '\n';
        draw();
    }
};

// Concrete shapes – no virtual functions anywhere!
struct Circle : ShapeBase<Circle>
{
    double radius = 5.0;

    void draw_impl() const { std::cout << "Drawing Circle(r=" << radius << ")\n"; }
    void rotate_impl(double) { std::cout << "Circle rotation does nothing\n"; }
    void name() const { std::cout << "Circle"; }
};

struct Square : ShapeBase<Square>
{
    double side = 4.0;

    void draw_impl() const { std::cout << "Drawing Square(side=" << side << ")\n"; }
    void rotate_impl(double angle) { std::cout << "Rotating Square by " << angle << " degrees\n"; }
    void name() const { std::cout << "Square"; }
};

// -------------------------------------------------------------------
// Example 2: CRTP for code reuse – Comparable mixin
// -------------------------------------------------------------------
template <typename Derived>
struct Comparable
{
    // operator< uses the derived class's operator==
    bool operator>(const Comparable& other) const
    {
        const auto& self = static_cast<const Derived&>(*this);
        const auto& rhs  = static_cast<const Derived&>(other);
        return rhs < self;  // reuse operator<
    }

    bool operator<=(const Comparable& other) const
    {
        return !(*this > other);
    }

    bool operator>=(const Comparable& other) const
    {
        return !(*this < other);
    }
};

// Example 3: CRTP for counting object instances (very common use)
// -------------------------------------------------------------------
template <typename Derived>
struct Counter
{
    static inline std::size_t count = 0;  // C++17 inline static variable

    Counter() { ++count; }
    Counter(const Counter&) { ++count; }
    Counter(Counter&&) noexcept { ++count; }
    ~Counter() { --count; }

    static std::size_t alive() noexcept { return count; }
};

struct Widget : Counter<Widget>
{
    std::string name;
    Widget(std::string n) : name(std::move(n)) {}
};

// -------------------------------------------------------------------
// Example 4: Combining multiple CRTP bases (multiple inheritance)
// -------------------------------------------------------------------
struct Point
    : ShapeBase<Point>          // gets draw/rotate interface
    , Comparable<Point>         // gets >, <=, >= from operator<
    , Counter<Point>            // gets instance counting
{
    double x = 0.0, y = 0.0;
    Point(double x_, double y_) : x(x_), y(y_) {}

    // Required by ShapeBase
    void draw_impl() const { std::cout << "Point(" << x << ',' << y << ")\n"; }
    void rotate_impl(double) {}  // points don't rotate
    void name() const { std::cout << "Point"; }

    // Required by Comparable – only implement < and ==
    bool operator<(const Point& other) const
    {
        // Lexicographical order for demo
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    bool operator==(const Point& other) const
    {
        return x == other.x && y == other.y;
    }
};

} // namespace crtp_example

// ===================================================================
// Demo
// ===================================================================
int main()
{
    using namespace crtp_example;

    std::cout << "=== Shape interface (static polymorphism) ===\n";
    Circle c;
    Square s;

    c.print_info();  // Circle → draws circle
    s.print_info();  // Square → draws square
    s.rotate(45);    // Square rotates

    std::cout << "\n=== Comparable mixin ===\n";
    Point p1(1.0, 2.0);
    Point p2(3.0, 4.0);
    Point p3(1.0, 2.0);

    std::cout << std::boolalpha;
    std::cout << "(p1 < p2) = " << (p1 < p2) << '\n';   // true
    std::cout << "(p1 > p2) = " << (p1 > p2) << '\n';   // false (uses CRTP-generated >)
    std::cout << "(p1 == p3) = " << (p1 == p3) << '\n'; // true

    std::cout << "\n=== Instance counter ===\n";
    std::cout << "Widgets alive: " << Widget::alive() << '\n';  // 0
    {
        Widget w1("one"), w2("two");
        std::cout << "Inside scope: " << Widget::alive() << '\n';  // 2
    }
    std::cout << "After scope: " << Widget::alive() << '\n';    // 0

    std::cout << "\n=== Points alive counter ===\n";
    std::cout << "Points alive: " << Point::alive() << '\n';  // 3 (p1,p2,p3 still alive)
    return 0;
}
