// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// settings.cpp
//
#include "settings.hpp"

#include "context.hpp"

namespace boardgame
{
    void SimpleLayout::setup(const Map_t & map, const GameConfig & config)
    {
        const sf::Vector2f windowSize{ config.windowSize<float>() };
        m_windowBounds = sf::FloatRect({ 0.0f, 0.0f }, windowSize);

        m_boardBounds = m_windowBounds;

        m_cellCounts =
            sf::Vector2i(static_cast<int>(map.front().size()), static_cast<int>(map.size()));

        M_CHECK_SS(
            ((m_cellCounts.x > 0) && (m_cellCounts.y > 0)),
            "m_windowBounds=" << m_windowBounds << ", m_cellCounts=" << m_cellCounts);

        m_cellCountTotal = static_cast<std::size_t>(m_cellCounts.x * m_cellCounts.y);

        m_cellSize = (util::size(m_windowBounds) / sf::Vector2f(m_cellCounts));

        M_CHECK_SS(
            (!(m_cellSize.x < 1.0f) && !(m_cellSize.y < 1.0f)),
            "m_windowBounds=" << m_windowBounds << ", m_cellCounts=" << m_cellCounts
                              << ", m_cellSize=" << m_cellSize);

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

    bool SimpleLayout::isPositionValid(const BoardPos_t & pos) const
    {
        return (
            (pos.x >= 0) && (pos.x < m_cellCounts.x) && (pos.y >= 0) && (pos.y < m_cellCounts.y));
    }

    sf::FloatRect SimpleLayout::cellBounds(const BoardPos_t & pos) const
    {
        if (!isPositionValid(pos))
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }

        const sf::FloatRect bounds{
            (util::position(m_boardBounds) + (m_cellSize * sf::Vector2f(pos))), m_cellSize
        };

        return bounds;
    }

    //

    void SimpleGameInPlay::reset()
    {
        m_score = 0;
        m_isGameOver = false;
        m_didPlayerWin = false;
    }

    int SimpleGameInPlay::scoreAdj(const int adj)
    {
        m_score += adj;

        if (m_score < 0)
        {
            m_score = 0;
        }

        return m_score;
    }
} // namespace boardgame