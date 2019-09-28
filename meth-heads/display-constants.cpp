// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "display-constants.hpp"

#include "utils.hpp"

#include <cmath>
#include <iostream>

namespace methhead
{
    DisplayConstants::DisplayConstants(const sf::Vector2u & windowSize)
        : window_size(windowSize)
        , score_bounds(0.0f, 0.0f, (0.2f * window_size.x), window_size.y)
        , board_bounds(
              (score_bounds.left + score_bounds.width),
              score_bounds.top,
              (window_size.x - score_bounds.width),
              score_bounds.height)
        , lazy_color { 80, 80, 255 } // TODO
        , greedy_color { 100, 255, 100 } // TODO
        , column_count { 20 }
        , row_count { column_count }
        , cellCountsU(static_cast<unsigned int>(column_count), static_cast<unsigned int>(row_count))
        , cellCountsI(cellCountsU)
        , line_thickness((1.0f / 600.0f) * window_size.y)
        , cell_dimm(window_size.y / static_cast<float>(std::max(column_count, row_count)))
        , cell_size(cell_dimm, cell_dimm)
        , score_rectangle_width(score_bounds.width * 0.5f)
        , font()
        , default_text()
    {
        const auto FONT_FILE_PATH("font/gentium-plus.ttf");

        if (!font.loadFromFile(FONT_FILE_PATH))
        {
            std::cout << "Failed to load font: \"" << FONT_FILE_PATH << "\"" << std::endl;
        }

        default_text.setCharacterSize(static_cast<unsigned int>(std::sqrt(window_size.x)));
        default_text.setFont(font);
    }

} // namespace methhead
