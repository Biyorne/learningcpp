#ifndef SIMPLE_EFFECTS_RISING_FADE_HPP_INCLUDED
#define SIMPLE_EFFECTS_RISING_FADE_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"
#include "steady-mover.hpp"

struct RisingFadeEffect : public EffectBase
{
    RisingFadeEffect()
        : EffectBase(Mover({}, { 0.0f, -1.5f }))
    {
        sprite.setScale(0.25f, 0.25f);
    }

    RisingFadeEffect(const sf::Texture & tex, const sf::Vector2f & pos)
        : EffectBase(tex, Mover(100.0f, pos, { 0.0f, -1.5f }))
    {
        sprite.setScale(0.25f, 0.25f);
    }

    virtual ~RisingFadeEffect() = default;

    void update(const float elapsedTimeSec, const Context & context) override
    {
        EffectBase::update(elapsedTimeSec, context);
        sprite.setColor(sprite.getColor() - sf::Color(0, 0, 0, 8));
    }
};

#endif // SIMPLE_EFFECTS_RISING_FADE_HPP_INCLUDED
