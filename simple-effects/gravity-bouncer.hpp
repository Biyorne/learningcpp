#ifndef SIMPLE_EFFECTS_GRAVITY_BOUNCER_HPP_INCLUDED
#define SIMPLE_EFFECTS_GRAVITY_BOUNCER_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"
#include "movement.hpp"
#include "random.hpp"

#include <SFML/Graphics.hpp>

namespace entity
{
    class GravityBouncerEffect : public EffectBase
    {
      public:
        GravityBouncerEffect(
            const Context & context, const sf::Texture & texture, const sf::Vector2f & velocity)
            : EffectBase(makeSprite(
                  context,
                  texture,
                  context.sprite_size_ratio_default,
                  util::sfRandom(context.random, context.window_rect)))
            , m_mover(context.window_rect, velocity)
        {}

      private:
        FencedMover m_mover;
    };

    // TODO implement using Mover class
    // class GravityBouncerEffect : public EffectBase
    //{
    //  public:
    //    GravityBouncerEffect(
    //        const Context & context,
    //        const sf::Texture & texture,
    //        const float textureSizeWindowRatio,
    //        const sf::Vector2f & spawnPos,
    //        const float gravityMag)
    //        : EffectBase(makeSprite(context, texture, textureSizeWindowRatio, spawnPos))
    //        , m_velocity({ 0.0f, 0.0f })
    //        , m_acceleration({ 0.0f, gravityMag })
    //    {}
    //
    //    virtual ~GravityBouncerEffect() = default;
    //
    //    void update(const Context &, const float frameTimeSec) override
    //    {
    //        m_velocity.vector += m_acceleration.updateDelta(frameTimeSec);
    //        m_sprite.move(m_velocity.updateDelta(frameTimeSec));
    //    }
    //
    //  private:
    //    MovementVector m_velocity;
    //    MovementVector m_acceleration;
    //};
} // namespace entity

#endif // SIMPLE_EFFECTS_GRAVITY_BOUNCER_HPP_INCLUDED
