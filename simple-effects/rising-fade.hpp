#ifndef SIMPLE_EFFECTS_RISING_FADE_HPP_INCLUDED
#define SIMPLE_EFFECTS_RISING_FADE_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"
#include "movement.hpp"

namespace entity
{

    class RisingFadeEffect : public EffectBase
    {
      public:
        RisingFadeEffect(
            const Context & context,
            const sf::Texture & texture,
            const float textureSizeWindowRatio);

        virtual ~RisingFadeEffect() = default;

        void update(const Context &, const float frameTimeSec) override;

      private:
        Mover m_mover;
    };
} // namespace entity
#endif // SIMPLE_EFFECTS_RISING_FADE_HPP_INCLUDED
