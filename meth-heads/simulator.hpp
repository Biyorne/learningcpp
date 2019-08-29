#ifndef METH_HEAD_SIMULATOR_HPP_INCLUDED
#define METH_HEAD_SIMULATOR_HPP_INCLUDED

#include "cell.hpp"
#include "display-constants.hpp"
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
    public:
        explicit Simulator(const unsigned int windowWidth, const unsigned int windowHeight);

        void run();

    private:
        void spawnMethHead(const Motivation motive);

        void printResults(const std::size_t lazyFinalScore, const std::size_t greedyFinalScore);

        void handleEvents();
        void handleEvent(const sf::Event & event);

        void update(std::size_t & frameCount, sf::Clock & frameClock);
        void draw();
        void printFinalResults();

    private:
        Visuals m_visuals;
        sf::RenderWindow m_window;
        Audio m_audio;
        Random m_random;
        const DisplayConstants m_displayConstants;
        BoardMap_t m_board;
        std::vector<IActorUPtr_t> m_actors;
        std::size_t m_maxIterationsPerSec;
    };

} // namespace methhead

#endif // METH_HEAD_SIMULATOR_HPP_INCLUDED
