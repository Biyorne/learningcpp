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

        void update(const Context &, const float elapsedTimeSec) override
        {
            const sf::Vector2f posMoved(
                m_velocity.updateAbsolute(elapsedTimeSec, m_sprite.getPosition()));

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
} // namespace entity

#endif // SIMPLE_EFFECTS_WALL_BOUNCER_HPP_INCLUDED
