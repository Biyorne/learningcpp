#ifndef SIMPLE_EFFECTS_RISING_FADE_HPP_INCLUDED
#define SIMPLE_EFFECTS_RISING_FADE_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"
#include "steady-mover.hpp"

namespace entity
{

    class RisingFadeEffect : public EffectBase
    {
      public:
        RisingFadeEffect(const Context & context, const sf::Texture & texture)
            : EffectBase(makeSprite(texture, context.mouse_pos))
            , m_velocity({ 0.0f, -90.0f })
        {}

        virtual ~RisingFadeEffect() = default;

        void update(const Context &, const float frameTimeSec) override
        {
            m_sprite.move(m_velocity.updateDelta(frameTimeSec));
            m_sprite.setColor(m_sprite.getColor() - sf::Color(0, 0, 0, 5));
        }

      private:
        Velocity m_velocity;
    };
} // namespace entity
#endif // SIMPLE_EFFECTS_RISING_FADE_HPP_INCLUDED
