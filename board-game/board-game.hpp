#ifndef BOARDGAME_BOARDGAME_HPP_INCLUDED
#define BOARDGAME_BOARDGAME_HPP_INCLUDED

#include "animation-player.hpp"
#include "bloom-shader.hpp"
#include "collider.hpp"
#include "meth-head.hpp"
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
    class BoardGame
    {
      public:
        explicit BoardGame(const std::string & mediaDirPath);

        void run();

      private:
        void reset();

        void loadMap();

        void handleEvents();
        void handleEvent(const sf::Event & event);

        void update(const float elapsedSec);
        void draw();

      private:
        std::string m_mediaDirPath;
        bool m_enableSpecialEffects;

        float m_simTimeMult;
        sf::Clock m_frameClock;

        sf::VideoMode m_videoMode;
        sf::RenderWindow m_window;
        util::BloomEffectHelper m_bloomWindow;

        Resources m_resources;
        util::Random m_random;
        util::SoundPlayer m_soundPlayer;
        util::AnimationPlayer m_animationPlayer;
        DisplayConstants m_display;
        Collider m_collider;

        PieceUPtrVec_t m_pieces;

        Context m_context;
    };
} // namespace boardgame

#endif // BOARDGAME_BOARDGAME_HPP_INCLUDED