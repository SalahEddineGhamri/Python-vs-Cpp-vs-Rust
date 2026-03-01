#include <iostream>
#include <type_traits>

// ===============================================================
// Custom trait: is_integer<T>
// Alias-safe, CV-correct, no <cstdint> duplication
// ===============================================================

namespace trait {

// Primary template (fallback)
template <typename T>
struct is_integer_impl : std::false_type {};

// Fundamental integer specializations
template <> struct is_integer_impl<bool>               : std::true_type {};
template <> struct is_integer_impl<char>               : std::true_type {};
template <> struct is_integer_impl<signed char>        : std::true_type {};
template <> struct is_integer_impl<unsigned char>      : std::true_type {};
template <> struct is_integer_impl<short>              : std::true_type {};
template <> struct is_integer_impl<unsigned short>     : std::true_type {};
template <> struct is_integer_impl<int>                : std::true_type {};
template <> struct is_integer_impl<unsigned int>       : std::true_type {};
template <> struct is_integer_impl<long>               : std::true_type {};
template <> struct is_integer_impl<unsigned long>      : std::true_type {};
template <> struct is_integer_impl<long long>          : std::true_type {};
template <> struct is_integer_impl<unsigned long long> : std::true_type {};

// Character integer types
template <> struct is_integer_impl<wchar_t>  : std::true_type {};
template <> struct is_integer_impl<char16_t> : std::true_type {};
template <> struct is_integer_impl<char32_t> : std::true_type {};
#ifdef __cpp_char8_t
template <> struct is_integer_impl<char8_t>  : std::true_type {};
#endif

// Public trait: strips cv-qualifiers first
template <typename T>
struct is_integer
  : is_integer_impl<std::remove_cv_t<T>> {};

// C++17 convenience variable
template <typename T>
inline constexpr bool is_integer_v = is_integer<T>::value;

} // namespace trait

// ===============================================================
// SFINAE usage
// ===============================================================

namespace sfinae {

template <typename T>
std::enable_if_t<trait::is_integer_v<T>, void>
print(const T& value)
{
    std::cout << "[INTEGER] " << value
              << " (size: " << sizeof(T) << ")\n";
}

template <typename T>
std::enable_if_t<!trait::is_integer_v<T>, void>
print(const T& value)
{
    std::cout << "[NON-INTEGER] " << value << '\n';
}

} // namespace sfinae

// ===============================================================
// Demo
// ===============================================================

int main()
{
    using namespace sfinae;

    print(42);
    print(3.14);
    print(true);
    print('A');
    print("hello");
    print(123ULL);

    static_assert(trait::is_integer_v<int>);
    static_assert(trait::is_integer_v<const volatile unsigned long>);
    static_assert(!trait::is_integer_v<double>);
}

