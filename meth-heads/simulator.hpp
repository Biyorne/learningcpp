#ifndef METHHEADS_SIMULATOR_HPP_INCLUDED
#define METHHEADS_SIMULATOR_HPP_INCLUDED

#include "animation-player.hpp"
#include "display-variables.hpp"
#include "enums.hpp"
#include "meth-head.hpp"
#include "random.hpp"
#include "score.hpp"
#include "sound-player.hpp"
#include "utils.hpp"

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace methhead
{
    class Simulator
    {
      public:
        explicit Simulator(const Mode mode);

        void run();

      private:
        void reset();

        float getElapsedSimFrameTimeSec();

        void spawnMethHead(const Motivation motive, const std::size_t count = 1);
        void spawnLoot(const std::size_t count = 1);

        void killMethHead(const std::size_t count = 1);
        void killLoot(const std::size_t count = 1);

        void handleEvents();
        void handleEvent(const sf::Event & event);

        void update(const float elapsedSec);
        void draw();

        void handleActorPickingup(IActor & actor);

        void forceActorsToPickTargets();

      private:
        bool m_willStop;
        bool m_enableSpecialEffects;

        sf::VideoMode m_videoMode;
        sf::RenderWindow m_window;

        Score m_score;
        Random m_random;
        SoundPlayer m_soundPlayer;
        AnimationPlayer m_animationPlayer;
        DisplayVariables m_displayVars;

        std::vector<IActorUPtr_t> m_actors;
        std::vector<IPickupUPtr_t> m_pickups;

        SimContext m_context;

        sf::Clock m_frameClock;
        std::size_t m_framesPerSecondMax;
        float m_simTimeMult;
    };
} // namespace methhead

#endif // METHHEADS_SIMULATOR_HPP_INCLUDED