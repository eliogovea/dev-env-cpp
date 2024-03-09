#include <cstdint>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

#include "power.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "unknown arguments\n";
        std::cout << "usage: " << argv[0] << " base exponent\n";  // NOLINT
        return EXIT_FAILURE;
    }

    try {
        std::uint64_t const base     = std::stoll(argv[1]);  // NOLINT
        std::uint64_t const exponent = std::stoll(argv[2]);  // NOLINT
        std::cout << Math::Power(base, exponent) << "\n";
    } catch (std::exception& e) {
        std::cout << "error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
