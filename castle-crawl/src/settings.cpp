// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// settings.cpp
//
#include "settings.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "map.hpp"

#include <filesystem>

namespace castlecrawl
{
    GameConfig::GameConfig()
        : game_name{ "game" }
        , media_dir_path{ std::filesystem::current_path() / "media" }
        , video_mode{ sf::VideoMode::getDesktopMode() }
        , is_fullscreen{ true }
        , frame_rate_limit{ 0 }
        , background_color{ sf::Color(17, 14, 14) }
        , map_cell_size_ratio{ 0.024f }
    {}

    void Layout::setupWindow(const GameConfig & config)
    {
        const sf::Vector2f windowSize{ sf::Vector2u{ config.video_mode.width,
                                                     config.video_mode.height } };

        m_windowBounds = sf::FloatRect({ 0.0f, 0.0f }, windowSize);

        const float cellDimm = std::floor(config.map_cell_size_ratio * windowSize.x);

        m_cellSize.x = cellDimm;
        m_cellSize.y = cellDimm;
    }

    void Layout::setupBoard(const sf::Vector2i & mapSize)
    {
        m_cellCounts = mapSize;

        m_cellCountTotal = static_cast<std::size_t>(m_cellCounts.x * m_cellCounts.y);

        const sf::Vector2f actualBoardSize{ sf::Vector2i(m_cellSize) * m_cellCounts };

        const sf::Vector2f actualBoardPos{ util::center(m_windowBounds) -
                                           (actualBoardSize / 2.0f) };

        m_boardBounds = sf::FloatRect(actualBoardPos, actualBoardSize);
    }

    bool Layout::isPositionValid(const MapPos_t & pos) const
    {
        return (
            (pos.x >= 0) && (pos.x < m_cellCounts.x) && (pos.y >= 0) && (pos.y < m_cellCounts.y));
    }

    sf::FloatRect Layout::cellBounds(const MapPos_t & pos) const
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
} // namespace castlecrawl
