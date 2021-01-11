#ifndef BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED
#define BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED
//
// game-coordinator.hpp
//
#include "animation-player.hpp"
#include "bloom-shader.hpp"
#include "context.hpp"
#include "keys.hpp"
#include "map.hpp"
#include "player.hpp"
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

        void reset(const GameConfig & config, const MapChars_t & mapChars);
        void switchToMap(const Map & map);
        void run();

      protected:
        void openWindow();
        void handleEvents();
        void handleEvent(const sf::Event & event);
        bool handleExitEvent(const sf::Event & event);
        void update(const float elapsedTimeSec);
        void draw();
        void printFinalStatusToConsole();

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
        Player m_player;
        Context m_context;
    };

} // namespace boardgame

#endif // BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED
