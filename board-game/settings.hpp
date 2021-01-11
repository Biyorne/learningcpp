#ifndef BOARDGAME_SETTINGS_HPP_INCLUDED
#define BOARDGAME_SETTINGS_HPP_INCLUDED
//
// settings.hpp
//
#include "check-macros.hpp"
#include "context.hpp"
#include "map.hpp"
#include "tile-image.hpp"
#include "util.hpp"

#include <filesystem>
#include <limits>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

namespace boardgame
{
    // All settings about the game that must be set before...well before everything else.
    // No need for an interface class since this will be const in Context.
    struct GameConfig
    {
        std::string game_name{ "game" };
        std::filesystem::path media_dir_path{ std::filesystem::current_path() / "media" };
        sf::VideoMode video_mode{ sf::VideoMode::getDesktopMode() };
        bool is_fullscreen{ true };
        unsigned frame_rate_limit{ 60 };
        sf::Color background_color{ sf::Color::Black };
        float between_cells_pad_ratio{ 0.975f };
        float map_cell_size_ratio{ 0.029f };

        template <typename T>
        sf::Vector2<T> windowSize() const
        {
            return sf::Vector2<T>{ static_cast<T>(video_mode.width),
                                   static_cast<T>(video_mode.height) };
        }

        float mapCellDimm() const { return (map_cell_size_ratio * windowSize<float>().x); }

        sf::Vector2f mapCellSize() const { return sf::Vector2f(mapCellDimm(), mapCellDimm()); }
    };

    // Everything about the window that can only be calculated once BOTH the window is open and the
    // map has been parsed.
    class Layout
    {
      public:
        Layout() = default;

        void setup(const Map & map, const GameConfig & config);

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

} // namespace boardgame

#endif // BOARDGAME_SETTINGS_HPP_INCLUDED
