#ifndef CASTLECRAWL_GAMECOORDINATOR_HPP_INCLUDED
#define CASTLECRAWL_GAMECOORDINATOR_HPP_INCLUDED
//
// game-coordinator.hpp
//
#include "animation-player.hpp"
#include "bloom-shader.hpp"
#include "board.hpp"
#include "context.hpp"
#include "keys.hpp"
#include "map.hpp"
#include "player.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "settings.hpp"
#include "sound-player.hpp"
#include "tile-image.hpp"
#include "util.hpp"

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
        void handleFps();

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
        Board m_board;
        Context m_context;

        sf::Clock m_fpsClock;
        float m_fpsFrameCounter;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_GAMECOORDINATOR_HPP_INCLUDED
