#ifndef SIMPLE_EFFECTS_FOLLOWER_HPP_INCLUDED
#define SIMPLE_EFFECTS_FOLLOWER_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"
#include "steady-mover.hpp"
#include "util.hpp"

namespace entity
{
    // accelerate a sprite toward a target
    class FollowerEffect : public EffectBase
    {
      public:
        FollowerEffect(
            const sf::Texture & texture,
            const sf::Vector2f & spawnPos,
            const float accMagnitude,
            const sf::Vector2f & targetPos)
            : EffectBase(makeSprite(texture, spawnPos))
            , m_velocity({ 0.0f, 0.0f })
            , m_acceleration({ 0.0f, 0.0f })
            , m_targetPos(targetPos)
            , m_accelerationMag(accMagnitude)
        {}

        virtual ~FollowerEffect() = default;

        void setTargetPos(const sf::Vector2f & pos) { m_targetPos = pos; }

        void update(const Context & context, const float frameTimeSec) override
        {
            m_targetPos = context.mouse_pos;

            m_acceleration.vector =
                (util::vecNormal(m_targetPos - m_sprite.getPosition()) * m_accelerationMag);

            m_velocity.vector += m_acceleration.updateDelta(frameTimeSec);

            const float speedLimit{ 500.0f };
            const float speed{ util::vecMagnitude(m_velocity.vector) };
            if (speed > speedLimit)
            {
                m_velocity.vector *= (speedLimit / speed);
            }

            m_sprite.move(m_velocity.updateDelta(frameTimeSec));
        }

      private:
        Velocity m_velocity;
        Acceleration m_acceleration;
        sf::Vector2f m_targetPos;
        float m_accelerationMag;
    };
} // namespace entity

#endif // SIMPLE_EFFECTS_FOLLOWER_HPP_INCLUDED