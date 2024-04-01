#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

#include "svg_image.hpp"

auto GenerateSvgImage(SVG::Svg svg) -> std::string
{
    return SVG::SvgBuilder{svg}
        .AddCircle({
            .cx = svg.width / 2,
            .cy = svg.height / 2,
            .r  = std::min(svg.width, svg.height) / 4,
        })
        .AddLine({
            .x1 = 0,
            .y1 = 0,
            .x2 = svg.width,
            .y2 = svg.height,
        })
        .GetSvgString();
}

int main()
{
    try {
        std::cout << GenerateSvgImage({.width = 1000, .height = 500})
                  << std::flush;
        return EXIT_SUCCESS;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n" << std::flush;
        return EXIT_FAILURE;
    }
}
