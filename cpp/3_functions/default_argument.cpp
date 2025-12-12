#include <iostream>
#include <string>
#include <vector>

using namespace std;

/* -----------------------------------------------------------
   1. BASIC DEFAULT ARGUMENTS
   -----------------------------------------------------------*/
// Default values are bound at the *call site*, not at function body.
// They must appear in the declaration, not repeated in the definition.
int add(int a, int b = 10) { return a + b; }

/* -----------------------------------------------------------
   2. POINTER, REF, CONST & COMPLEX TYPES
   -----------------------------------------------------------*/
void log_msg(const string& msg = "no-msg",
             int level = 1,
             const char* src = nullptr)
{
    cout << "[L" << level << "] "
         << (src ? src : "<unknown>")
         << ": " << msg << "\n";
}

/* -----------------------------------------------------------
   3. CTOR DEFAULT ARGUMENTS
   -----------------------------------------------------------*/
struct Config {
    int threads;
    double timeout;

    // default arguments inside constructor
    Config(int t = 4, double to = 0.5)
        : threads(t), timeout(to) {}
};

/* -----------------------------------------------------------
   4. MEMBER FUNCTIONS WITH DEFAULTS
   -----------------------------------------------------------*/
struct Engine {
    void start(int max_rpm = 6000) {
        cout << "Engine starting at " << max_rpm << " RPM\n";
    }
};

/* -----------------------------------------------------------
   5. DEFAULT ARGUMENTS + OVERLOADING PITFALL
      Ambiguity if overloading interacts with default args.
   -----------------------------------------------------------*/
void f(int a, int b = 42) {
    cout << "f(int,int)\n";
}

// void f(int a) { }          // BAD: ambiguous with default b

/* -----------------------------------------------------------
   6. DEFAULT ARGUMENTS + TEMPLATES
   -----------------------------------------------------------*/
template <typename T = int>
T identity(T v = T{}) {      // default type + default value
    return v;
}

/* -----------------------------------------------------------
   7. DEFAULT ARGUMENTS AND EVALUATION ORDER
      - expression is evaluated at call site
      - beware of expensive expressions
   -----------------------------------------------------------*/
int heavy() {
    cout << "(heavy computation)\n";
    return 99;
}

void g(int x = heavy()) {
    cout << "g called with " << x << "\n";
}

/* -----------------------------------------------------------
   9. BAD PRACTICE: REPEATED DEFAULT IN DECL + DEF
   -----------------------------------------------------------*/
// int bad(int x = 5);       // decl
// int bad(int x = 10) { }   // def → compile error (redefinition)

int main() {
    cout << "=== Basic ===\n";
    cout << add(1) << "\n";       // uses 10
    cout << add(1,2) << "\n";     // explicit

    cout << "\n=== Logging ===\n";
    log_msg("startup", 2, "core");
    log_msg();                    // all defaults

    cout << "\n=== Constructor Defaults ===\n";
    Config c1;                    // t=4, to=0.5
    Config c2(8);                 // to=0.5
    cout << c1.threads << "," << c1.timeout << "\n";
    cout << c2.threads << "," << c2.timeout << "\n";

    cout << "\n=== Member Function Defaults ===\n";
    Engine e;
    e.start();                    // 6000

    cout << "\n=== Template Defaults ===\n";
    cout << identity() << "\n";         // T=int, v=0
    cout << identity<double>() << "\n"; // T=double, v=0.0

    cout << "\n=== Expensive Default Expression ===\n";
    g();                          // heavy() evaluated
    g(5);                         // heavy() NOT evaluated

    return 0;
}
