#ifndef BOARDGAME_RESOURCES_HPP_INCLUDED
#define BOARDGAME_RESOURCES_HPP_INCLUDED
//
// resources.hpp
//
#include "board.hpp"
#include "check-macros.hpp"
#include "context.hpp"
#include "types.hpp"

#include <cassert>
#include <filesystem>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

//

namespace boardgame
{
    struct Context;

    //

    struct IResources
    {
        virtual ~IResources() = default;

        virtual const sf::Font & font(const Piece) const = 0;
        virtual const sf::Texture & texture(const Piece) const = 0;

        virtual sf::Sprite makeDefaultSprite(
            const Context & context,
            const Piece piece,
            const BoardPos_t & boardPos,
            const sf::Color & color) const = 0;
    };

    //

    class ResourcesBase : public IResources
    {
      public:
        ResourcesBase() = default;
        virtual ~ResourcesBase() = default;

        void setup(const std::filesystem::path & mediaDirPath)
        {
            makeDefaultTexture();
            load((mediaDirPath / "font/gentium-plus/gentium-plus.ttf"), m_font);
        }

        const sf::Font & font(const Piece) const override { return m_font; }

        const sf::Texture & texture(const Piece piece) const override
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

        sf::Sprite makeDefaultSprite(
            const Context & context,
            const Piece which,
            const BoardPos_t & boardPos,
            const sf::Color & color) const override
        {
            M_CHECK(Piece::Count != which);
            sf::Sprite sprite(texture(which));
            util::scaleAndCenterInside(sprite, context.board.layout().bounds(boardPos));
            sprite.setColor(color);
            return sprite;
        }

      protected:
        void makeDefaultTexture()
        {
            sf::Image image;
            image.create(1, 1, sf::Color::White);
            m_defaultTexture.loadFromImage(image);
        }

        template <typename T>
        void load(const std::filesystem::path & path, T & loadable)
        {
            if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
            {
                M_CHECK_LOG_SS(std::filesystem::exists(path), path);
                M_CHECK_LOG_SS(std::filesystem::is_regular_file(path), path);
                return;
            }

            if (!loadable.loadFromFile(path.string()))
            {
                std::cout << "Error:  ResourcesBase::load(\"" << path
                          << "\")  failed the sfml.loadFromFile() call." << std::endl;
            }
        }

      protected:
        std::vector<std::unique_ptr<sf::Texture>> m_textures;
        sf::Font m_font;

        static inline sf::Texture m_defaultTexture;
    };

    // void loadImages() override
    // {
    //     for (std::size_t i(0); i < Piece::Count; ++i)
    //     {
    //         const Piece piece = static_cast<Piece>(i);
    //         m_textures.push_back(std::make_unique<sf::Texture>());
    //         load(imagePath(piece), *m_textures.back());
    //     }
    // }

    // void loadFonts() override
    //{
    //    load((m_mediaPath / "font/gentium-plus/gentium-plus.ttf"), m_font);
    //}
} // namespace boardgame

#endif // #define BOARDGAME_RESOURCES_HPP_INCLUDED