#ifndef SIMPLE_EFFECTS_FOLLOWER_HPP_INCLUDED
#define SIMPLE_EFFECTS_FOLLOWER_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"
#include "movement.hpp"
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
            : EffectBase(makeSprite(context, texture, textureSizeWindowRatio))
            , m_targetPos(context.mouse_pos)
            , m_mover()
        {
            const sf::Vector2f acc =
                util::vectorMakeWithMag(m_sprite.getPosition(), m_targetPos, acceleration);

            m_mover.setup({ 0.0f, 0.0f }, acc, speedLimit);
        }

        virtual ~FollowerEffectBase() = default;

        virtual void setTargetPos(const sf::Vector2f & pos) { m_targetPos = pos; }

        virtual sf::Vector2f findTargetPos(const Context &) const { return m_targetPos; }

        void update(const Context & context, const float frameTimeSec) override
        {
            m_targetPos = findTargetPos(context);
            m_mover.aim(m_sprite.getPosition(), m_targetPos);
            m_sprite.move(m_mover.updateDelta(frameTimeSec));
        }

      private:
        sf::Vector2f m_targetPos;
        Mover m_mover;
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
