// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// follower-base.cpp
//
#include "follower-base.hpp"

namespace entity
{
    FollowerEffectBase::FollowerEffectBase(
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

    void FollowerEffectBase::update(const Context & context, const float frameTimeSec)
    {
        m_targetPos = findTargetPos(context);
        m_mover.aim(m_sprite.getPosition(), m_targetPos);
        m_sprite.move(m_mover.updateDelta(frameTimeSec));
    }

} // namespace entity
