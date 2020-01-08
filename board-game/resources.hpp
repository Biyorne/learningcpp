#ifndef BOARDGAME_RESOURCES_HPP_INCLUDED
#define BOARDGAME_RESOURCES_HPP_INCLUDED
//
// resources.hpp
//
#include "pieces.hpp"

#include <filesystem>
#include <sstream>
#include <vector>

#include <SFML/Graphics.hpp>

namespace boardgame
{
    struct Resources
    {
        Resources(const std::string & mediaDirPath)
            : media_dir_path(mediaDirPath)
        {
            if (!std::filesystem::exists(media_dir_path) ||
                !std::filesystem::is_directory(media_dir_path))
            {
                std::ostringstream ss;

                ss << "Error:  Resources::Resources(\"" << mediaDirPath << "\", which_became=\""
                   << media_dir_path.string()
                   << "\") either does not exist, or is not a directory.";

                throw std::runtime_error(ss.str());
            }

            load(media_dir_path / "font/gentium-plus/gentium-plus.ttf", font);
            load(media_dir_path / PlayerPiece::imageRelativePath(), hero_texture);
            load(media_dir_path / DemonPiece::imageRelativePath(), demon_texture);
            load(media_dir_path / ChildPiece::imageRelativePath(), child_texture);
            load(media_dir_path / WallPiece::imageRelativePath(), wall_texture);
        }

        template <typename T>
        static void load(const std::filesystem::path & path, T & loadable)
        {
            const std::string pathStr{ path.string() };

            if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
            {
                std::ostringstream ss;

                ss << "Error:  Resources::load(\"" << pathStr
                   << "\") failed because that file does not exist, or is not a regular file.";

                throw std::runtime_error(ss.str());
            }

            if (!loadable.loadFromFile(pathStr))
            {
                std::ostringstream ss;

                ss << "Error:  Resources::load(\"" << pathStr
                   << "\")  failed the sfml.loadFromFile() call.";

                throw std::runtime_error(ss.str());
            }

            if constexpr (std::is_same_v<std::remove_cv_t<T>, sf::Texture>)
            {
                loadable.setSmooth(true);
            }
        }

        const std::filesystem::path media_dir_path;

        sf::Font font;
        sf::Texture hero_texture;
        sf::Texture demon_texture;
        sf::Texture child_texture;
        sf::Texture wall_texture;
    };
} // namespace boardgame

#endif // #define BOARDGAME_RESOURCES_HPP_INCLUDED