// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "display-variables.hpp"

namespace methhead
{
    DisplayVariables::DisplayVariables(const sf::Vector2u & windowSize)
        : m_constants(windowSize)
    {}

    BoardMap_t DisplayVariables::makeBoard() const
    {
        BoardMap_t board;

        for (std::size_t c(0); c < m_constants.column_count; ++c)
        {
            for (std::size_t r(0); r < m_constants.row_count; ++r)
            {
                const sf::Vector2i boardPos(static_cast<int>(c), static_cast<int>(r));
                const sf::Vector2f windowPos(cellPosToWindowPos(boardPos));

                const Cell cell(
                    boardPos, windowPos, m_constants.cell_size, m_constants.line_thickness);

                board.insert(std::make_pair(cell.board_pos, cell));
            }
        }

        return board;
    }

    sf::Vector2f DisplayVariables::cellPosToWindowPos(const sf::Vector2i & cellPos) const
    {
        const sf::Vector2f boardPos(m_constants.board_bounds.left, m_constants.board_bounds.top);

        const sf::Vector2f gridTotalSize(
            (m_constants.cell_size.x * static_cast<float>(m_constants.column_count)),
            (m_constants.cell_size.y * static_cast<float>(m_constants.row_count)));

        const float centerOffsetHoriz((m_constants.board_bounds.width - gridTotalSize.x) * 0.5f);
        const float centerOffsetVert((m_constants.board_bounds.height - gridTotalSize.y) * 0.5f);
        const sf::Vector2f centerOffset(centerOffsetHoriz, centerOffsetVert);

        sf::Vector2f pos(
            (static_cast<float>(cellPos.x) * m_constants.cell_size.x),
            (static_cast<float>(cellPos.y) * m_constants.cell_size.y));

        pos += (boardPos + centerOffset);
        return pos;
    }

} // namespace methhead
