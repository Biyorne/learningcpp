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
            : m_velocity(velocity)
            , m_fence({ sf::Vector2f(0.0f, 0.0f), sf::Vector2f(target.getSize()) })
        {
            sprite.setTexture(texture);

            util::setOrigin2Center(sprite);

            sprite.setPosition(
                context.random.zeroTo(m_fence.bounds.width),
                context.random.zeroTo(m_fence.bounds.height));
        }

        virtual ~WallBouncerEffect() = default;

        void update(const Context &, const float elapsedTimeSec) override
        {
            const sf::Vector2f posMoved(
                m_velocity.updateAbsolute(elapsedTimeSec, sprite.getPosition()));

            sprite.setPosition(posMoved);

            const entity::BounceResult bounceResult(
                m_fence.updateDeltaBounce(sprite.getGlobalBounds(), m_velocity.vector));

            m_velocity.vector = bounceResult.velocity;
            sprite.move(bounceResult.pos_delta);
        }

      private:
        entity::Velocity m_velocity;
        entity::Fence m_fence;
    };
} // namespace entity

#endif // SIMPLE_EFFECTS_WALL_BOUNCER_HPP_INCLUDED
