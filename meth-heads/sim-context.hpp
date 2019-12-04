#ifndef METHHEADS_SIM_CONTEXT_HPP_INCLUDED
#define METHHEADS_SIM_CONTEXT_HPP_INCLUDED
//
// sim-context.hpp
//
#include "utils.hpp"

#include <memory>
#include <vector>

namespace methhead
{
    class Random;
    class Settings;
    class PosRefCounter;
    struct DisplayConstants;

    struct IPickup;
    using IPickupUPtr_t = std::unique_ptr<IPickup>;

    struct IActor;
    using IActorUPtr_t = std::unique_ptr<IActor>;

    //

    struct SimContext
    {
        SimContext(
            const Random & rand,
            const std::vector<IActorUPtr_t> & acts,
            const std::vector<IPickupUPtr_t> & pups,
            const DisplayConstants & disp,
            const Settings & sets);

        // TODO move these to a better place...probaby PosRefCounter
        bool isActorAt(const BoardPos_t & posToCheck) const;
        bool isPickupAt(const BoardPos_t & posToCheck) const;

        inline bool isAnythingAt(const BoardPos_t & posToCheck) const
        {
            return (isPickupAt(posToCheck) || isActorAt(posToCheck));
        }

        const Random & random;
        const std::vector<IActorUPtr_t> & actors;
        const std::vector<IPickupUPtr_t> & pickups;
        const DisplayConstants & display;
        const Settings & settings;
    };
} // namespace methhead

#endif // METHHEADS_SIM_CONTEXT_HPP_INCLUDED