#ifndef METHHEADS_DISPLAY_CONSTANTS_HPP_INCLUDED
#define METHHEADS_DISPLAY_CONSTANTS_HPP_INCLUDED
//
// display-constants.hpp
//
#include "utils.hpp"

#include <SFML/Graphics.hpp>

namespace methhead
{
    struct DisplayConstants
    {
        explicit DisplayConstants(const sf::Vector2u & windowSize);

        sf::Vector2f boardPosToWindowPos(const BoardPos_t & boardPos) const;
        sf::FloatRect boardPosToWindowRect(const BoardPos_t & boardPos) const;

        // just smaller than the actual window size to create a border
        sf::FloatRect inner_window_rect;

        sf::FloatRect score_rect;
        sf::FloatRect board_rect;
        sf::FloatRect fps_rect;

        sf::Vector2f cell_size;
        std::size_t horiz_cell_count;
        std::size_t vert_cell_count;

        float cell_line_thickness;
        sf::Color cell_line_color;
        sf::Color cell_background_color;

        sf::Font font;
        sf::Text default_text;

        sf::Texture loot_texture;
        sf::Texture lazy_texture;
        sf::Texture greedy_texture;

        // TODO these two probably don't belong here
        sf::Color lazy_color;
        sf::Color greedy_color;
    };
} // namespace methhead

#endif // METHHEADS_DISPLAY_CONSTANTS_HPP_INCLUDED