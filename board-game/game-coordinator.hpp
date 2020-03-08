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

    class LightsOutGame : public SimpleGameCoordinator
    {
      public:
        LightsOutGame() = default;
        virtual ~LightsOutGame() = default;

        void reset(const GameConfig & configOld, const Map_t & mapOrig) override;
        void switchToMap(const Map_t & map) override;
        void draw() override;

        static Map_t makeMapOfSize(std::size_t size);

      private:
        void printFinalStatusToConsole() override;
        void handleEvent(const sf::Event & event) override;
        bool handleBoardResizeMapEvent(const sf::Event & event);
        void toggleCell(const BoardPos_t & pos);
        void randomizeCells();
        std::vector<BoardPos_t> findAllBoardPosToToggle(const BoardPos_t & clickedBoardPos) const;
    };
} // namespace boardgame

#endif // BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED