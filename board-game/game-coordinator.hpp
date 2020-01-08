#ifndef BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED
#define BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED

#include "animation-player.hpp"
#include "bloom-shader.hpp"
#include "board.hpp"
#include "collider.hpp"
#include "pieces.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "sound-player.hpp"
#include "types.hpp"
#include "util.hpp"

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace boardgame
{
    class GameCoordinator
    {
      public:
        explicit GameCoordinator(const std::string & mediaDirPath);

        void run();

      private:
        void reset();

        void loadMap();

        void handleEvents();
        void handleEvent(const sf::Event & event);

        void update(const float elapsedTimeSec);
        void draw();

        template <typename T>
        void update(const float elapsedTimeSec, T & piece)
        {
            const BoardPos_t origPos{ piece.boardPos() };
            const BoardPos_t newPos{ piece.update(m_context, elapsedTimeSec) };

            // skip if the piece does not want to move
            if (newPos == origPos)
            {
                return;
            }

            m_collider.collide(m_context, piece, newPos);
        }

      private:
        bool m_enableSpecialEffects;
        Resources m_resources;

        float m_simTimeMult;
        sf::Clock m_frameClock;

        sf::VideoMode m_videoMode;
        sf::RenderWindow m_window;
        util::BloomEffectHelper m_bloomWindow;

        util::Random m_random;
        util::SoundPlayer m_soundPlayer;
        util::AnimationPlayer m_animationPlayer;
        Board m_board;
        Collider m_collider;

        Context m_context;
    };
} // namespace boardgame

#endif // BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED