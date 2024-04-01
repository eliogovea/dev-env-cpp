#include "svg_image.hpp"

#include <format>
#include <string>

namespace SVG {

SvgBuilder::SvgBuilder(Svg svg)
{
    auto constexpr format
        = R"(<svg width="{}" height="{}" xmlns="http://www.w3.org/2000/svg">)";

    stream_ << std::format(format, svg.width, svg.height);
}

auto SvgBuilder::GetSvgString() const -> std::string
{
    return std::format("{}{}", stream_.str(), "</svg>");
}

auto SvgBuilder::AddCircle(SVG::Circle const& circle) -> SvgBuilder&
{
    auto constexpr format
        = R"(<circle cx="{}" cy="{}" r="{}" stroke="black" fill="none"/>)";

    stream_ << std::format(format, circle.cx, circle.cy, circle.r);
    return *this;
}

auto SvgBuilder::AddLine(SVG::Line const& line) -> SvgBuilder&
{
    auto constexpr format
        = R"(<line x1="{}" y1="{}" x2="{}" y2="{}" stroke="black"/>)";

    stream_ << std::format(format, line.x1, line.y1, line.x2, line.y2);
    return *this;
}

}  // namespace SVG
