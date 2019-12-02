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

        inline sf::Vector2f boardPosToWindowPos(const BoardPos_t & boardPos) const noexcept
        {
            return (
                sf::Vector2f(board_rect.left, board_rect.top) +
                (sf::Vector2f(boardPos) * cell_size));
        }

        inline sf::FloatRect boardPosToWindowRect(const BoardPos_t & boardPos) const noexcept
        {
            return sf::FloatRect(boardPosToWindowPos(boardPos), cell_size);
        }

        inline BoardPos_t indexToBoardPos(const std::size_t index) const noexcept
        {
            return { static_cast<int>(index % cell_counts.x),
                     static_cast<int>(index / cell_counts.x) };
        }

        inline std::size_t boardPosToIndex(const BoardPos_t & pos) const noexcept
        {
            return static_cast<std::size_t>((pos.y * cell_countsI.x) + pos.x);
        }

        // just smaller than the actual window size to create a border
        sf::FloatRect inner_window_rect;

        sf::FloatRect score_rect;
        sf::FloatRect board_rect;
        sf::FloatRect fps_rect;

        sf::Vector2f cell_size;
        sf::Vector2s cell_counts;
        sf::Vector2i cell_countsI;
        std::size_t cell_count;

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