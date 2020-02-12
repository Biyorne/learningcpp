#ifndef SIMPLE_EFFECTS_WALL_BOUNCER_HPP_INCLUDED
#define SIMPLE_EFFECTS_WALL_BOUNCER_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"
#include "steady-mover.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace entity
{
    class WallBouncerEffect : public EffectBase
    {
      public:
        WallBouncerEffect(
            const Context & context,
            const sf::Texture & texture,
            const sf::Vector2f & velocity,
            const sf::RenderTarget & target)
            : EffectBase(makeSprite(texture, util::sfRandom(context.random, m_fence.bounds)))
            , m_velocity(velocity)
            , m_fence({ sf::Vector2f(0.0f, 0.0f), sf::Vector2f(target.getSize()) })
        {}

        virtual ~WallBouncerEffect() = default;

        void update(const Context &, const float frameTimeSec) override
        {
            const sf::Vector2f posMoved(
                m_velocity.updateAbsolute(frameTimeSec, m_sprite.getPosition()));

            m_sprite.setPosition(posMoved);

            const entity::BounceResult bounceResult(
                m_fence.updateDeltaBounce(m_sprite.getGlobalBounds(), m_velocity.vector));

            m_velocity.vector = bounceResult.velocity;
            m_sprite.move(bounceResult.pos_delta);
        }

      private:
        entity::Velocity m_velocity;
        entity::Fence m_fence;
    };

    class GravityBouncerEffect : public EffectBase
    {
      public:
        GravityBouncerEffect(
            const sf::Texture & texture, const sf::Vector2f & spawnPos, const float gravityMag)
            : EffectBase(makeSprite(texture, spawnPos))
            , m_velocity({ 0.0f, 0.0f })
            , m_acceleration({ 0.0f, gravityMag })
        {}

        virtual ~GravityBouncerEffect() = default;

        void update(const Context &, const float frameTimeSec) override
        {
            m_velocity.vector += m_acceleration.updateDelta(frameTimeSec);
            m_sprite.move(m_velocity.updateDelta(frameTimeSec));
        }

      private:
        Velocity m_velocity;
        Acceleration m_acceleration;
    };
} // namespace entity

#endif // SIMPLE_EFFECTS_WALL_BOUNCER_HPP_INCLUDED
