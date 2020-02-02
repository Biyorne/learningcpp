#ifndef BOARDGAME_RESOURCES_HPP_INCLUDED
#define BOARDGAME_RESOURCES_HPP_INCLUDED
//
// resources.hpp
//
#include "types.hpp"

#include <array>
#include <cassert>
#include <filesystem>
#include <sstream>
#include <vector>

#include <SFML/Graphics.hpp>

//

namespace boardgame
{
    enum class Image
    {
        Blank,
        //
        Demon,
        Pickup,
        Player,
        //
        WhiteGlow,
        WhiteRing,
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
        WoodFloor6
    };

    //

    class ImageHandler
    {
      public:
        explicit ImageHandler(const std::string & mediaDirPath);

        sf::Sprite makeSprite(
            const Image image,
            const sf::FloatRect & windowRect,
            const sf::Color & color = sf::Color::White) const;

      private:
        static sf::IntRect coords(const Image image)
        {
            // clang-format off
            switch (image)
            {
                case Image::Blank:       return { 224,   0,  32,  32 };
                case Image::Player:      return {  96,   0,  64,  64 };
                case Image::Demon:       return { 160,   0,  64,  64 };
                case Image::Pickup:      return { 160,  64,  96,  64 };
                //
                case Image::WhiteGlow:   return {   0, 128, 128, 128 };
                case Image::WhiteRing:   return { 128, 128, 128, 128 };
                //
                case Image::BlockCorner: return {  0,   0,  32,  32 };
                case Image::Shadow:      return { 32,  32,  32,  32 };
                case Image::WallVert:    return {  0,  32,  32,  32 };
                case Image::WallHoriz:   return { 32,   0,  32,  32 };
                case Image::BlockTop:    return { 64,   0,  32,  32 };
                case Image::BlockBottom: return { 64,  32,  32,  32 };
                //
                case Image::WoodFloor1:  return {  0,  64,  32,  32 };
                case Image::WoodFloor2:  return { 32,  64,  32,  32 };
                case Image::WoodFloor3:  return { 64,  64,  32,  32 };
                case Image::WoodFloor4:  return {  0,  96,  32,  32 };
                case Image::WoodFloor5:  return { 32,  96,  32,  32 };
                case Image::WoodFloor6:  return { 64,  96,  32,  32 };
                //
                default: return {  0,   0,   0,   0 };
            }
            // clang-format on
        }

        template <typename T>
        static void load(const std::filesystem::path & path, T & loadable)
        {
            const std::string pathStr{ path.string() };

            if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
            {
                std::ostringstream ss;

                ss << "Error:  ImageHandler::load(\"" << pathStr
                   << "\") failed because that file does not exist, or is not a regular file.";

                throw std::runtime_error(ss.str());
            }

            if (!loadable.loadFromFile(pathStr))
            {
                std::ostringstream ss;

                ss << "Error:  ImageHandler::load(\"" << pathStr
                   << "\")  failed the sfml.loadFromFile() call.";

                throw std::runtime_error(ss.str());
            }

            if constexpr (std::is_same_v<std::remove_cv_t<T>, sf::Texture>)
            {
                // loadable.setSmooth(true);
            }
        }

      private:
        std::filesystem::path m_mediaDirPath;
        sf::Font m_font;
        sf::Texture m_texture;
    };

    //

    struct Map
    {
        // clang-format off
        const std::vector<std::string> map_rows = {
            "CHHHHHHHTHHHHHHHHHHHHHCHCHHHHT",
            "V       V             VPV    V",
            "V   T BHB  BHTHB T    B B    V",
            "V   V        V   V           V",
            "V CHB  BHT   B   BHTHB  CHHB V",
            "V V      V         V    V    V",
            "V B      V T    T  B    B  BHB",
            "V        B V  D V     C      V",
            "V BHTHHB   V CHHB     V    BHT",
            "V   V    BHB V        BHHT   V",
            "V   V T      B   T       V   V",
            "V   B V          V       V   V",
            "BHB   BHB THB BHHBHB T T B   V",
            "V         V          V V     V",
            "BHHHHHHHHHBHHHHHHHHHHBHBHHHHHB" };
        // clang-format on

        const std::size_t cell_count_horiz{ map_rows.front().size() };
        const std::size_t cell_count_vert{ map_rows.size() };
        const std::size_t cell_count_total{ cell_count_horiz * cell_count_vert };

        bool isPosValid(const BoardPos_t & boardPos) const
        {
            const sf::Vector2s mapPos(boardPos);

            return (
                (mapPos.x >= 0) && (mapPos.x < cell_count_horiz) && (mapPos.y >= 0) &&
                (mapPos.y < cell_count_vert));
        }

        char posToChar(const BoardPos_t & pos) const
        {
            assert(isPosValid(pos));
            if (!isPosValid)
            {
                return ' ';
            }

            return map_rows.at(pos.y).at(pos.x);
        }

        Piece charToPiece(const char ch) const
        {
            switch (ch)
            {
                case 'P': return Piece::Player;
                case 'D': return Piece::Villan;

                case 'T':
                case 'B':
                case 'V':
                case 'H':
                case 'C': return Piece::Obstacle;

                case ' ':
                default: return Piece::Empty;
            };
        }

        Piece posToPiece(const BoardPos_t & pos) const { return charToPiece(posToChar(pos)); }

        Image charToImage(const char ch) const
        {
            switch (ch)
            {
                case 'P': return Image::Player;
                case 'D': return Image::Demon;

                case 'T': return Image::BlockTop;
                case 'B': return Image::BlockBottom;
                case 'C': return Image::BlockCorner;

                case 'V': return Image::WallVert;
                case 'H': return Image::WallHoriz;

                case ' ':
                default: return Image::Blank;
            };
        }

        Image posToImage(const BoardPos_t & pos) const { return charToImage(posToChar(pos)); }
    };
} // namespace boardgame

#endif // #define BOARDGAME_RESOURCES_HPP_INCLUDED