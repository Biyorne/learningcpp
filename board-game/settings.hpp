#ifndef BOARDGAME_SETTINGS_HPP_INCLUDED
#define BOARDGAME_SETTINGS_HPP_INCLUDED
//
// settings.hpp
//
#include "types.hpp"
#include "util.hpp"

#include <filesystem>
#include <string>

#include <SFML/Graphics.hpp>

namespace boardgame
{
    using BoardPos_t = sf::Vector2i;

    //

    struct IGameSettings
    {
        virtual ~IGameSettings() = default;

        virtual std::string name() const = 0;
        virtual int score() const = 0;
        virtual int scoreAdj(const int adj) = 0;

        virtual bool isGameOver() const = 0;
        virtual void isGameOver(const bool isOver) = 0;

        virtual bool isGamePaused() const = 0;
        virtual void isGamePaused(const bool isPaused) = 0;

        virtual std::filesystem::path mediaDirPath() const = 0;

        virtual sf::VideoMode videoMode() const = 0;
        virtual unsigned int windowStyle() const = 0;
        virtual unsigned int frameRateLimit() const = 0;
        virtual sf::Vector2f windowSize() const = 0;
        virtual sf::FloatRect windowBounds() const = 0;

        virtual sf::Vector2i cellCounts() const = 0;
    };

    //

    class GameSettingsBase : public IGameSettings
    {
      public:
        explicit GameSettingsBase(
            const std::string & gameName,
            const sf::Vector2i & cellCounts,
            const std::filesystem::path & mediaDirPath,
            const sf::VideoMode & videoMode = sf::VideoMode::getDesktopMode(),
            const unsigned style = sf::Style::Default,
            const unsigned frameRateLimit = 60)
            : m_name(gameName)
            , m_score(0)
            , m_isGameOver(false)
            , m_isGamePaused(false)
            , m_mediaDirPath(mediaDirPath)
            , m_cellCounts(cellCounts)
            , m_videoMode(videoMode)
            , m_windowStyle(style)
            , m_frameRateLimit(frameRateLimit)

        {}

        virtual ~GameSettingsBase() = default;

        std::string name() const override { return m_name; }
        std::filesystem::path mediaDirPath() const override { return m_mediaDirPath; }

        sf::VideoMode videoMode() const override { return m_videoMode; }

        unsigned int windowStyle() const override { return m_windowStyle; }
        unsigned int frameRateLimit() const override { return m_frameRateLimit; }

        bool isGameOver() const override { return m_isGameOver; }
        void isGameOver(const bool isOver) override { m_isGameOver = isOver; }

        bool isGamePaused() const override { return m_isGamePaused; }
        void isGamePaused(const bool isPaused) override { m_isGamePaused = isPaused; }

        sf::Vector2f windowSize() const override
        {
            return sf::Vector2f{ sf::Vector2u(m_videoMode.width, m_videoMode.height) };
        }

        sf::FloatRect windowBounds() const override { return { { 0.0f, 0.0f }, windowSize() }; }

        int score() const override { return m_score; }

        int scoreAdj(const int adj) override
        {
            m_score += adj;

            if (m_score < 0)
            {
                m_score = 0;
            }

            return m_score;
        }

        sf::Vector2i cellCounts() const override { return m_cellCounts; }

      protected:
        std::string m_name;

        int m_score;
        bool m_isGameOver;
        bool m_isGamePaused;

        std::filesystem::path m_mediaDirPath;

        sf::Vector2i m_cellCounts;
        sf::VideoMode m_videoMode;
        unsigned int m_windowStyle;
        unsigned int m_frameRateLimit; // zero will disable the limit
    };
} // namespace boardgame

#endif // BOARDGAME_SETTINGS_HPP_INCLUDED