#ifndef BOARDGAME_RESOURCES_HPP_INCLUDED
#define BOARDGAME_RESOURCES_HPP_INCLUDED
//
// resources.hpp
//
#include "types.hpp"

#include <cassert>
#include <filesystem>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

//

namespace boardgame
{
    struct IResources
    {
        virtual ~IResources() = default;

        virtual const sf::Font & font(const Piece::Enum) const = 0;
        virtual const sf::Texture & texture(const Piece::Enum) const = 0;
    };

    //

    class ResourcesBase : public IResources
    {
      public:
        explicit ResourcesBase(const std::filesystem::path & mediaDirPath)
            : m_mediaPath(mediaDirPath)
            , m_textures()
            , m_font()
        {}

        virtual ~ResourcesBase() = default;

        const sf::Font & font(const Piece::Enum) const override { return m_font; }

        const sf::Texture & texture(const Piece::Enum piece) const override
        {
            const std::size_t index = static_cast<std::size_t>(piece);

            if (index < m_textures.size())
            {
                return *m_textures.at(index);
            }

            return m_defaultTexture;
        }

      protected:
        // void loadImages() override
        // {
        //     for (std::size_t i(0); i < Piece::Count; ++i)
        //     {
        //         const Piece::Enum piece = static_cast<Piece::Enum>(i);
        //         m_textures.push_back(std::make_unique<sf::Texture>());
        //         load(imagePath(piece), *m_textures.back());
        //     }
        // }

        // void loadFonts() override
        //{
        //    load((m_mediaPath / "font/gentium-plus/gentium-plus.ttf"), m_font);
        //}

        template <typename T>
        void load(const std::filesystem::path & path, T & loadable)
        {
            if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
            {
                std::cout
                    << "Error:  ResourcesBase::load(\"" << path
                    << "\") failed because that file does not exist, or is not a regular file."
                    << std::endl;

                return;
            }

            if (!loadable.loadFromFile(path.string()))
            {
                std::cout << "Error:  ResourcesBase::load(\"" << path
                          << "\")  failed the sfml.loadFromFile() call." << std::endl;
            }
        }

        // default is kinda useless
        // virtual std::filesystem::path imagePath(const Piece::Enum) const { return m_mediaPath;
        // }

      protected:
        std::filesystem::path m_mediaPath;
        std::vector<std::unique_ptr<sf::Texture>> m_textures;
        sf::Font m_font;

        static inline sf::Texture m_defaultTexture{};
    };

    //

    class SnakeResources : public ResourcesBase
    {
      public:
        SnakeResources(const std::filesystem::path & mediaDirPath)
            : ResourcesBase(mediaDirPath)
        {
            load((m_mediaPath / "font/gentium-plus/gentium-plus.ttf"), m_font);

            // make the default texture a solid white cell-sozed square
            sf::Image image;
            image.create(256, 256, sf::Color::White);
            m_defaultTexture.loadFromImage(image);
        }

        virtual ~SnakeResources() = default;
    };
} // namespace boardgame

#endif // #define BOARDGAME_RESOURCES_HPP_INCLUDED