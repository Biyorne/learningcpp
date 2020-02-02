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

        void setupBoardBackgroundImage();

        void loadMap();

        void handleEvents();
        void handleEvent(const sf::Event & event);

        void update(const float elapsedTimeSec);
        void draw();

        float randomPickupSpawnDelaySec() const { return m_context.random.fromTo(4.0f, 8.0f); }

        void updatePickupSpawn(const float elapsedTimeSec);

      private:
        bool m_enableSpecialEffects;
        Resources m_images;

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

        sf::Sprite m_boardBaseSprite;
        sf::RenderTexture m_boardBaseTexture;

        bool m_isPickupSpawnEffectRunning;
        float m_victimSpawnWaitDurSec;
        float m_victimSpawnWaitSoFarSec;
        float m_victimSpawnScaler;
        BoardPos_t m_victimSpawnBoardPos;
        sf::Sprite m_victimSpawnSprite;
    };
} // namespace boardgame

#endif // BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED