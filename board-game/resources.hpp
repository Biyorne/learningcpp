#ifndef BOARDGAME_RESOURCES_HPP_INCLUDED
#define BOARDGAME_RESOURCES_HPP_INCLUDED
//
// media.hpp
//
#include "check-macros.hpp"
#include "types.hpp"

#include <filesystem>

#include <SFML/Graphics.hpp>

//

namespace boardgame
{
    struct Context;
    struct GameConfig;
    using BoardPos_t = sf::Vector2i;

    //

    struct IMedia
    {
        virtual ~IMedia() = default;

        virtual const sf::Font & font(const Piece) const = 0;
        virtual const sf::Texture & texture(const Piece) const = 0;

        virtual sf::Sprite makeDefaultSprite(
            const Context & context,
            const Piece piece,
            const BoardPos_t & boardPos,
            const sf::Color & color,
            const bool willSkewToFitExactly = false) const = 0;
    };

    //

    class SimpleMedia : public IMedia
    {
      public:
        SimpleMedia() = default;
        virtual ~SimpleMedia() = default;

        void setup(const GameConfig & config);

        const sf::Font & font(const Piece) const override;
        const sf::Texture & texture(const Piece piece) const override;

        sf::Sprite makeDefaultSprite(
            const Context & context,
            const Piece piece,
            const BoardPos_t & boardPos,
            const sf::Color & color,
            const bool willSkewToFitExactly) const override;

      protected:
        void makeDefaultTexture();

        template <typename T>
        void load(const std::filesystem::path & path, T & loadable)
        {
            M_CHECK_LOG_SS(std::filesystem::exists(path), path);
            M_CHECK_LOG_SS(std::filesystem::is_regular_file(path), path);

            const bool loadSuccess{ loadable.loadFromFile(path.string()) };
            M_CHECK_LOG_SS(loadSuccess, path);
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