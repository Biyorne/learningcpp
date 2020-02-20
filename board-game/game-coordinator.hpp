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
        explicit GameCoordinator(const SnakeGameSettings & settings);
        virtual ~GameCoordinator();

        void run();

      private:
        void reset();
        void handleEvents();
        void handleEvent(const sf::Event & event);
        void update(const float frameTimeSec);
        void draw();

        void updateScore();

      private:
        SnakeGameSettings m_settings;
        sf::RenderWindow m_window;
        SnakeBoard m_board;
        util::Random m_random;
        SnakeResources m_resources;
        util::SoundPlayer m_soundPlayer;
        util::AnimationPlayer m_animationPlayer;
        Context m_context;

        sf::Text m_scoreText;
    };
} // namespace boardgame

#endif // BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED