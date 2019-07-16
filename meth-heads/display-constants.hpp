#ifndef METH_HEAD_DISPLAY_CONSTANTS_HPP_INCLUDED
#define METH_HEAD_DISPLAY_CONSTANTS_HPP_INCLUDED

#include "cell.hpp"

#include <SFML/Graphics.hpp>

namespace methhead
{
    struct DisplayConstants
    {
        explicit DisplayConstants(const sf::Vector2u & windowSize);

        float pad_ratio;
        sf::Vector2f window_size;
        sf::FloatRect score_bounds;
        sf::FloatRect board_bounds;
        sf::Color cell_background_color;
        sf::Color cell_line_color;
        sf::Color lazy_color;
        sf::Color greedy_color;
        sf::Color prize_color;
        std::size_t column_count;
        std::size_t row_count;
        sf::Vector2u cellCountU;
        sf::Vector2i cellCountI;
        float line_thickness_ratio;
        float line_thickness;
        float cell_dimm;
        sf::Vector2f cell_size;
        float score_rectangle_width;
        sf::Font font;
        sf::Text default_text;

        BoardMap_t makeGameBoard() const;

    private:
        const sf::Vector2f cellToWindowPos(const sf::Vector2i & cellPos) const;

        const sf::RectangleShape
            makeGridRectangleShape(const sf::Vector2f & pos, const sf::Vector2f & size) const;
    };

} // namespace methhead

#endif // METH_HEAD_DISPLAY_CONSTANTS_HPP_INCLUDED
