#ifndef SIMPLE_EFFECTS_BASE_HPP_INCLUDED
#define SIMPLE_EFFECTS_BASE_HPP_INCLUDED

#include "context.hpp"
#include "steady-mover.hpp"

#include <SFML/Graphics.hpp>

namespace entity
{
    struct EffectBase : public sf::Drawable
    {
        explicit EffectBase(const sf::Sprite & spr)
            : sprite(spr)
        {
            util::setOrigin2Center(sprite);
        }

        virtual ~EffectBase() = default;

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            target.draw(sprite, states);
        }

        virtual void update(const Context & context, const float elapsedTimeSec) = 0;

        sf::Sprite sprite;
    };
} // namespace entity
#endif // SIMPLE_EFFECTS_BASE_HPP_INCLUDED
