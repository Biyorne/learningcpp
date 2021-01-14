#ifndef CASTLECRAWL_RESOURCES_HPP_INCLUDED
#define CASTLECRAWL_RESOURCES_HPP_INCLUDED
//
// resources.hpp
//
#include "check-macros.hpp"
#include "context.hpp"
#include "tile-image.hpp"

#include <filesystem>

#include <SFML/Graphics.hpp>

//

namespace castlecrawl
{
    struct GameConfig;

    class Media
    {
      public:
        Media() = default;

        void setup(const GameConfig & config);

        const sf::Font & font() const;
        const sf::Sprite & sprite(const TileImage) const;

      protected:
        void makeDefaults();

        template <typename T>
        void load(const std::filesystem::path & path, T & loadable)
        {
            M_CHECK_LOG_SS(std::filesystem::exists(path), path);
            M_CHECK_LOG_SS(std::filesystem::is_regular_file(path), path);

            const bool loadSuccess{ loadable.loadFromFile(path.string()) };
            M_CHECK_LOG_SS(loadSuccess, path);
        }

      protected:
        sf::Font m_font;
        std::vector<sf::Sprite> m_pieceSprites;
        sf::Texture m_tileTexture;

        static inline sf::Texture m_defaultTexture;
        static inline sf::Sprite m_defaultSprite;
    };

} // namespace castlecrawl

#endif // #define CASTLECRAWL_RESOURCES_HPP_INCLUDED