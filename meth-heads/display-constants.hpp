#ifndef METH_HEAD_DISPLAY_CONSTANTS_HPP_INCLUDED
#define METH_HEAD_DISPLAY_CONSTANTS_HPP_INCLUDED

#include "cell.hpp"

#include <SFML/Graphics.hpp>

namespace methhead
{
    struct DisplayConstants
    {
        explicit DisplayConstants(const sf::Vector2u & windowSize);

        sf::Vector2f window_size;
        sf::FloatRect score_bounds;
        sf::FloatRect board_bounds;

        sf::Color lazy_color;
        sf::Color greedy_color;
        std::size_t column_count;
        std::size_t row_count;
        sf::Vector2u cellCountsU;
        sf::Vector2i cellCountsI;
        float line_thickness;
        float cell_dimm;
        sf::Vector2f cell_size;
        float score_rectangle_width;
        sf::Font font;
        sf::Text default_text;
    };

} // namespace methhead

#endif // METH_HEAD_DISPLAY_CONSTANTS_HPP_INCLUDED
