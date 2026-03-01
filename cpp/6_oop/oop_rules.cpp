// rule_of_3_5_0_demo.cpp
// Complete C++17 tutorial and demonstration of the Rule of Three, Rule of Five, and Rule of Zero.
//
// This file shows:
// - Why special member functions matter for classes that manage resources.
// - Classic Rule of Three (C++98 to C++11).
// - Modern Rule of Five (C++11+ with move semantics).
// - Preferred Rule of Zero (let the compiler do everything).
//
// Compile: g++ -std=c++17 -Wall -Wextra -pedantic -O2 rule_of_3_5_0_demo.cpp -o rule_of_3_5_0_demo

#include <iostream>
#include <cstring>      // std::strlen, std::strcpy
#include <utility>      // std::move
#include <memory>

/*
    Resource-managing classes must control copying, moving, and destruction properly.
    If a class manages raw resources (heap memory, file handles, locks, etc.),
    the default compiler-generated functions may do the wrong thing (shallow copy).

    Rule of Three (pre-C++11):
    If you need to define ANY of destructor, copy constructor, or copy assignment,
    you probably need to define ALL three.

    Rule of Five (C++11+):
    If you need to define ANY of destructor, copy ctor, copy assign, move ctor, or move assign,
    you probably need to define ALL five.

    Rule of Zero (modern best practice):
    Design your class to own resources through objects that already follow the rule of five
    (e.g., std::unique_ptr, std::string, std::vector). Then you need to define ZERO special
    member functions — the compiler generates correct copy/move/destroy automatically.

    Interesting fact:
    Most modern C++ code follows the Rule of Zero. Explicitly writing the five functions
    is a code smell unless you have a very specific low-level reason.
*/

struct BadString {                  // Violates Rule of Three → buggy!
    char* data;

    BadString(const char* s) {
        data = new char[std::strlen(s) + 1];
        std::strcpy(data, s);
        std::cout << "BadString constructed: " << data << "\n";
    }

    ~BadString() {                  // Custom destructor needed
        std::cout << "BadString destroyed: " << data << "\n";
        delete[] data;
    }
    // Missing: copy constructor and copy assignment → shallow copy → double delete!
};

struct RuleOfThreeString {          // Correctly follows Rule of Three
    char* data;

    RuleOfThreeString(const char* s = "") {
        data = new char[std::strlen(s) + 1];
        std::strcpy(data, s);
        std::cout << "RuleOfThreeString constructed: " << data << "\n";
    }

    ~RuleOfThreeString() {
        std::cout << "RuleOfThreeString destroyed: " << data << "\n";
        delete[] data;
    }

    // Copy constructor
    RuleOfThreeString(const RuleOfThreeString& other) {
        data = new char[std::strlen(other.data) + 1];
        std::strcpy(data, other.data);
        std::cout << "RuleOfThreeString COPY constructed\n";
    }

    // Copy assignment
    RuleOfThreeString& operator=(const RuleOfThreeString& other) {
        if (this != &other) {               // self-assignment guard
            delete[] data;                  // free old resource
            data = new char[std::strlen(other.data) + 1];
            std::strcpy(data, other.data);
        }
        std::cout << "RuleOfThreeString COPY assigned\n";
        return *this;
    }
};

struct RuleOfFiveString {           // Adds move semantics (Rule of Five)
    char* data;

    RuleOfFiveString(const char* s = "") {
        data = new char[std::strlen(s) + 1];
        std::strcpy(data, s);
        std::cout << "RuleOfFiveString constructed: " << data << "\n";
    }

    ~RuleOfFiveString() {
        std::cout << "RuleOfFiveString destroyed: " << data << "\n";
        delete[] data;
    }

    // Copy operations (same as Rule of Three)
    RuleOfFiveString(const RuleOfFiveString& other) {
        data = new char[std::strlen(other.data) + 1];
        std::strcpy(data, other.data);
        std::cout << "RuleOfFiveString COPY constructed\n";
    }

