// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// wall-bouncer.cpp
//
#include "wall-bouncer.hpp"

namespace entity
{
    WallBouncerEffect::WallBouncerEffect(
        const Context & context, const sf::Texture & texture, const sf::Vector2f & velocity)
        : EffectBase(makeSprite(
              context,
              texture,
              context.sprite_size_ratio_default,
              util::sfRandom(context.random, context.window_rect)))
        , m_mover(context.window_rect, velocity)
    {}

    void WallBouncerEffect::update(const Context &, const float frameTimeSec)
    {
        const sf::Vector2f posMoved(m_mover.updateDelta(m_sprite.getGlobalBounds(), frameTimeSec));

        m_sprite.move(posMoved);
    }
} // namespace entity
