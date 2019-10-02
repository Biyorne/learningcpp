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
        , horiz_cell_count(0)
        , vert_cell_count(0)
        , cell_size(0.0f, 0.0f)
        , font()
        , default_text()
    {
        // partition the window into a region for drawing scores on the left and board on the right
        score_window_bounds.width = (0.1f * window_size.x);
        score_window_bounds.height = window_size.y;

        board_window_bounds.left = score_window_bounds.width;
        board_window_bounds.width = (window_size.x - score_window_bounds.width);
        board_window_bounds.height = window_size.y;

        // shrink these regions a bit to give them a small pad preventing them from touching
        const float windowBoundsPad(window_size.x * 0.025f);
        resizeInPlace(score_window_bounds, -windowBoundsPad);
        resizeInPlace(board_window_bounds, -windowBoundsPad);
        score_window_bounds.width += windowBoundsPad;

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

        // these colors should move after cleanup
        lazy_color = sf::Color(80, 80, 255);
        greedy_color = sf::Color(100, 255, 100);
    }

} // namespace methhead
