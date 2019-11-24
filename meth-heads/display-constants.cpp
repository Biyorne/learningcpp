// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "display-constants.hpp"

#include "utils.hpp"

#include <cmath>
#include <iostream>

namespace methhead
{
    DisplayConstants::DisplayConstants(const sf::Vector2u& windowSize)
        : window_size(windowSize)
        , score_window_rect(0.0f, 0.0f, 0.0f, 0.0f)
        , board_window_rect(0.0f, 0.0f, 0.0f, 0.0f)
        , cell_size(0.0f, 0.0f)
        , horiz_cell_count(0)
        , vert_cell_count(0)
        , font()
        , default_text()
        , loot_texture()
        , cell_background_color(32, 32, 32)
        , cell_line_color(220, 220, 220)
        , lazy_color()
        , greedy_color()
    {
        // shrink the drawable window rect a bit to create nice looking border
        const float windowBorderRatio(0.975f);
        sf::FloatRect window_rect({ 0.0f, 0.0f }, window_size);
        scaleRectInPlace(window_rect, windowBorderRatio);

        // partition the window into a region for drawing scores on the left and board on the right
        score_window_rect = window_rect;
        score_window_rect.width *= 0.1f;

        board_window_rect = window_rect;

        board_window_rect.left
            = (score_window_rect.left + score_window_rect.width + score_window_rect.left);

        board_window_rect.width -= (score_window_rect.width + score_window_rect.left);

        // figure out the cell size (must be square)
        horiz_cell_count = 20;
        vert_cell_count = 20;
        const std::size_t maxCellsPerSideCount(std::max(horiz_cell_count, vert_cell_count));

        const float minBoardWindowSideLength(
            std::min(board_window_rect.width, board_window_rect.height));

        const float cellSideLength(
            minBoardWindowSideLength / static_cast<float>(maxCellsPerSideCount));

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

        // these colors should move after cleanup
        lazy_color = sf::Color(80, 80, 255);
        greedy_color = sf::Color(100, 255, 100);
    }

    sf::Vector2f DisplayConstants::boardPosToWindowPos(const BoardPos_t& boardPos) const
    {
        const sf::Vector2f topLeftBoardWindowPos(board_window_rect.left, board_window_rect.top);
        const sf::Vector2f boardPosF(boardPos);

        return (topLeftBoardWindowPos + (boardPosF * cell_size));
    };

    sf::FloatRect DisplayConstants::boardPosToWindowRect(const BoardPos_t& boardPos) const
    {
        return sf::FloatRect(boardPosToWindowPos(boardPos), cell_size);
    }
} // namespace methhead