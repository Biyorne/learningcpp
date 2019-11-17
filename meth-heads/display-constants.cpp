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
        , score_window_bounds(0.0f, 0.0f, 0.0f, 0.0f)
        , board_window_bounds(0.0f, 0.0f, 0.0f, 0.0f)
        , cell_size(0.0f, 0.0f)
        , horiz_cell_count(0)
        , vert_cell_count(0)
        , font()
        , default_text()
        , loot_texture()
        , board_map()
        , cell_background_color(32, 32, 32)
        , cell_line_color(220, 220, 220)
        , off_screen_pair()
        , lazy_color()
        , greedy_color()
    {
        // shrink the drawable window bounds a bit to create nice looking border
        const float windowBorderRatio(0.975f);
        sf::FloatRect window_bounds({ 0.0f, 0.0f }, window_size);
        scaleRectInPlace(window_bounds, windowBorderRatio);

        // partition the window into a region for drawing scores on the left and board on the right
        score_window_bounds = window_bounds;
        score_window_bounds.width *= 0.1f;

        board_window_bounds = window_bounds;

        board_window_bounds.left
            = (score_window_bounds.left + score_window_bounds.width + score_window_bounds.left);

        board_window_bounds.width -= (score_window_bounds.width + score_window_bounds.left);

        // figure out the cell size (must be square)
        horiz_cell_count = 20;
        vert_cell_count = 20;
        const std::size_t maxCellCount(std::max(horiz_cell_count, vert_cell_count));

        const float minBoardWindowBounds(
            std::min(board_window_bounds.width, board_window_bounds.height));

        const float cellSideLength(minBoardWindowBounds / static_cast<float>(maxCellCount));

        cell_size.x = cellSideLength;
        cell_size.y = cellSideLength;

        // load the font and setup the default text options
        const std::string fontFilePath("font/gentium-plus.ttf");

        if (font.loadFromFile(fontFilePath))
        {
            default_text.setFont(font);
        }
        else
        {
            std::cout << "Failed to load font: \"" << fontFilePath << "\"" << std::endl;
        }

        // this is a handy trick to calc the right font size when working with sfml
        default_text.setCharacterSize(static_cast<unsigned int>(std::sqrt(window_size.x)));

        if (!loot_texture.loadFromFile("image/loot.png"))
        {
            std::cerr << "Error:  Unable to load loot image from: image/loot.png" << std::endl;
        }

        populateBoardMap();

        off_screen_pair.first = BoardPos_t((horiz_cell_count * 2), (vert_cell_count * 2));
        off_screen_pair.second = sf::FloatRect((window_size * 2.0f), cell_size);

        // these colors should move after cleanup
        lazy_color = sf::Color(80, 80, 255);
        greedy_color = sf::Color(100, 255, 100);
    }

    sf::Vector2f DisplayConstants::boardToWindowPos(const BoardPos_t & boardPos) const
    {
        const auto bounds(board_window_bounds);

        const sf::Vector2f windowPos(bounds.left, bounds.top);

        const sf::Vector2f gridTotalSize(
            (cell_size.x * static_cast<float>(horiz_cell_count)),
            (cell_size.y * static_cast<float>(vert_cell_count)));

        const float centerOffsetHoriz((bounds.width - gridTotalSize.x) * 0.5f);
        const float centerOffsetVert((bounds.height - gridTotalSize.y) * 0.5f);
        const sf::Vector2f centerOffset(centerOffsetHoriz, centerOffsetVert);

        sf::Vector2f pos(
            (static_cast<float>(boardPos.x) * cell_size.x),
            (static_cast<float>(boardPos.y) * cell_size.y));

        pos += (windowPos + centerOffset);
        return pos;
    }

    sf::FloatRect DisplayConstants::cellBounds(const BoardPos_t & boardPos) const
    {
        const auto foundIter(board_map.find(boardPos));

        if (foundIter == std::end(board_map))
        {
            return off_screen_pair.second;
        }

        return foundIter->second.getGlobalBounds();
    }

    void DisplayConstants::populateBoardMap()
    {
        const float outlineThickness(cell_size.x * 0.025f);

        for (std::size_t horiz(0); horiz < horiz_cell_count; ++horiz)
        {
            for (std::size_t vert(0); vert < vert_cell_count; ++vert)
            {
                const BoardPos_t boardPos(static_cast<int>(horiz), static_cast<int>(vert));
                const sf::Vector2f windowPos(boardToWindowPos(boardPos));

                sf::RectangleShape rectangle;
                rectangle.setPosition(windowPos);
                rectangle.setSize(cell_size);
                rectangle.setOutlineThickness(outlineThickness);
                rectangle.setFillColor(cell_background_color);
                rectangle.setOutlineColor(cell_line_color);

                board_map.insert(std::make_pair(boardPos, rectangle));
            }
        }
    }

} // namespace methhead
