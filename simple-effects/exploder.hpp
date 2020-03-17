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
        ExploderEffect(
            const Context & context,
            const sf::Texture & texture,
            const float,
            const std::size_t tileCount)
            : EffectBase(makeSprite(
                  context,
                  texture,
                  context.sprite_size_ratio_default,
                  (context.window_size * 0.5f)))
            , m_tiles()
            , m_offScreenTexture()
            , m_willExplode(false)
        {
            m_tiles.reserve(tileCount);

            makeScaledTextureCopy();
            divideIntoRects(context, tileCount, m_offScreenTexture.getTexture());
        }

        virtual ~ExploderEffect() = default;

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            // EffectBase::draw(target, states);

            for (const auto & tile : m_tiles)
            {
                target.draw(tile, states);
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

        void handleEvent(const Context &, const sf::Event & event) override
        {
            if (m_willExplode)
            {
                return;
            }

            m_willExplode =
                ((event.type == sf::Event::MouseButtonPressed) &&
                 (event.mouseButton.button == sf::Mouse::Right));
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
            const int tileCountPerSide(static_cast<int>(std::sqrt(tileCount)));
            const sf::Vector2i tileCounts(tileCountPerSide, tileCountPerSide);
            const sf::Vector2i tileSize(textureSizeInt / tileCounts);

            const Mover mover({ 0.0f, -300.0f }, { 0.0f, 300.0f }, 300.0f);

            for (int vert(0); vert < textureSizeInt.y; vert += tileSize.y)
            {
                for (int horiz(0); horiz < textureSizeInt.x; horiz += tileSize.x)
                {
                    const sf::IntRect tileBounds({ horiz, vert }, tileSize);
                    sf::Sprite sp(texture, tileBounds);
                    util::setOrigin2Center(sp);
                    sp.setPosition(util::center(sf::FloatRect(tileBounds)));
                    sp.move(m_sprite.getPosition());
                    sp.move(m_sprite.getOrigin() * -m_sprite.getScale());
                    m_tiles.push_back(
                        ParticleEffect(context, sp, mover, (context.window_size * 0.5f)));
                }
            }
        }

      private:
        std::vector<ParticleEffect> m_tiles;
        sf::RenderTexture m_offScreenTexture;
        bool m_willExplode;
    };

} // namespace entity

#endif // SIMPLE_EFFECTS_EXPLODER_HPP_INCLUDED
