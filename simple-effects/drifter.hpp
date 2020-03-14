#ifndef SIMPLE_EFFECTS_DRIFTER_HPP_INCLUDED
#define SIMPLE_EFFECTS_DRIFTER_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"
#include "movement.hpp"

namespace entity
{
    /*
    struct DrifterEffect : public EffectBase
    {
        DrifterEffect(const Context & context, const float speed, const sf::Texture & drifterTex)
            : EffectBase(drifterTex, Mover(speed, (context.window_size * 0.5f), {}, {}, speed))
            , target_pos(
                  context.random.zeroTo(context.window_size.x),
                  context.random.zeroTo(context.window_size.y))
        {
            m_sprite.setScale(0.5f, 0.5f);
        }

        virtual ~DrifterEffect() = default;

        void update(const float frameTimeSec, const Context & context) override
        {
            // Accelerate toward target
            const sf::Vector2f posDiff(target_pos - mover.position);
            const float distance(std::sqrt((posDiff.x * posDiff.x) + (posDiff.y * posDiff.y)));

            if (distance > 1.0f)
            {
                const sf::Vector2f posDiffNorm(posDiff / distance);
                mover.velocity += ((posDiffNorm * mover.speed) * frameTimeSec);
            }
            else
            {
                mover.position = target_pos;
                mover.velocity = { 0.0f, 0.0f };
            }

            EffectBase::update(frameTimeSec, context);

            // Set target to random position
        }

        sf::Vector2f target_pos;
    };
    */

} // namespace entity

#endif // SIMPLE_EFFECTS_DRIFTER_HPP_INCLUDED
