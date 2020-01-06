// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "display-constants.hpp"

#include "util.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

namespace boardgame
{
    DisplayConstants::DisplayConstants(const sf::Vector2u & windowSize)
        : inner_window_rect({ 0.0f, 0.0f }, sf::Vector2f(windowSize))
        , score_rect(0.0f, 0.0f, 0.0f, 0.0f)
        , board_rect(0.0f, 0.0f, 0.0f, 0.0f)
        , cell_size(0.0f, 0.0f)
        , cell_counts(20, 20)
        , cell_countsI(cell_counts)
        , cell_count(cell_counts.x * cell_counts.y)
        , cell_line_thickness(2.0f)
        , cell_line_color(220, 220, 220)
        , cell_background_color(32, 32, 32)
        , wall_color(sf::Color(220, 220, 220))
    {
        assert((cell_counts.x > 0) && (cell_counts.x < windowSize.x));
        assert((cell_counts.y > 0) && (cell_counts.y < windowSize.y));

        // shrink the drawable window rect a bit to create nice looking border
        const float windowBorderRatio(0.975f);
        util::scaleRectInPlace(inner_window_rect, windowBorderRatio);
        const sf::Vector2f inner_window_pos(inner_window_rect.left, inner_window_rect.top);

        // figure out the cell size (must be square)
        const std::size_t maxCellsPerSideCount(std::max(cell_counts.x, cell_counts.y));

        const float minBoardRectSideLength(
            std::min(inner_window_rect.width, inner_window_rect.height));

        const float cellSideLength(
            minBoardRectSideLength / static_cast<float>(maxCellsPerSideCount));

        cell_size.x = cellSideLength;
        cell_size.y = cellSideLength;

        //
        // once we know how many cells there are and what size they are, we can split the screen
        // into the score and board rects
        const sf::Vector2f boardSize((cell_counts.x * cell_size.x), (cell_counts.y * cell_size.y));

        const sf::Vector2f boardPos(
            ((inner_window_rect.left + inner_window_rect.width) - boardSize.x),
            inner_window_rect.top);

        board_rect = sf::FloatRect(boardPos, boardSize);

        //
        const sf::Vector2f scoreSize(
            ((inner_window_rect.width - boardSize.x) - inner_window_pos.x),
            (boardSize.y - inner_window_rect.top));

        const sf::Vector2f scorePos(inner_window_pos);

        score_rect = sf::FloatRect(scorePos, scoreSize);
    }
} // namespace boardgame