// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// sim-context.cpp
//
#include "sim-context.hpp"

#include "display-constants.hpp"
#include "meth-head.hpp"
#include "random.hpp"
#include "settings.hpp"

namespace methhead
{
    SimContext::SimContext(
        const Random & rand,
        const std::vector<IActorUPtr_t> & acts,
        const std::vector<IPickupUPtr_t> & pups,
        const DisplayConstants & disp,
        const Settings & sets)
        : random(rand)
        , actors(acts)
        , pickups(pups)
        , display(disp)
        , settings(sets)
    {}

    // TODO Nell:
    //  What is the same about these two functions?
    //  What is different?
    //  What opportunity or other-way-to-write the code does that reveal?
    //  Would that be simpler/more-complex/faster/slower/better/worse?
    bool SimContext::isActorAt(const BoardPos_t & posToCheck) const
    {
        const auto foundIter =
            std::find_if(std::begin(actors), std::end(actors), [&](const IActorUPtr_t & actor) {
                return (actor->boardPos() == posToCheck);
            });

        return (foundIter != std::end(actors));
    }

    bool SimContext::isPickupAt(const BoardPos_t & posToCheck) const
    {
        const auto foundIter =
            std::find_if(std::begin(pickups), std::end(pickups), [&](const IPickupUPtr_t & pickup) {
                return (pickup->boardPos() == posToCheck);
            });

        return (foundIter != std::end(pickups));
    }
} // namespace methhead