// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// exploder.cpp
//
#include "exploder.hpp"

namespace entity
{
    ExploderEffect::ExploderEffect(const Context & context, const sf::Texture & texture)
        : EffectBase(makeSprite(
              context, texture, context.sprite_size_ratio_default, (context.window_size * 0.5f)))
        , m_tiles()
        , m_offScreenTexture()
        , m_speedBase(0.0f)
        , m_willExplode(false)
    {
        makeScaledTextureCopy();
    }

    void ExploderEffect::draw(sf::RenderTarget & target, sf::RenderStates states) const //-V813
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

    void ExploderEffect::update(const Context & context, const float frameTimeSec)
    {
        if (m_willExplode)
        {
            for (auto & tile : m_tiles)
            {
                tile.update(context, frameTimeSec);
            }
        }
    }

    void ExploderEffect::handleEvent(const Context & context, const sf::Event & event)
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

    void ExploderEffect::makeScaledTextureCopy()
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

    void ExploderEffect::divideIntoRects(
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

    Mover ExploderEffect::calcMoverForParticle(
        const Context & context, const int horiz, const sf::Vector2i & textureSize)
    {
        const bool isOnLeft(horiz < (textureSize.x / 2));

        const float velRatio{ (isOnLeft) ? -1.0f : 1.0f };

        const MoverRatios ratios{ BaseRatios_t(velRatio, -1.0f), BaseRatios_t(0.0f, 1.0f), 10.0f };

        MoverRanges ranges;
        ranges.velocity.x = 1.0f;
        ranges.velocity.y = 0.5f;

        return MoverFactory::makeFromRanges(context, m_speedBase, ratios, ranges);
    }

    sf::Sprite ExploderEffect::makeParticleSprite(
        const sf::Texture & texture, const sf::IntRect & texCoords)
    {
        // TODO clean this up
        sf::Sprite sp(texture, texCoords);
        util::setOrigin2Center(sp);
        sp.setPosition(util::center(sf::FloatRect(texCoords)));
        sp.move(m_sprite.getPosition());
        sp.move(m_sprite.getOrigin() * -m_sprite.getScale());

        return sp;
    }

} // namespace entity
