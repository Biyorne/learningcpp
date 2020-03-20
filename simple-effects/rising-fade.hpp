#ifndef SIMPLE_EFFECTS_RISING_FADE_HPP_INCLUDED
#define SIMPLE_EFFECTS_RISING_FADE_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"
#include "movement.hpp"

namespace entity
{

    class RisingFadeEffect : public EffectBase
    {
      public:
        RisingFadeEffect(
            const Context & context,
            const sf::Texture & texture,
            const float textureSizeWindowRatio)
            : EffectBase(makeSprite(context, texture, textureSizeWindowRatio, context.mouse_pos))
            , m_mover({ 0.0f, -90.0f })
        {}

        virtual ~RisingFadeEffect() = default;

        void update(const Context &, const float frameTimeSec) override
        {
            m_sprite.move(m_mover.updateDelta(frameTimeSec));
            m_sprite.setColor(m_sprite.getColor() - sf::Color(0, 0, 0, 5));
        }

      private:
        Mover m_mover;
    };
} // namespace entity
#endif // SIMPLE_EFFECTS_RISING_FADE_HPP_INCLUDED
