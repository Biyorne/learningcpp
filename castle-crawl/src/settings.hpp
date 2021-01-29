#ifndef CASTLECRAWL_SETTINGS_HPP_INCLUDED
#define CASTLECRAWL_SETTINGS_HPP_INCLUDED
//
// settings.hpp
//
#include "context.hpp"
#include "map-types.hpp"
#include "tile-image.hpp"
#include "util.hpp"

#include <string>
#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/VideoMode.hpp>

namespace castlecrawl
{
    class Map;

    // All settings about the game that must be set before...well before everything else.
    // No need for an interface class since this will be const in Context.
    struct GameConfig
    {
        GameConfig();

        std::string game_name;
        std::filesystem::path media_dir_path;
        sf::VideoMode video_mode;
        bool is_fullscreen;
        unsigned frame_rate_limit;
        sf::Color background_color;
        float map_cell_size_ratio;
    };

    // Everything about the window that can only be calculated once BOTH the window is open and the
    // map has been parsed.
    class Layout
    {
      public:
        Layout() = default;

        // always call this setup first
        void setupWindow(const GameConfig & config);

        //...and this setup on each map load
        void setupBoard(const sf::Vector2i & mapSize);

        float mapCellDimm() const { return m_cellSize.x; }

        sf::Vector2f mapCellSize() const { return m_cellSize; }

        sf::Vector2f windowSize() const { return util::size(m_windowBounds); }
        sf::FloatRect windowBounds() const { return m_windowBounds; }
        sf::FloatRect boardBounds() const { return m_boardBounds; }
        sf::Vector2i cellCounts() const { return m_cellCounts; }
        std::size_t cellCountTotal() const { return m_cellCountTotal; }
        sf::Vector2f cellSize() const { return m_cellSize; }
        sf::FloatRect cellBounds(const MapPos_t & pos) const;

        bool isPositionValid(const MapPos_t & pos) const;

        // BoardPosOpt_t windowPosToBoardPos(const sf::Vector2f windowPos) const ;

      protected:
        sf::FloatRect m_windowBounds;
        sf::FloatRect m_boardBounds;
        sf::Vector2i m_cellCounts;
        std::size_t m_cellCountTotal{ 0 };
        sf::Vector2f m_cellSize;
    };

    // All info about a game in progress that can changes during play.
    class GameInPlay
    {
      public:
        GameInPlay() = default;

        void reset();

        int score() const { return m_score; }
        void score(const int newScore) { m_score = newScore; }
        int scoreAdj(const int adj);

        bool isGameOver() const { return m_isGameOver; }

        void endGame(const bool didPlayerWin)
        {
            m_isGameOver = true;
            m_didPlayerWin = didPlayerWin;
        }

        bool didPlayerWin() const
        {
            if (!m_isGameOver)
            {
                return false;
            }

            return m_didPlayerWin;
        }

      protected:
        int m_score{ 0 };
        bool m_isGameOver{ false };
        bool m_didPlayerWin{ false };
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_SETTINGS_HPP_INCLUDED
