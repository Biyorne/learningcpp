#ifndef SIMPLE_EFFECTS_FOLLOWER_BASE_HPP_INCLUDED
#define SIMPLE_EFFECTS_FOLLOWER_BASE_HPP_INCLUDED

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
            const float speedLimit);

        virtual ~FollowerEffectBase() = default;

        virtual void setTargetPos(const sf::Vector2f & pos) { m_targetPos = pos; }

        virtual sf::Vector2f findTargetPos(const Context &) const { return m_targetPos; }

        void update(const Context & context, const float frameTimeSec) override;

      private:
        sf::Vector2f m_targetPos;
        Mover m_mover;
    };
} // namespace entity

#endif // SIMPLE_EFFECTS_FOLLOWER_BASE_HPP_INCLUDED
