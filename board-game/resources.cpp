// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// resources.cpp
//
#include "resources.hpp"

#include "board.hpp"
#include "context.hpp"
#include "settings.hpp"

#include <cassert>
#include <filesystem>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

//

namespace boardgame
{
    void SimpleMedia::setup(const GameConfig & config)
    {
        makeDefaultTexture();
        load((config.media_dir_path / "font/gentium-plus/gentium-plus.ttf"), m_font);
    }

    const sf::Font & SimpleMedia::font(const Piece) const { return m_font; }

    const sf::Texture & SimpleMedia::texture(const Piece piece) const
    {
        const std::size_t index = static_cast<std::size_t>(piece);
        if (index < m_textures.size())
        {
            return *m_textures.at(index);
        }
        else
        {
            return m_defaultTexture;
        }
    }

    sf::Sprite SimpleMedia::makeDefaultSprite(
        const Context & context,
        const Piece piece,
        const BoardPos_t & boardPos,
        const sf::Color & color,
        const bool willSkewToFitExactly) const
    {
        M_CHECK(Piece::Count != piece);
        sf::Sprite sprite(texture(piece));

        const sf::FloatRect bounds{ context.layout.cellBounds(boardPos) };
        util::scaleAndCenterInside(sprite, bounds, willSkewToFitExactly);
        sprite.setColor(color);
        return sprite;
    }

    void SimpleMedia::makeDefaultTexture()
    {
        sf::Image image;
        image.create(1, 1, sf::Color::White);
        m_defaultTexture.loadFromImage(image);
    }

    // void loadImages()
    // {
    //     for (std::size_t i(0); i < Piece::Count; ++i)
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
} // namespace boardgame