#ifndef METHHEADS_DISPLAY_CONSTANTS_HPP_INCLUDED
#define METHHEADS_DISPLAY_CONSTANTS_HPP_INCLUDED

#include <SFML/Graphics.hpp>

namespace methhead
{
    using BoardPos_t = sf::Vector2i;
    using BoardMap_t = std::map<BoardPos_t, sf::RectangleShape>;

    struct DisplayConstants
    {
        explicit DisplayConstants(const sf::Vector2u & windowSize);

        sf::Vector2f boardToWindowPos(const BoardPos_t & boardPos) const;

        sf::FloatRect cellBounds(const BoardPos_t & boardPos) const;

        sf::Vector2f window_size;

        sf::FloatRect score_window_bounds;
        sf::FloatRect board_window_bounds;

        sf::Vector2f cell_size;
        std::size_t horiz_cell_count;
        std::size_t vert_cell_count;

        sf::Font font;
        sf::Text default_text;

        sf::Texture loot_texture;

        BoardMap_t board_map;

        sf::Color cell_background_color;
        sf::Color cell_line_color;

        std::pair<BoardPos_t, sf::FloatRect> off_screen_pair;

        // TODO these two probably don't belong here
        sf::Color lazy_color;
        sf::Color greedy_color;

    private:
        void populateBoardMap();
    };

} // namespace methhead

#endif // METHHEADS_DISPLAY_CONSTANTS_HPP_INCLUDED
