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
#include "map.hpp"
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
    class GameCoordinator
    {
      public:
        GameCoordinator();
        virtual ~GameCoordinator() = default;

        virtual void reset(const GameConfig & config, const MapChars_t & mapChars);
        virtual void switchToMap(const Map & map);
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
        Map m_map;
        GameConfig m_config;
        sf::RenderWindow m_window;

        util::Random m_random;
        util::SoundPlayer m_soundPlayer;
        util::AnimationPlayer m_animationPlayer;

        Layout m_layout;
        Media m_media;
        GameInPlay m_game;
        Context m_context;
    };

} // namespace boardgame

#endif // BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED
