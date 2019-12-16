// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "display-constants.hpp"

#include "utils.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

namespace methhead
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
        , font()
        , default_text()
        , loot_texture()
        , lazy_texture()
        , greedy_texture()
        , lazy_color()
        , greedy_color()
    {
        assert((cell_counts.x > 0) && (cell_counts.x < windowSize.x));
        assert((cell_counts.y > 0) && (cell_counts.y < windowSize.y));

        // shrink the drawable window rect a bit to create nice looking border
        const float windowBorderRatio(0.975f);
        scaleRectInPlace(inner_window_rect, windowBorderRatio);
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

        //
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
        default_text.setCharacterSize(static_cast<unsigned int>(std::sqrt(windowSize.x)));

        //
        if (!loot_texture.loadFromFile("image/loot.png"))
        {
            std::cerr << "Error:  Unable to load loot image from: image/loot.png" << std::endl;
        }

        if (!lazy_texture.loadFromFile("image/head-1.png"))
        {
            std::cerr << "Unable to load lazy image: image/head-1.png" << std::endl;
        }

        if (!greedy_texture.loadFromFile("image/head-2.png"))
        {
            std::cerr << "Unable to load greedy image: image/head-2.png" << std::endl;
        }

        // these colors should move after cleanup
        lazy_color = sf::Color(80, 80, 255);
        greedy_color = sf::Color(100, 255, 100);
    }
} // namespace methhead