#ifndef METH_HEAD_SIMULATOR_HPP_INCLUDED
#define METH_HEAD_SIMULATOR_HPP_INCLUDED

#include "cell.hpp"
#include "display-variables.hpp"
#include "meth-head-enum.hpp"
#include "meth-head.hpp"
#include "random.hpp"

#include <vector>

namespace methhead
{
    enum class Visuals
    {
        Enabled,
        Disabled
    };

    class Simulator
    {
        struct Scores
        {
            std::size_t lazy = 0;
            std::size_t greedy = 0;
        };

    public:
        explicit Simulator(const Visuals visuals);

        void run();

    private:
        void spawnMethHead(const Motivation motive);

        void printResults(const Scores & scores);

        void handleEvents();
        void handleEvent(const sf::Event & event);

        void update(std::size_t & frameCount, sf::Clock & frameClock);
        void draw();

        bool printOncePerSecondConsoleStatus(
            const float elapsedTimeSec, const std::size_t frameCount);

        Scores calcScores() const;

    private:
        Visuals m_visuals;
        sf::RenderWindow m_window;
        Audio m_audio;
        Random m_random;
        DisplayVariables m_displayVars;
        BoardMap_t m_board;
        std::vector<IActorUPtr_t> m_actors;
        std::size_t m_maxTurnsPerSec;
    };

} // namespace methhead

#endif // METH_HEAD_SIMULATOR_HPP_INCLUDED
