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

        virtual const sf::Font & font() const = 0;
        virtual sf::Color color(const Piece::Enum) const = 0;
        virtual const sf::Texture & texture(const Piece::Enum piece) const = 0;

      protected:
        virtual void preLoad() = 0;
        virtual std::filesystem::path imagePath(const Piece::Enum piece) const = 0;
    };

    //

    class ResourcesBase : public IResources
    {
      public:
        explicit ResourcesBase(const std::filesystem::path & mediaDirPath)
            : m_mediaDirPath(mediaDirPath)
            , m_textures()
            , m_font()
        {
            // m_textures.reserve(static_cast<std::size_t>(Piece::Count));
            preLoad();
        }

        virtual ~ResourcesBase() = default;

        const sf::Font & font() const override { return m_font; }

        sf::Color color(const Piece::Enum) const override { return sf::Color::White; }

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
        void preLoad() override
        {
            for (std::size_t i(0); i < Piece::Count; ++i)
            {
                const Piece::Enum piece = static_cast<Piece::Enum>(i);
                m_textures.push_back(std::make_unique<sf::Texture>());
                load(imagePath(piece), *m_textures.back());
            }

            load((m_mediaDirPath / "font/gentium-plus/gentium-plus.ttf"), m_font);
        }

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

        std::filesystem::path imagePath(const Piece::Enum piece) const override
        {
            return (m_mediaDirPath / Piece::imageRelativePath(piece));
        }

      protected:
        std::filesystem::path m_mediaDirPath;
        std::vector<std::unique_ptr<sf::Texture>> m_textures;
        sf::Font m_font;

      private:
        static inline sf::Texture m_defaultTexture{};
    };

    //

    class Resources : public ResourcesBase
    {
      public:
        Resources(const std::filesystem::path & mediaDirPath)
            : ResourcesBase(mediaDirPath)
        {}

        virtual ~Resources() = default;
    };
} // namespace boardgame

#endif // #define BOARDGAME_RESOURCES_HPP_INCLUDED