    RuleOfFiveString& operator=(const RuleOfFiveString& other) {
        if (this != &other) {
            delete[] data;
            data = new char[std::strlen(other.data) + 1];
            std::strcpy(data, other.data);
        }
        std::cout << "RuleOfFiveString COPY assigned\n";
        return *this;
    }

    // Move constructor — steals resources
    RuleOfFiveString(RuleOfFiveString&& other) noexcept
        : data(other.data)
    {
        other.data = nullptr;               // leave moved-from object safe
        std::cout << "RuleOfFiveString MOVE constructed\n";
    }

    // Move assignment
    RuleOfFiveString& operator=(RuleOfFiveString&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            other.data = nullptr;
        }
        std::cout << "RuleOfFiveString MOVE assigned\n";
        return *this;
    }
};

struct RuleOfZeroString {           // Modern best practice
    std::string data;               // std::string already manages memory perfectly

    RuleOfZeroString(const char* s = "") : data(s) {
        std::cout << "RuleOfZeroString constructed: " << data << "\n";
    }
    // No destructor, no copy/move functions needed → Rule of Zero!
};

// Just use std::string! But if you MUST manage raw memory...
struct BetterString {
    std::unique_ptr<char[]> data;  // Smart pointer!

    BetterString(const char* s)
        : data(std::make_unique<char[]>(std::strlen(s) + 1)) {
        std::strcpy(data.get(), s);
    }

    // Rule of Zero! No destructor, no copy/move needed
    // unique_ptr handles everything automatically

    // Note: unique_ptr disables copying (good - prevents shallow copy)
    // But enables moving automatically
};

void demo_bad()
{
    std::cout << "\n=== Demo 1: BadString (violates Rule of Three) ===\n";
    BadString a("hello");
    BadString b = a;                // shallow copy → both point to same memory
    // When both destruct → double delete → crash or corruption!
    std::cout << "End of scope → double delete likely crashes here!\n";
}

void demo_rule_of_three()
{
    std::cout << "\n=== Demo 2: RuleOfThreeString (safe copying) ===\n";
    RuleOfThreeString x("world");
    RuleOfThreeString y = x;        // copy constructor
    y = x;                          // copy assignment (self-assignment safe)
}

void demo_rule_of_five()
{
    std::cout << "\n=== Demo 3: RuleOfFiveString (with move semantics) ===\n";
    RuleOfFiveString tmp("temporary");
    RuleOfFiveString moved = std::move(tmp);  // move constructor
}

void demo_rule_of_zero()
{
    std::cout << "\n=== Demo 4: RuleOfZeroString (modern best practice) ===\n";
    RuleOfZeroString modern("clean and simple");
    RuleOfZeroString copy = modern;           // deep copy automatically
    RuleOfZeroString moved = std::move(modern); // move automatically
}


void demo_better_string()
{
    std::cout << "\n=== Demo 4: smart pointers solve it (modern best practice) ===\n";
    BetterString modernn("clean and simple");
    //BetterString copy = modernn;           // deep copy disabled unique pointer
    BetterString moved = std::move(modernn); // move automatically
}

int main()
{
    std::cout << "Rule of 3 / 5 / 0 Tutorial & Demo\n";
    std::cout << "==================================\n";

    // demo_bad();          // Uncomment to see crash (undefined behavior)
    demo_rule_of_three();
    demo_rule_of_five();
    demo_rule_of_zero();
    demo_better_string();

    std::cout << "\n=== Summary ===\n";
    std::cout << "- If you manage raw resources → follow Rule of Five.\n";
    std::cout << "- Writing all five functions is verbose and error-prone.\n";
    std::cout << "- Prefer Rule of Zero: use std::string, std::vector, std::unique_ptr, etc.\n";
    std::cout << "- Rule of Zero gives you correct copy/move/destroy for free and is exception-safe.\n";
    std::cout << "\nTrick: Many style guides (C++ Core Guidelines) say:\n";
    std::cout << "  'If you have to write a destructor, you probably have a bug.'\n";
    std::cout << "  → Design classes to own RAII objects instead.\n";

    return 0;
}
