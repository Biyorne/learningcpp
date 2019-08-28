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
    class Simulator
    {
    public:
        Simulator();

        void run();

    private:
        void spawnMethHead(const Motivation motive);

        void printResults(const std::size_t lazyFinalScore, const std::size_t greedyFinalScore);

    private:
        sf::RenderWindow m_window;
        Audio m_audio;
        Random m_random;
        const DisplayConstants m_displayConstants;
        BoardMap_t m_board;
        std::vector<IActorUPtr_t> m_actors;
    };

} // namespace methhead

#endif // METH_HEAD_SIMULATOR_HPP_INCLUDED
