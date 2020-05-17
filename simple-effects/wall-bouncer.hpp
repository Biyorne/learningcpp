#ifndef SIMPLE_EFFECTS_WALL_BOUNCER_HPP_INCLUDED
#define SIMPLE_EFFECTS_WALL_BOUNCER_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"
#include "movement.hpp"
#include "random.hpp"

#include <SFML/Graphics.hpp>

namespace entity
{
    class WallBouncerEffect : public EffectBase
    {
      public:
        WallBouncerEffect(
            const Context & context, const sf::Texture & texture, const sf::Vector2f & velocity);

        virtual ~WallBouncerEffect() = default;

        void update(const Context &, const float frameTimeSec) override;

      private:
        FencedMover m_mover;
    };

} // namespace entity

#endif // SIMPLE_EFFECTS_WALL_BOUNCER_HPP_INCLUDED
