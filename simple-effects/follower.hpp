#ifndef SIMPLE_EFFECTS_FOLLOWER_HPP_INCLUDED
#define SIMPLE_EFFECTS_FOLLOWER_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"
#include "steady-mover.hpp"
#include "util.hpp"

namespace entity
{
    // accelerate a sprite toward a target
    class FollowerEffectBase : public EffectBase
    {
      public:
        FollowerEffectBase(
            const Context & context,
            const sf::Texture & texture,
            const float textureSizeWindowRatio,
            const float acceleration,
            const float speedLimit)
            : EffectBase(makeSprite(texture))
            , m_velocity({ 0.0f, 0.0f })
            , m_acceleration({ 0.0f, 0.0f })
            , m_speedLimit(speedLimit)
            , m_targetPos(m_sprite.getPosition())
            , m_accelerationMag(acceleration)
        {}

        virtual ~FollowerEffectBase() = default;

        virtual void setTargetPos(const sf::Vector2f & pos) { m_targetPos = pos; }

        virtual sf::Vector2f findTargetPos(const Context &) const { return m_targetPos; }

        void update(const Context & context, const float frameTimeSec) override
        {
            m_targetPos = findTargetPos(context);

            m_acceleration.vector =
                (util::vecNormal(m_targetPos - m_sprite.getPosition()) * m_accelerationMag);

            m_velocity.vector += m_acceleration.updateDelta(frameTimeSec);

            m_velocity.vector = m_speedLimit.clamp(m_velocity.vector);

            m_sprite.move(m_velocity.updateDelta(frameTimeSec));
        }

      private:
        Velocity m_velocity;
        Acceleration m_acceleration;
        MagnitudeClamp m_speedLimit;

        sf::Vector2f m_targetPos;
        float m_accelerationMag;
    };

    //

    struct MouseFollowerEffect : public FollowerEffectBase
    {
        MouseFollowerEffect(
            const Context & context,
            const sf::Texture & texture,
            const float textureSizeWindowRatio,
            const float acceleration,
            const float speedLimit)
            : FollowerEffectBase(context, texture, textureSizeWindowRatio, acceleration, speedLimit)
        {}

        virtual ~MouseFollowerEffect() = default;

        sf::Vector2f findTargetPos(const Context & context) const override
        {
            return context.mouse_pos;
        }
    };

} // namespace entity

#endif // SIMPLE_EFFECTS_FOLLOWER_HPP_INCLUDED
