#ifndef SIMPLE_EFFECTS_MOUSE_FOLLOWER_HPP_INCLUDED
#define SIMPLE_EFFECTS_MOUSE_FOLLOWER_HPP_INCLUDED

#include "follower-base.hpp"

namespace entity
{
    struct MouseFollowerEffect : public FollowerEffectBase
    {
        MouseFollowerEffect(
            const Context & context,
            const sf::Texture & texture,
            const float textureSizeWindowRatio,
            const float acceleration,
            const float speedLimit);

        virtual ~MouseFollowerEffect() = default;

        sf::Vector2f findTargetPos(const Context & context) const override;
    };

} // namespace entity

#endif // SIMPLE_EFFECTS_MOUSE_FOLLOWER_HPP_INCLUDED
