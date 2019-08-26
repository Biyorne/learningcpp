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
        void run();

    private:
        void spawnMethHead(
            const Motivation motive,
            std::vector<IActorUPtr_t> & actors,
            BoardMap_t & board,
            const DisplayConstants & displayConstants,
            const Random & random);

        void printResults(const std::size_t lazyFinalScore, const std::size_t greedyFinalScore);

    private:
    };

} // namespace methhead

#endif // METH_HEAD_SIMULATOR_HPP_INCLUDED
