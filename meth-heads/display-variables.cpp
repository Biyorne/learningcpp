// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "display-variables.hpp"

#include "utils.hpp"

namespace methhead
{
    DisplayVariables::DisplayVariables(const sf::Vector2u & windowSize)
        : m_constants(windowSize)
    {}

    BoardMap_t DisplayVariables::makeBoard() const
    {
        BoardMap_t board;

        for (std::size_t horiz(0); horiz < m_constants.horiz_cell_count; ++horiz)
        {
            for (std::size_t vert(0); vert < m_constants.vert_cell_count; ++vert)
            {
                const sf::Vector2i boardPos(static_cast<int>(horiz), static_cast<int>(vert));
                const sf::Vector2f windowPos(cellPosToWindowPos(boardPos));

                const Cell cell(boardPos, windowPos, m_constants.cell_size);

                board.insert(std::make_pair(cell.board_pos, cell));
            }
        }

        return board;
    }

    void DisplayVariables::scoreBarSetup(
        std::size_t lazyScore,
        std::size_t greedyScore,
        sf::RectangleShape & lazyRectangle,
        sf::RectangleShape & greedyRectangle)
    {
        const sf::FloatRect bounds(m_constants.score_window_bounds);

        // start with both at full height but not quite full width
        const sf::Vector2f barSizeFull((bounds.width * 0.475f), bounds.height);

        sf::Vector2f lazySize(barSizeFull);
        sf::Vector2f greedySize(barSizeFull);

        // shrink the height of the loser
        if (lazyScore != greedyScore)
        {
            const auto highScore(std::max(lazyScore, greedyScore));
            const auto lowScore(std::min(lazyScore, greedyScore));

            const float heightShrinkRatio(
                static_cast<float>(lowScore) / static_cast<float>(highScore));

            if (lazyScore < greedyScore)
            {
                lazySize.y *= heightShrinkRatio;
            }
            else
            {
                greedySize.y *= heightShrinkRatio;
            }
        }

        lazyRectangle.setPosition(bounds.left, ((bounds.top + bounds.height) - lazySize.y));
        lazyRectangle.setSize(lazySize);

        greedyRectangle.setPosition(
            ((bounds.left + bounds.width) - greedySize.x),
            ((bounds.top + bounds.height) - greedySize.y));

        greedyRectangle.setSize(greedySize);
    }

    sf::Vector2f DisplayVariables::cellPosToWindowPos(const sf::Vector2i & cellPos) const
    {
        const auto bounds(m_constants.board_window_bounds);

        const sf::Vector2f boardPos(bounds.left, bounds.top);

        const sf::Vector2f gridTotalSize(
            (m_constants.cell_size.x * static_cast<float>(m_constants.horiz_cell_count)),
            (m_constants.cell_size.y * static_cast<float>(m_constants.vert_cell_count)));

        const float centerOffsetHoriz((bounds.width - gridTotalSize.x) * 0.5f);
        const float centerOffsetVert((bounds.height - gridTotalSize.y) * 0.5f);
        const sf::Vector2f centerOffset(centerOffsetHoriz, centerOffsetVert);

        sf::Vector2f pos(
            (static_cast<float>(cellPos.x) * m_constants.cell_size.x),
            (static_cast<float>(cellPos.y) * m_constants.cell_size.y));

        pos += (boardPos + centerOffset);
        return pos;
    }

} // namespace methhead
