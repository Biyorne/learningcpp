#ifndef BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED
#define BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED
//
// game-coordinator.hpp
//
#include "animation-player.hpp"
#include "bloom-shader.hpp"
#include "board.hpp"
#include "context.hpp"
#include "keys.hpp"
#include "pieces.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "settings.hpp"
#include "sound-player.hpp"
#include "types.hpp"
#include "util.hpp"

#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace boardgame
{
    class SimpleGameCoordinator
    {
      public:
        SimpleGameCoordinator();
        virtual ~SimpleGameCoordinator() = default;

        virtual void reset(const GameConfig & config, const Map_t & map);
        virtual void switchToMap(const Map_t & map);
        virtual void run();

      protected:
        virtual void openWindow();
        virtual void handleEvents();
        virtual void handleEvent(const sf::Event & event);
        virtual bool handleExitEvents(const sf::Event & event);
        virtual void update(const float elapsedTimeSec);
        virtual void draw();
        virtual void printFinalStatusToConsole();

        void runFullCheck();

      protected:
        Map_t m_map;
        GameConfig m_config;
        sf::RenderWindow m_window;

        util::Random m_random;
        util::SoundPlayer m_soundPlayer;
        util::AnimationPlayer m_animationPlayer;

        SimpleLayout m_layout;
        SimpleMedia m_media;
        SimpleBoard m_board;
        SimpleGameInPlay m_game;
        Context m_context;
    };

    //

    class TestingFrenzyGame : public SimpleGameCoordinator
    {
      public:
        TestingFrenzyGame() = default;
        virtual ~TestingFrenzyGame() = default;

        void reset(const GameConfig & configOld, const Map_t & mapOrig) override;

        static Map_t makeMapOfSize(std::size_t horiz, std::size_t vert);
        void update(const float elapsedTimeSec) override;
        void draw() override;

      private:
        void handleEvent(const sf::Event & event) override;
        void restartWithNewMapSize();
        void switchToMap(const Map_t & map) override;

      private:
        sf::Clock m_mapResizeClock{};
        float m_mapResizeTimeRemainingSec{ 0.0f };
        std::size_t m_eaterCount{ 0 };
        std::size_t m_foodCount{ 0 };
        std::size_t m_obstacleCount{ 0 };
        float m_timeUntilPopulationChecks{ 0.0f };
        sf::Clock m_oneSecondClock{};
        float m_frameCount{ 0.0f };
    };
} // namespace boardgame

#endif // BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED