#pragma once

#include <sstream>

namespace SVG {

struct Svg
{
    int width;
    int height;
};

struct Circle
{
    int cx;
    int cy;
    int r;
};

struct Line
{
    int x1;
    int y1;
    int x2;
    int y2;
};

class SvgBuilder
{
   public:
    SvgBuilder() = delete;

    SvgBuilder(SvgBuilder const&)     = delete;
    SvgBuilder(SvgBuilder&&) noexcept = default;

    auto operator=(SvgBuilder const&) -> SvgBuilder&     = delete;
    auto operator=(SvgBuilder&&) noexcept -> SvgBuilder& = default;

    ~SvgBuilder() = default;

    explicit SvgBuilder(Svg svg);

    auto AddCircle(SVG::Circle const& circle) -> SvgBuilder&;

    auto AddLine(SVG::Line const& line) -> SvgBuilder&;

    auto GetSvgString() const -> std::string;

   private:
    std::ostringstream stream_;
};

}  // namespace SVG
