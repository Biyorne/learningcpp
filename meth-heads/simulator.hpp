#ifndef METHHEADS_SIMULATOR_HPP_INCLUDED
#define METHHEADS_SIMULATOR_HPP_INCLUDED

#include "audio.hpp"
#include "cell.hpp"
#include "display-variables.hpp"
#include "meth-head.hpp"
#include "random.hpp"
#include "utils.hpp"

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace methhead
{
    class Simulator
    {
        struct Scores
        {
            std::size_t lazy = 0;
            std::size_t greedy = 0;
        };

    public:
        explicit Simulator(const Mode mode);

        void run();

    private:
        void updateSecondsPerTurn();
        bool willKeepRunning() const;

        void consoleStatus();
        void printConsoleStatus();

        void spawnMethHead(const Motivation motive);

        void handleEvents();
        void handleEvent(const sf::Event & event);

        void update();
        void draw();

        Scores calcScores() const;

    private:
        Mode m_mode;
        sf::VideoMode m_videoMode;

        sf::RenderWindow m_window;
        Random m_random;
        Audio m_audio;
        DisplayVariables m_displayVars;
        BoardMap_t m_board;

        std::size_t m_actorTurnIndex;
        std::vector<IActorUPtr_t> m_actors;

        float m_secondsPerTurn;
        float m_secondsPerTurnMultipler;
        std::size_t m_frameCount;

        sf::Clock m_consoleStatusClock;
        std::size_t m_consoleStatusFrameCount;
        std::size_t m_consoleStatusFrameCountMax;

        sf::Texture m_lootTexture;
    };

} // namespace methhead

#endif // METHHEADS_SIMULATOR_HPP_INCLUDED
