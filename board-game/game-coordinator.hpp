#ifndef BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED
#define BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED

#include "animation-player.hpp"
#include "bloom-shader.hpp"
#include "board.hpp"
#include "pieces.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "settings.hpp"
#include "sound-player.hpp"
#include "types.hpp"
#include "util.hpp"

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace boardgame
{
    class GameCoordinator
    {
      public:
        explicit GameCoordinator(const GameSettingsBase & settings);
        virtual ~GameCoordinator();

        void run();

      private:
        void handleEvents();
        void handleEvent(const sf::Event & event);
        void update(const float elapsedTimeSec);
        void draw();

      private:
        GameSettingsBase m_settings;
        sf::RenderWindow m_window;
        SnakeBoard m_board;
        util::Random m_random;
        ResourcesBase m_resources;
        util::SoundPlayer m_soundPlayer;
        util::AnimationPlayer m_animationPlayer;
        Context m_context;
    };
} // namespace boardgame

#endif // BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED