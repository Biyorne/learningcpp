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
        ExploderEffect(const Context & context, const sf::Texture & texture)
            : EffectBase(makeSprite(
                  context,
                  texture,
                  context.sprite_size_ratio_default,
                  (context.window_size * 0.5f)))
            , m_tiles()
            , m_offScreenTexture()
            , m_speedBase(0.0f)
            , m_willExplode(false)
        {
            makeScaledTextureCopy();
        }

        virtual ~ExploderEffect() = default;

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override //-V813
        {
            // EffectBase::draw(target, states);

            if (m_tiles.empty())
            {
                target.draw(m_sprite, states);
            }
            else
            {
                for (const auto & tile : m_tiles)
                {
                    target.draw(tile, states);
                }
            }
        }

        void update(const Context & context, const float frameTimeSec) override
        {
            if (m_willExplode)
            {
                for (auto & tile : m_tiles)
                {
                    tile.update(context, frameTimeSec);
                }
            }
        }

        void handleEvent(const Context & context, const sf::Event & event) override
        {
            if (m_willExplode)
            {
                return;
            }

            m_willExplode =
                ((event.type == sf::Event::MouseButtonPressed) &&
                 (event.mouseButton.button == sf::Mouse::Right));

            if (!m_willExplode)
            {
                return;
            }

            m_speedBase = std::clamp(context.mouse_pos.y, 10.0f, context.window_size.x);

            const sf::Vector2s textureLocalSize{ context.resources.exploder_texture.getSize() };
            const sf::Vector2s mousePos(context.mouse_pos);

            const std::size_t tileCount{ std::clamp(
                mousePos.y, 4_st, std::min(textureLocalSize.x, textureLocalSize.y)) };

            divideIntoRects(context, tileCount, m_offScreenTexture.getTexture());
        }

      private:
        void makeScaledTextureCopy()
        {
            m_offScreenTexture.create(
                static_cast<unsigned>(m_sprite.getGlobalBounds().width),
                static_cast<unsigned>(m_sprite.getGlobalBounds().height));

            m_offScreenTexture.clear();

            sf::Sprite tempSprite(m_sprite);
            tempSprite.setPosition(0.0f, 0.0f);
            tempSprite.move(m_sprite.getOrigin() * m_sprite.getScale());
            m_offScreenTexture.draw(tempSprite);
            m_offScreenTexture.display();
        }

        void divideIntoRects(
            const Context & context, const std::size_t tileCount, const sf::Texture & texture)
        {
            const sf::Vector2i textureSizeInt(texture.getSize());
            const int tileCountPerSide(static_cast<int>(std::sqrt(tileCount))); //-V113
            const sf::Vector2i tileCounts(tileCountPerSide, tileCountPerSide);
            const sf::Vector2i tileSize(textureSizeInt / tileCounts);

            for (int vert(0); vert < textureSizeInt.y; vert += tileSize.y)
            {
                for (int horiz(0); horiz < textureSizeInt.x; horiz += tileSize.x)
                {
                    const sf::IntRect texCoords({ horiz, vert }, tileSize);
                    const sf::Sprite sp(makeParticleSprite(texture, texCoords));
                    const Mover mover(calcMoverForParticle(context, horiz, textureSizeInt));
                    m_tiles.push_back(ParticleEffect(context, sp, mover, sp.getPosition()));
                }
            }
        }

      private:
        Mover calcMoverForParticle(
            const Context & context, const int horiz, const sf::Vector2i & textureSize)
        {
            const bool isOnLeft(horiz < (textureSize.x / 2));

            const float velRatio{ (isOnLeft) ? -1.0f : 1.0f };

            const MoverRatios ratios{ BaseRatios_t(velRatio, -1.0f),
                                      BaseRatios_t(0.0f, 1.0f),
                                      10.0f };

            MoverRanges ranges;
            ranges.velocity.x = 1.0f;
            ranges.velocity.y = 0.5f;

            return MoverFactory::makeFromRanges(context, m_speedBase, ratios, ranges);
        }

        sf::Sprite makeParticleSprite(const sf::Texture & texture, const sf::IntRect & texCoords)
        {
            // TODO clean this up
            sf::Sprite sp(texture, texCoords);
            util::setOrigin2Center(sp);
            sp.setPosition(util::center(sf::FloatRect(texCoords)));
            sp.move(m_sprite.getPosition());
            sp.move(m_sprite.getOrigin() * -m_sprite.getScale());

            return sp;
        }

      private:
        std::vector<ParticleEffect> m_tiles;
        sf::RenderTexture m_offScreenTexture;
        float m_speedBase;
        bool m_willExplode;
    };

} // namespace entity

#endif // SIMPLE_EFFECTS_EXPLODER_HPP_INCLUDED
