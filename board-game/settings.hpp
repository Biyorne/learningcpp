#ifndef BOARDGAME_SETTINGS_HPP_INCLUDED
#define BOARDGAME_SETTINGS_HPP_INCLUDED
//
// settings.hpp
//
#include "board.hpp"
#include "check-macros.hpp"
#include "context.hpp"
#include "types.hpp"
#include "util.hpp"

#include <filesystem>
#include <limits>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

namespace boardgame
{
    using BoardPos_t = sf::Vector2i;

    // All settings about the game that must be set before...well before everything else.
    // No need for an interface class since this will be const in Context.
    struct GameConfig
    {
        std::string game_name;
        std::filesystem::path media_dir_path{ std::filesystem::current_path() / "media" };
        sf::VideoMode video_mode{ sf::VideoMode::getDesktopMode() };
        bool is_fullscreen{ true };
        unsigned frame_rate_limit{ 60 };
        sf::Color background_color{ sf::Color::Black };
        float between_cells_pad_ratio{ 0.975f };

        template <typename T>
        sf::Vector2<T> windowSize() const
        {
            return sf::Vector2<T>{ static_cast<T>(video_mode.width),
                                   static_cast<T>(video_mode.height) };
        }
    };

    // Boardgame boards are so small that their contents are simply rows of strings, where different
    // chars stand for different map contents.  i.e. walls/pickups/etc.
    // Each string MUST be the same length.
    using Map_t = std::vector<std::string>;

    // Everything about the window that can only be calculated once BOTH the window is open and the
    // map has been parsed.
    struct ILayout
    {
        virtual ~ILayout() = default;

        virtual sf::Vector2f windowSize() const = 0;
        virtual sf::FloatRect windowBounds() const = 0;
        virtual sf::FloatRect boardBounds() const = 0;
        virtual sf::Vector2i cellCounts() const = 0;
        virtual std::size_t cellCountTotal() const = 0;
        virtual sf::Vector2f cellSize() const = 0;
        virtual sf::FloatRect cellBounds(const BoardPos_t & pos) const = 0;

        virtual bool isPositionValid(const BoardPos_t & pos) const = 0;
        virtual const std::vector<BoardPos_t> & allValidPositions() const = 0;
        virtual BoardPosOpt_t windowPosToBoardPos(const sf::Vector2f windowPos) const = 0;
    };

    //
    class SimpleLayout : public ILayout
    {
      public:
        SimpleLayout() = default;
        virtual ~SimpleLayout() = default;

        void setup(const Map_t & map, const GameConfig & config);

        sf::Vector2f windowSize() const override { return util::size(m_windowBounds); }
        sf::FloatRect windowBounds() const override { return m_windowBounds; }
        sf::FloatRect boardBounds() const override { return m_boardBounds; }
        sf::Vector2i cellCounts() const override { return m_cellCounts; }
        std::size_t cellCountTotal() const override { return m_cellCountTotal; }
        sf::Vector2f cellSize() const override { return m_cellSize; }
        sf::FloatRect cellBounds(const BoardPos_t & pos) const override;

        bool isPositionValid(const BoardPos_t & pos) const override;

        const std::vector<BoardPos_t> & allValidPositions() const override
        {
            return m_allValidPositions;
        }

        BoardPosOpt_t windowPosToBoardPos(const sf::Vector2f windowPos) const override;

      protected:
        sf::FloatRect m_windowBounds;
        sf::FloatRect m_boardBounds;
        sf::Vector2i m_cellCounts;
        std::size_t m_cellCountTotal{ 0 };
        sf::Vector2f m_cellSize;
        std::vector<BoardPos_t> m_allValidPositions;
    };

    // All info about a game in progress that can changes during play.
    struct IGameInPlay
    {
        virtual ~IGameInPlay() = default;

        virtual void reset() = 0;

        virtual int score() const = 0;
        virtual void score(const int newScore) = 0;
        virtual int scoreAdj(const int adj) = 0;

        virtual bool isGameOver() const = 0;
        virtual void endGame(const bool didPlayerWin) = 0;
        virtual bool didPlayerWin() const = 0;
    };

    //
    class SimpleGameInPlay : public IGameInPlay
    {
      public:
        SimpleGameInPlay() = default;
        virtual ~SimpleGameInPlay() = default;

        void reset() override;

        int score() const override { return m_score; }
        void score(const int newScore) override { m_score = newScore; }
        int scoreAdj(const int adj) override;

        bool isGameOver() const override { return m_isGameOver; }

        void endGame(const bool didPlayerWin) override
        {
            m_isGameOver = true;
            m_didPlayerWin = didPlayerWin;
        }

        bool didPlayerWin() const override
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