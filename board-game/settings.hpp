#ifndef BOARDGAME_SETTINGS_HPP_INCLUDED
#define BOARDGAME_SETTINGS_HPP_INCLUDED
//
// settings.hpp
//
#include "check-macros.hpp"
#include "types.hpp"
#include "util.hpp"

#include <filesystem>
#include <limits>
#include <set>
#include <string>

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
        virtual const std::set<BoardPos_t> & allValidPositions() const = 0;

        static inline const BoardPos_t not_a_pos{ std::numeric_limits<int>::lowest(),
                                                  std::numeric_limits<int>::lowest() };
    };

    //
    class SimpleLayout : public ILayout
    {
      public:
        SimpleLayout() = default;
        virtual ~SimpleLayout() = default;

        sf::Vector2f windowSize() const { return util::size(m_windowBounds); }
        sf::FloatRect windowBounds() const { return m_windowBounds; }
        sf::FloatRect boardBounds() const { return m_boardBounds; }
        sf::Vector2i cellCounts() const { return m_cellCounts; }
        std::size_t cellCountTotal() const { return m_cellCountTotal; }
        sf::Vector2f cellSize() const { return m_cellSize; }
        sf::FloatRect cellBounds(const BoardPos_t & pos) const;

        bool isPositionValid(const BoardPos_t & pos) const;
        const std::set<BoardPos_t> & allValidPositions() const { return m_allValidPositions; }

        void setup(const Map_t & map, const GameConfig & config);

      protected:
        sf::FloatRect m_windowBounds;
        sf::FloatRect m_boardBounds;
        sf::Vector2i m_cellCounts;
        std::size_t m_cellCountTotal{ 0 };
        sf::Vector2f m_cellSize;
        std::set<BoardPos_t> m_allValidPositions;
    };

    // All info about a game in progress that can changes during play.
    struct IGameInPlay
    {
        virtual ~IGameInPlay() = default;

        virtual int score() const = 0;
        virtual int scoreAdj(const int adj) = 0;

        virtual bool isGameOver() const = 0;
        virtual void isGameOver(const bool isOver) = 0;

        virtual bool didPlayerWin() const = 0;
        virtual void didPlayerWin(const bool didPlayerWin) = 0;
    };

    //
    class SimpleGameInPlay : public IGameInPlay
    {
      public:
        SimpleGameInPlay() = default;
        virtual ~SimpleGameInPlay() = default;

        void reset();

        int score() const override { return m_score; }
        int scoreAdj(const int adj) override;

        bool isGameOver() const override { return m_isGameOver; }
        void isGameOver(const bool isOver) override { m_isGameOver = isOver; }

        bool didPlayerWin() const override { return m_didPlayerWin; }
        void didPlayerWin(const bool didWin) override { m_didPlayerWin = didWin; }

      protected:
        int m_score{ 0 };
        bool m_isGameOver{ false };
        bool m_didPlayerWin{ false };
    };
} // namespace boardgame

#endif // BOARDGAME_SETTINGS_HPP_INCLUDED