// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// resources.cpp
//
#include "resources.hpp"

#include "pieces.hpp"
#include "random.hpp"

namespace boardgame
{
    ImageHandler::ImageHandler(const std::string & mediaDirPath)
        : m_mediaDirPath(mediaDirPath)
        , m_font()
        , m_texture()
    {
        if (!std::filesystem::exists(m_mediaDirPath) ||
            !std::filesystem::is_directory(m_mediaDirPath))
        {
            std::ostringstream ss;

            ss << "Error:  ImageHandler::ImageHandler(\"" << mediaDirPath << "\", which_became=\""
               << m_mediaDirPath.string() << "\") either does not exist, or is not a directory.";

            throw std::runtime_error(ss.str());
        }

        load(m_mediaDirPath / "font/gentium-plus/gentium-plus.ttf", m_font);
        load(m_mediaDirPath / "image/boardgame.png", m_texture);
    }

    sf::Sprite ImageHandler::makeSprite(
        const Image image, const sf::FloatRect & windowRect, const sf::Color & color) const
    {
        sf::Sprite sprite;

        if (Image::Blank != image)
        {
            sprite.setTexture(m_texture);
            sprite.setTextureRect(coords(image));
            sprite.setColor(color);
            util::setOriginToCenter(sprite);
            util::scale(sprite, util::size(windowRect));
            sprite.setPosition(util::center(windowRect));
        }

        return sprite;
    }
} // namespace boardgame