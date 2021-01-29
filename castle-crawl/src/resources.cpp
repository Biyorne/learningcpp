// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// resources.cpp
//
#include "resources.hpp"

#include "context.hpp"
#include "settings.hpp"

#include <cassert>
#include <filesystem>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

namespace castlecrawl
{
    void Media::load(const GameConfig & config, const Layout & layout, util::SoundPlayer & audio)
    {
        makeDefaults();
        load((config.media_dir_path / "font/gentium-plus/gentium-plus.ttf"), m_font);

        loadTileSprites(config, layout);
        loadSoundEffects(audio);
    }

    void Media::loadTileSprites(const GameConfig & config, const Layout & layout)
    {
        const std::size_t imageCount = static_cast<std::size_t>(TileImage::Count);
        m_tileSprites.resize(imageCount);

        load((config.media_dir_path / "image/tile.png"), m_tileTexture);

        for (sf::Sprite & sprite : m_tileSprites)
        {
            sprite.setTexture(m_tileTexture);
        }

        // keep in sync with enum declaration
        for (std::size_t i(0); i < imageCount; ++i)
        {
            m_tileSprites.at(i).setTextureRect(tileImageToTileRect(static_cast<TileImage>(i)));
            util::fit(m_tileSprites.at(i), layout.mapCellSize());
        }

        m_tileSprites.at(static_cast<std::size_t>(TileImage::Door))
            .setColor(sf::Color(160, 130, 100));

        m_tileSprites.at(static_cast<std::size_t>(TileImage::Barrel))
            .setColor(sf::Color(110, 60, 10));

        m_tileSprites.at(static_cast<std::size_t>(TileImage::Coffin))
            .setColor(sf::Color(130, 75, 40));

        m_tileSprites.at(static_cast<std::size_t>(TileImage::Empty)) = m_defaultSprite;
    }

    void Media::loadSoundEffects(util::SoundPlayer & audio)
    {
        M_CHECK(audio.load(
            { "tap-wood-low.ogg", "locked.ogg", "door-open.ogg", "tick-on-2.ogg", "stairs.ogg" }));
    }

    const sf::Font & Media::font() const { return m_font; }

    const sf::Sprite & Media::sprite(const TileImage image) const
    {
        const std::size_t index = static_cast<std::size_t>(image);
        if (index < m_tileSprites.size())
        {
            return m_tileSprites.at(index);
        }
        else
        {
            return m_defaultSprite;
        }
    }

    void Media::makeDefaults()
    {
        sf::Image image;
        image.create(1, 1, sf::Color::Transparent);
        m_defaultTexture.loadFromImage(image);
        m_defaultSprite.setTexture(m_defaultTexture, true);
    }

    // void loadImages()
    // {
    //     for (std::size_t i(0); i < TileImage::Count; ++i)
    //     {
    //         const Piece piece = static_cast<Piece>(i);
    //         m_textures.push_back(std::make_unique<sf::Texture>());
    //         load(imagePath(piece), *m_textures.back());
    //     }
    // }

    // void loadFonts()
    //{
    //    load((m_mediaPath / "font/gentium-plus/gentium-plus.ttf"), m_font);
    //}
} // namespace castlecrawl
