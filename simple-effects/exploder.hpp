#ifndef SIMPLE_EFFECTS_EXPLODER_HPP_INCLUDED
#define SIMPLE_EFFECTS_EXPLODER_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"
#include "movement.hpp"
#include "particle-emitter.hpp"
#include "random.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace entity
{
    class ExploderEffect : public EffectBase
    {
      public:
        ExploderEffect(const Context & context, const sf::Texture & texture);

        virtual ~ExploderEffect() = default;

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

        void update(const Context & context, const float frameTimeSec) override;

        void handleEvent(const Context & context, const sf::Event & event) override;

      private:
        void makeScaledTextureCopy();

        void divideIntoRects(
            const Context & context, const std::size_t tileCount, const sf::Texture & texture);

      private:
        Mover calcMoverForParticle(
            const Context & context, const int horiz, const sf::Vector2i & textureSize);

        sf::Sprite makeParticleSprite(const sf::Texture & texture, const sf::IntRect & texCoords);

      private:
        std::vector<ParticleEffect> m_tiles;
        sf::RenderTexture m_offScreenTexture;
        float m_speedBase;
        bool m_willExplode;
    };

} // namespace entity

#endif // SIMPLE_EFFECTS_EXPLODER_HPP_INCLUDED
