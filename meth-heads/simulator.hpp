#ifndef METHHEADS_SIMULATOR_HPP_INCLUDED
#define METHHEADS_SIMULATOR_HPP_INCLUDED

#include "animation-player.hpp"
#include "display-variables.hpp"
#include "meth-head.hpp"
#include "random.hpp"
#include "settings.hpp"
#include "sound-player.hpp"
#include "utils.hpp"

#include <fstream>
#include <optional>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace methhead
{
    class Simulator
    {
        struct Scores
        {
            int lazy = 0;
            int greedy = 0;
        };

      public:
        explicit Simulator(const Mode mode);

        void run();

      private:
        void spawnInitialPieces();
        float getSimFrameTimeElapsed();
        void printStatus();

        std::optional<BoardPos_t> findRandomFreeBoardPos() const;

        void spawnMethHead(const Motivation motive);
        void spawnLoot();

        void handleEvents();
        void handleEvent(const sf::Event & event);

        void update(const float elapsedSec);
        void draw();

        void handleActorPickingUpSometing(IActor & actor);

        Scores calcScores() const;

      private:
        bool m_isModeNormal;
        bool m_willStop;

        sf::VideoMode m_videoMode;
        sf::RenderWindow m_window;

        Random m_random;
        Settings m_settings;
        SoundPlayer m_soundPlayer;
        AnimationPlayer m_animationPlayer;
        DisplayVariables m_displayVars;

        std::vector<IActorUPtr_t> m_actors;
        std::vector<IPickupUPtr_t> m_pickups;

        sf::Clock m_frameClock;
        std::size_t m_framesPerSecondMax;

        sf::Clock m_statusClock;
        std::size_t m_framesSinceStatusCount;
        float m_statusIntervalSec;
        std::size_t m_statusCount;

        SimContext m_context;

        std::ofstream m_statsFile;
    };
} // namespace methhead

#endif // METHHEADS_SIMULATOR_HPP_INCLUDED