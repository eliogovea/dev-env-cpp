#pragma once

#include <cstdint>
#include <type_traits>

namespace Geometry {

enum class ValueSign : std::int8_t
{
    Negative = -1,
    Zero     = 0,
    Positive = +1,
};

inline auto operator*(ValueSign lhs, ValueSign rhs) -> ValueSign
{
    return static_cast<ValueSign>(static_cast<std::int8_t>(lhs)
                                  * static_cast<std::int8_t>(rhs));
}

template <typename Value>
auto GetValueSign(Value&& value) -> ValueSign
{
    constexpr auto ValueZero = std::remove_cvref_t<Value>{0};

    if (value < ValueZero) {
        return ValueSign::Negative;
    }

    if (ValueZero < value) {
        return ValueSign::Positive;
    }

    return ValueSign::Zero;
}

}  // namespace Geometry
