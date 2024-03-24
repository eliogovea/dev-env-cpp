#include <concepts>
#include <cstdint>

namespace Power {

/// @concept Multipliable
/// @brief A concept to check if a type supports multiplication operations.
/// @tparam T The type to check against the `Multipliable` requirements.
template <typename T>
concept Multipliable = requires(T lhs, T rhs) {
    // clang-format off
    {lhs * rhs} -> std::convertible_to<T>;
    // clang-format on
};

/// @brief Calculates the power of a base raised to an exponent
///
/// @tparam Base The base. Must support multiplication (*)
/// @param base The base of type Base
/// @param exponent A non-negative integer representing the exponent.
///
/// @return The result of raising base to the power of exponent.
template <Multipliable Base, std::unsigned_integral Exponent>
constexpr auto Power(Base base, Exponent exponent) -> Base
{
    auto power = Base{1};
    while (exponent > Exponent{0U}) {
        if (exponent & Exponent{1U}) {
            power *= base;
        }
        base *= base;
        exponent >>= Exponent{1U};
    }
    return power;
}

}  // namespace Power
