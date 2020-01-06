#ifndef BOARDGAME_RESOURCES_HPP_INCLUDED
#define BOARDGAME_RESOURCES_HPP_INCLUDED
//
// resources.hpp
//
#include "meth-head.hpp"

#include <sstream>
#include <vector>

#include <SFML/Graphics.hpp>

namespace boardgame
{
    struct Resources
    {
        Resources(const std::string & mediaDirPath)
        {
            load(mediaDirPath, "/font/gentium-plus.ttf", font);

            default_text.setFont(font);
            default_text.setCharacterSize(99);

            load(mediaDirPath, PlayerPiece::imageRelativePath(), hero_texture);
            load(mediaDirPath, DemonPiece::imageRelativePath(), demon_texture);
            load(mediaDirPath, ChildPiece::imageRelativePath(), child_texture);
            load(mediaDirPath, WallPiece::imageRelativePath(), wall_texture);
        }

        template <typename T>
        static void
            load(const std::string & mediaDirPath, const std::string & relativePath, T & loadable)
        {
            const std::string fullPath{ mediaDirPath + relativePath };

            if (!loadable.loadFromFile(fullPath))
            {
                std::ostringstream ss;

                ss << "Error:  Resources::load(media_dir=\"" << mediaDirPath << "\", rel_path=\""
                   << relativePath << "\")  full_path=\"" << fullPath << "\" -failed to load.";

                throw std::runtime_error(ss.str());
            }

            if constexpr (std::is_same_v<std::remove_cv_t<T>, sf::Texture>)
            {
                loadable.setSmooth(true);
            }
        }

        sf::Font font;
        sf::Text default_text;

        sf::Texture hero_texture;
        sf::Texture demon_texture;
        sf::Texture child_texture;
        sf::Texture wall_texture;
    };
} // namespace boardgame

#endif // #define BOARDGAME_RESOURCES_HPP_INCLUDED