// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// rising-fade.cpp
//
#include "rising-fade.hpp"

namespace entity
{
    RisingFadeEffect::RisingFadeEffect(
        const Context & context, const sf::Texture & texture, const float textureSizeWindowRatio)
        : EffectBase(makeSprite(context, texture, textureSizeWindowRatio, context.mouse_pos))
        , m_mover({ 0.0f, -90.0f })
    {}

    void RisingFadeEffect::update(const Context &, const float frameTimeSec)
    {
        m_sprite.move(m_mover.updateDelta(frameTimeSec));
        m_sprite.setColor(m_sprite.getColor() - sf::Color(0, 0, 0, 5));
    }
} // namespace entity
