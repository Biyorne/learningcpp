// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// settings.cpp
//
#include "settings.hpp"

#include "context.hpp"

namespace boardgame
{
    void Layout::setup(const Map & map, const GameConfig & config)
    {
        const sf::Vector2f windowSize{ config.windowSize<float>() };
        m_windowBounds = sf::FloatRect({ 0.0f, 0.0f }, windowSize);

        m_boardBounds = m_windowBounds;

        m_cellCounts = sf::Vector2i(
            static_cast<int>(map.m_mapChars.front().size()),
            static_cast<int>(map.m_mapChars.size()));

        M_CHECK_SS(
            ((m_cellCounts.x > 0) && (m_cellCounts.y > 0)),
            "m_windowBounds=" << m_windowBounds << ", m_cellCounts=" << m_cellCounts);

        m_cellCountTotal = static_cast<std::size_t>(m_cellCounts.x * m_cellCounts.y);

        m_cellSize = (util::size(m_windowBounds) / sf::Vector2f(m_cellCounts));

        M_CHECK_SS(
            (!(m_cellSize.x < 1.0f) && !(m_cellSize.y < 1.0f)),
            "m_windowBounds=" << m_windowBounds << ", m_cellCounts=" << m_cellCounts
                              << ", m_cellSize=" << m_cellSize);

        if (config.will_force_square_cells)
        {
            m_cellSize.x = std::floor(std::min(m_cellSize.x, m_cellSize.y));
            m_cellSize.y = m_cellSize.x;

            M_CHECK_SS(
                (!(m_cellSize.x < 1.0f) && !(m_cellSize.y < 1.0f)),
                "m_windowBounds=" << m_windowBounds << ", m_cellCounts=" << m_cellCounts
                                  << ", m_cellSize=" << m_cellSize);

            const sf::Vector2f actualBoardSize{ sf::Vector2i(m_cellSize) * m_cellCounts };

            const sf::Vector2f actualBoardPos{ util::center(m_boardBounds) -
                                               (actualBoardSize / 2.0f) };

            m_boardBounds = sf::FloatRect(actualBoardPos, actualBoardSize);
        }

        m_allValidPositions.clear();

        for (int vert(0); vert < m_cellCounts.y; ++vert)
        {
            for (int horiz(0); horiz < m_cellCounts.x; ++horiz)
            {
                const BoardPos_t pos{ horiz, vert };
                M_CHECK_SS(isPositionValid(pos), pos);
                m_allValidPositions.push_back(pos);
            }
        }

        M_CHECK_SS(
            (m_allValidPositions.size() == m_cellCountTotal),
            "m_allValidPositions.size()=" << m_allValidPositions.size()
                                          << ", m_cellCountTotal=" << m_cellCountTotal);
    }

    bool Layout::isPositionValid(const BoardPos_t & pos) const
    {
        return (
            (pos.x >= 0) && (pos.x < m_cellCounts.x) && (pos.y >= 0) && (pos.y < m_cellCounts.y));
    }

    sf::FloatRect Layout::cellBounds(const BoardPos_t & pos) const
    {
        M_CHECK_SS(isPositionValid(pos), pos);

        const sf::FloatRect bounds{
            (util::position(m_boardBounds) + (m_cellSize * sf::Vector2f(pos))), m_cellSize
        };

        return bounds;
    }

    // BoardPosOpt_t Layout::windowPosToBoardPos(const sf::Vector2f windowPos) const
    //{
    //    for (const BoardPos_t & boardPos : m_allValidPositions)
    //    {
    //        if (cellBounds(boardPos).contains(windowPos))
    //        {
    //            return boardPos;
    //        }
    //    }
    //
    //    return std::nullopt;
    //}

    //

    void GameInPlay::reset()
    {
        m_score = 0;
        m_isGameOver = false;
        m_didPlayerWin = false;
    }

    int GameInPlay::scoreAdj(const int adj)
    {
        m_score += adj;

        if (m_score < 0)
        {
            m_score = 0;
        }

        return m_score;
    }
} // namespace boardgame
