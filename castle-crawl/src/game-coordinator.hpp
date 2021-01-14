#ifndef CASTLECRAWL_GAMECOORDINATOR_HPP_INCLUDED
#define CASTLECRAWL_GAMECOORDINATOR_HPP_INCLUDED
//
// game-coordinator.hpp
//
#include "animation-player.hpp"
#include "board.hpp"
#include "context.hpp"
#include "frames-per-sec.hpp"
#include "keys.hpp"
#include "map.hpp"
#include "player.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "settings.hpp"
#include "sound-player.hpp"
#include "tile-image.hpp"

#include <memory>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

namespace castlecrawl
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
        sf::RenderWindow m_window;
        FramesPerSecond m_fps;

        util::Random m_random;
        util::SoundPlayer m_soundPlayer;
        util::AnimationPlayer m_animationPlayer;

        Map m_map;
        Media m_media;
        Board m_board;
        Layout m_layout;
        GameInPlay m_game;
        GameConfig m_config;
        Context m_context;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_GAMECOORDINATOR_HPP_INCLUDED
