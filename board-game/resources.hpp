#ifndef BOARDGAME_RESOURCES_HPP_INCLUDED
#define BOARDGAME_RESOURCES_HPP_INCLUDED
//
// resources.hpp
//
#include <array>
#include <filesystem>
#include <sstream>
#include <vector>

#include <SFML/Graphics.hpp>

namespace util
{
    class Random;
}

//

namespace boardgame
{
    struct Image
    {
        enum Enum : std::size_t
        {
            Demon = 0,
            Victim,
            Player,
            //
            WhitePuff,
            //
            Shadow,
            WallVert,
            WallHoriz,
            //
            BlockCorner,
            BlockTop,
            BlockBottom,
            //
            WoodFloor1,
            WoodFloor2,
            WoodFloor3,
            WoodFloor4,
            WoodFloor5,
            WoodFloor6,
            //
            Count
        };

        static bool isWall(const char mapChar)
        {
            return (
                (mapChar == 'C') || (mapChar == 'V') || (mapChar == 'H') || (mapChar == 'T') ||
                (mapChar == 'B'));
        }

        // clang-format off
        static sf::IntRect coords(const Enum image)
        {
            switch(image)
            {
                case Player     : return {  96,   0,  64,  64 };
                case Demon      : return { 160,   0,  96, 128 };
                case Victim     : return { 128, 128, 128, 128 };
                //
                case WhitePuff  : return {   0, 128, 128, 128 };
                //
                case BlockCorner: return {   0,   0,  32,  32 };
                case Shadow     : return {  32,  32,  32,  32 };
                case WallVert   : return {   0,  32,  32,  32 };
                case WallHoriz  : return {  32,   0,  32,  32 };
                case BlockTop   : return {  64,   0,  32,  32 };
                case BlockBottom: return {  64,  32,  32,  32 };
                //
                case WoodFloor1 : return {   0,  64,  32,  32 };
                case WoodFloor2 : return {  32,  64,  32,  32 };
                case WoodFloor3 : return {  64,  64,  32,  32 };
                case WoodFloor4 : return {   0,  96,  32,  32 };
                case WoodFloor5 : return {  32,  96,  32,  32 };
                case WoodFloor6 : return {  64,  96,  32,  32 };
                //
                case Count      :
                default         :  return {  0,   0,   0,   0 };
            }
        }
        // clang-format on

        static Image::Enum charToEnum(const char mapChar);

        static bool isBlock(const Enum image)
        {
            return ((image == BlockCorner) || (image == BlockTop) || (image == BlockBottom));
        }
    };

    struct Resources
    {
        explicit Resources(const std::string & mediaDirPath);

        const std::filesystem::path media_dir_path;
        sf::Font font;
        sf::Texture tile_texture;

      private:
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
                // loadable.setSmooth(true);
            }
        }
    };
} // namespace boardgame

#endif // #define BOARDGAME_RESOURCES_HPP_INCLUDED