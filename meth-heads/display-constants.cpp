// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "display-constants.hpp"

#include "utils.hpp"

#include <cmath>
#include <iostream>

namespace methhead
{
    DisplayConstants::DisplayConstants(const sf::Vector2u & windowSize)
        : pad_ratio(0.1f)
        , window_size(windowSize)
        , score_bounds(0.0f, 0.0f, (0.2f * window_size.x), window_size.y)
        , board_bounds(
              (score_bounds.left + score_bounds.width),
              score_bounds.top,
              (window_size.x - score_bounds.width),
              score_bounds.height)
        , cell_background_color { 32, 32, 32 }
        , cell_line_color { 220, 220, 220 }
        , lazy_color { 80, 80, 255 }
        , greedy_color { 100, 255, 100 }
        , prize_color { 255, 255, 100 }
        , column_count { 20 }
        , row_count { column_count }
        , cellCountU(static_cast<unsigned int>(column_count), static_cast<unsigned int>(row_count))
        , cellCountI(cellCountU)
        , line_thickness_ratio(1.0f / 600.0f)
        , line_thickness(line_thickness_ratio * window_size.y)
        , cell_dimm(window_size.y / static_cast<float>(std::max(column_count, row_count)))
        , cell_size(cell_dimm, cell_dimm)
        , score_rectangle_width(score_bounds.width * 0.5f)
        , font()
        , default_text()
    {
        const auto FONT_FILE_PATH("font/gentium-plus.ttf");

        if (!font.loadFromFile(FONT_FILE_PATH))
        {
            std::cout << "Failed to load font: \"" << FONT_FILE_PATH << "\"" << std::endl;
        }

        default_text.setCharacterSize(static_cast<unsigned int>(std::sqrt(window_size.x)));
        default_text.setFont(font);
    }

    const sf::Vector2f DisplayConstants::cellToWindowPos(const sf::Vector2i & cellPos) const
    {
        const sf::Vector2f boardPos(board_bounds.left, board_bounds.top);

        const sf::Vector2f gridTotalSize(
            (cell_size.x * static_cast<float>(column_count)),
            (cell_size.y * static_cast<float>(row_count)));

        const float centerOffsetHoriz((board_bounds.width - gridTotalSize.x) * 0.5f);
        const float centerOffsetVert((board_bounds.height - gridTotalSize.y) * 0.5f);
        const sf::Vector2f centerOffset(centerOffsetHoriz, centerOffsetVert);

        sf::Vector2f pos(
            (static_cast<float>(cellPos.x) * cell_size.x),
            (static_cast<float>(cellPos.y) * cell_size.y));

        pos += (boardPos + centerOffset);
        return pos;
    }

    const sf::RectangleShape DisplayConstants::makeGridRectangleShape(
        const sf::Vector2f & pos, const sf::Vector2f & size) const
    {
        sf::RectangleShape temporary(size);
        temporary.setFillColor(cell_background_color);
        temporary.setOutlineColor(cell_line_color);
        temporary.setOutlineThickness(line_thickness);
        temporary.setPosition(pos);
        return temporary;
    }

    BoardMap_t DisplayConstants::makeBoard() const
    {
        BoardMap_t board;

        for (std::size_t c(0); c < column_count; ++c)
        {
            for (std::size_t r(0); r < row_count; ++r)
            {
                const sf::Vector2i boardPos(static_cast<int>(c), static_cast<int>(r));
                const sf::Vector2f windowPos(cellToWindowPos(boardPos));
                const sf::RectangleShape rectangle(makeGridRectangleShape(windowPos, cell_size));
                const Cell cell(boardPos, rectangle);

                board.insert(std::make_pair(cell.board_pos, cell));
            }
        }

        return board;
    }

} // namespace methhead
