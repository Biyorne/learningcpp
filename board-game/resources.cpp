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
    Resources::Resources(const std::string & mediaDirPath)
        : media_dir_path(mediaDirPath)
    {
        if (!std::filesystem::exists(media_dir_path) ||
            !std::filesystem::is_directory(media_dir_path))
        {
            std::ostringstream ss;

            ss << "Error:  Resources::Resources(\"" << mediaDirPath << "\", which_became=\""
               << media_dir_path.string() << "\") either does not exist, or is not a directory.";

            throw std::runtime_error(ss.str());
        }

        load(media_dir_path / "font/gentium-plus/gentium-plus.ttf", font);
        load(media_dir_path / "image/boardgame.png", tile_texture);
    }

    Image::Enum Image::charToEnum(const char mapChar)
    {
        switch (mapChar)
        {
            case 'V': return WallVert;
            case 'H': return WallHoriz;
            case 'C': return BlockCorner;
            case 'T': return BlockTop;
            case 'B': return BlockBottom;
            default: return Count;
        }
    }
} // namespace boardgame