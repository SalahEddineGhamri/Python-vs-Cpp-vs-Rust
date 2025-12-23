#include <iostream>
#include <vector>

/*
 * overload the insertion operator to use class with "std::cout"
  class Person {
      std::string name_;
      int age_;
  public:
      Person(std::string n, int a) : name_(std::move(n)), age_(a) {}

      // Overload << as a friend (needs access to private members)
      friend std::ostream& operator<<(std::ostream& os, const Person& p) {
          os << "Person(name=" << p.name_ << ", age=" << p.age_ << ")";
          return os; // return stream for chaining
      }
  };
*/

class Demo {
    int value_;
public:
    Demo(int v = 0) : value_(v) {}

    // Arithmetic
    Demo operator+(const Demo& rhs) const { return Demo(value_ + rhs.value_); }
    Demo operator-(const Demo& rhs) const { return Demo(value_ - rhs.value_); }
    Demo operator*(const Demo& rhs) const { return Demo(value_ * rhs.value_); }
    Demo operator/(const Demo& rhs) const { return Demo(value_ / rhs.value_); }
    Demo operator%(const Demo& rhs) const { return Demo(value_ % rhs.value_); }
    Demo& operator++() { ++value_; return *this; }    // prefix
    Demo operator++(int) { Demo tmp = *this; ++value_; return tmp; } // postfix
    Demo& operator--() { --value_; return *this; }
    Demo operator--(int) { Demo tmp = *this; --value_; return tmp; }

    // Comparison
    bool operator==(const Demo& rhs) const { return value_ == rhs.value_; }
    bool operator!=(const Demo& rhs) const { return value_ != rhs.value_; }
    bool operator<(const Demo& rhs) const { return value_ < rhs.value_; }
    bool operator<=(const Demo& rhs) const { return value_ <= rhs.value_; }
    bool operator>(const Demo& rhs) const { return value_ > rhs.value_; }
    bool operator>=(const Demo& rhs) const { return value_ >= rhs.value_; }

    // Logical / Bitwise
    Demo operator&(const Demo& rhs) const { return Demo(value_ & rhs.value_); }
    Demo operator|(const Demo& rhs) const { return Demo(value_ | rhs.value_); }
    Demo operator^(const Demo& rhs) const { return Demo(value_ ^ rhs.value_); }
    Demo operator~() const { return Demo(~value_); }
    Demo operator<<(int shift) const { return Demo(value_ << shift); }
    Demo operator>>(int shift) const { return Demo(value_ >> shift); }

    // Assignment
    Demo& operator=(const Demo& rhs) { value_ = rhs.value_; return *this; }
    Demo& operator+=(const Demo& rhs) { value_ += rhs.value_; return *this; }
    Demo& operator-=(const Demo& rhs) { value_ -= rhs.value_; return *this; }
    Demo& operator*=(const Demo& rhs) { value_ *= rhs.value_; return *this; }
    Demo& operator/=(const Demo& rhs) { value_ /= rhs.value_; return *this; }
    Demo& operator%=(const Demo& rhs) { value_ %= rhs.value_; return *this; }
    Demo& operator&=(const Demo& rhs) { value_ &= rhs.value_; return *this; }
    Demo& operator|=(const Demo& rhs) { value_ |= rhs.value_; return *this; }
    Demo& operator^=(const Demo& rhs) { value_ ^= rhs.value_; return *this; }
    Demo& operator<<=(int shift) { value_ <<= shift; return *this; }
    Demo& operator>>=(int shift) { value_ >>= shift; return *this; }

    // Unary
    Demo operator+() const { return *this; }
    Demo operator-() const { return Demo(-value_); }
    Demo operator!() const { return Demo(!value_); }

    // Pointer-like / Stream
    friend std::ostream& operator<<(std::ostream& os, const Demo& d) {
        return os << d.value_;
    }

    friend std::istream& operator>>(std::istream& is, Demo& d) {
        return is >> d.value_;
    }

    // Comma operator
    Demo operator,(const Demo& rhs) const { return rhs; }
};

int main() {
    Demo a(5), b(2);

    Demo c = a + b;
    std::cout << "a+b = " << c << "\n";

    c = a & b;
    std::cout << "a&b = " << c << "\n";

    ++c;
    std::cout << "++c = " << c << "\n";

    c = a, b; // comma
    std::cout << "comma result = " << c << "\n";
}
