#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <vector>

#include "index_coordinator.hpp"

extern "C" int LLVMFuzzerTestOneInput(std::uint8_t const* data,
                                      std::size_t         size)
{
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    auto index = IndexCoordinator<std::uint8_t, 256U>{};

    auto taken = std::vector<std::uint16_t>{};

    taken.reserve(index.capacity());

    while (2U <= size) {
        if (index.size() == index.capacity()) {
            index.give(taken.back());
            taken.pop_back();
            continue;
        }

        if (index.size() == 0U) {
            taken.push_back(index.take());
            continue;
        }

        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        auto const op_type = data[--size];

        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        auto const op_value = data[--size];

        switch (op_type & 0x03U) {
        case 0x00: {  // take
            taken.push_back(index.take());
        } break;

        case 0x01: {  // give
            index.give(taken.back());
            taken.pop_back();
        } break;

        case 0x02:
        case 0x03: {  // give
            auto const lhs = taken.size() - 1U;
            auto const rhs = lhs < op_value ? lhs : op_value;
            std::swap(taken[lhs], taken[rhs]);
        } break;

        default: {
            std::abort();
        }
        }
    }

    return EXIT_SUCCESS;
}
