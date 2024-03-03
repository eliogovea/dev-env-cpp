#include <cstdint>

namespace Math
{

template <typename T>
concept Multipliable = requires(T lhs, T rhs) {
    // clang-format off
    lhs * rhs;
    // clang-format on
};

template <Multipliable Base>
constexpr auto Power(Base base, std::uint64_t exponent) -> Base
{
    auto power = Base{1};
    while (exponent > 0) {
        if (exponent & std::uint64_t{1}) {
            power = power * base;
        }
        base     = base * base;
        exponent = exponent >> std::uint64_t{1};
    }
    return power;
}

}  // namespace Math
