#ifndef SIMPLE_EFFECTS_WALL_BOUNCER_HPP_INCLUDED
#define SIMPLE_EFFECTS_WALL_BOUNCER_HPP_INCLUDED

#include "effect-base.hpp"
#include "steady-mover.hpp"

struct WallBouncerEffect : public EffectBase
{
    WallBouncerEffect(const sf::Texture & tex, const Mover & mov)
        : EffectBase(tex, mov, true)
    {}

    virtual ~WallBouncerEffect() = default;
};

#endif // SIMPLE_EFFECTS_WALL_BOUNCER_HPP_INCLUDED
