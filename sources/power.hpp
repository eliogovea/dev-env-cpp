#include <cstdint>

namespace Math {

template <typename T>
concept Multipliable = requires(T lhs, T rhs) {
    // clang-format off
    lhs * rhs;
    // clang-foramt on
};

template <Multipliable Base>
constexpr auto Power(Base base, std::uint64_t exponent) -> Base {
    Base result = {1};
    while (exponent > 0) {
        if (exponent & std::uint64_t {1}) {
            result = result * base;
        }
        base = base * base;
        exponent >>= std::uint64_t {1};
    }
    return result;
}

}  // namespace Math
