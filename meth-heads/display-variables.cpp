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
        // split the bounds into two halves for lazy vs greedy
        sf::FloatRect bounds(m_constants.score_window_bounds);

        // make the width just under half to put a space between that looks nice
        sf::FloatRect lazyBounds(bounds);
        lazyBounds.width *= 0.475f;

        sf::FloatRect greedyBounds(lazyBounds);
        greedyBounds.left = ((bounds.left + bounds.width) - greedyBounds.width);

        // shrink the height of the loser
        if (lazyScore != greedyScore)
        {
            const auto highScore(std::max(lazyScore, greedyScore));
            const auto lowScore(std::min(lazyScore, greedyScore));

            const float shrinkRatio(static_cast<float>(lowScore) / static_cast<float>(highScore));

            if (lazyScore < greedyScore)
            {
                lazyBounds.height *= shrinkRatio;
            }
            else
            {
                greedyBounds.height *= shrinkRatio;
            }
        }

        // position and size both rectangles
        lazyRectangle.setPosition(
            lazyBounds.left, ((bounds.top + bounds.height) - lazyBounds.height));

        lazyRectangle.setSize({ lazyBounds.width, lazyBounds.height });

        greedyRectangle.setPosition(
            greedyBounds.left, ((bounds.top + bounds.height) - greedyBounds.height));

        greedyRectangle.setSize({ greedyBounds.width, greedyBounds.height });
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
