#include "display-constants.hpp"

#include "cell-position.hpp"

#include <cmath>
#include <iostream>

namespace methhead
{
    DisplayConstants::DisplayConstants(const sf::Vector2u & windowSize)
        : pad_ratio(0.1f)
        , window_size(windowSize)
        , score_region(0.0f, 0.0f, (0.2f * window_size.x), window_size.y)
        , board_region(
              (score_region.left + score_region.width),
              score_region.top,
              (window_size.x - score_region.width),
              score_region.height)
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
        , score_rectangle_width(score_region.width * 0.5f)
        , positions(makeCellPositions())
        , rectangles(makeGrid())
        , font()
        , default_text()
    {
        const auto FONT_FILE_PATH("font/gentium-plus.ttf");

        if (!font.loadFromFile(FONT_FILE_PATH))
        {
            std::cerr << "Failed to load font: \"" << FONT_FILE_PATH << "\"" << std::endl;
        }

        default_text.setCharacterSize(static_cast<unsigned int>(std::sqrt(window_size.x)));
        default_text.setFont(font);
    }

    const sf::Vector2f DisplayConstants::cellToScreenPos(const sf::Vector2i & cellPos)
    {
        const sf::Vector2f boardPos(board_region.left, board_region.top);

        const sf::Vector2f gridTotalSize(
            (cell_size.x * static_cast<float>(column_count)),
            (cell_size.y * static_cast<float>(row_count)));

        const float centerOffsetHoriz((board_region.width - gridTotalSize.x) * 0.5f);
        const float centerOffsetVert((board_region.height - gridTotalSize.y) * 0.5f);
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

    std::vector<CellPositions> DisplayConstants::makeCellPositions()
    {
        std::vector<CellPositions> cellPositions;
        for (std::size_t c(0); c < column_count; ++c)
        {
            for (std::size_t r(0); r < row_count; ++r)
            {
                const sf::Vector2i cellPos(static_cast<int>(c), static_cast<int>(r));
                const sf::Vector2f cellScreenPos(cellToScreenPos(cellPos));
                const CellPositions cellPosition(cellPos, cellScreenPos);
                cellPositions.push_back(cellPosition);
            }
        }
        return cellPositions;
    }

    std::vector<sf::RectangleShape> DisplayConstants::makeGrid()
    {
        std::vector<sf::RectangleShape> rectangleStack;

        for (const CellPositions & cellPositions : positions)
        {
            rectangleStack.push_back(makeGridRectangleShape((cellPositions.screen), cell_size));
        }

        return rectangleStack;
    }
} // namespace methhead
