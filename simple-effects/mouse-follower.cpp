// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// mouse-follower.cpp
//
#include "mouse-follower.hpp"

namespace entity
{
    MouseFollowerEffect::MouseFollowerEffect(
        const Context & context,
        const sf::Texture & texture,
        const float textureSizeWindowRatio,
        const float acceleration,
        const float speedLimit)
        : FollowerEffectBase(context, texture, textureSizeWindowRatio, acceleration, speedLimit)
    {}

    sf::Vector2f MouseFollowerEffect::findTargetPos(const Context & context) const
    {
        return context.mouse_pos;
    }

} // namespace entity
