#ifndef METHHEADS_ENUMS_HPP_INCLUDED
#define METHHEADS_ENUMS_HPP_INCLUDED
//
// enum.hpp
//
namespace methhead
{
    enum class Piece
    {
        Lazy,
        Greedy,
        Loot,
        Wall
    };

    inline std::string pieceName(const Piece piece)
    {
        switch (piece)
        {
            case Lazy: return "Lazy";
            case Greedy: return "Greedy";
            case Loot: return "Loot";
            case Wall: return "Wall";
            default: return "";
        }
    }

    // static std::string imagePath(const Enum piece)
    //{
    //    switch (piece)
    //    {
    //        case Lazy: return "image/head-1.png";
    //        case Greedy: return "image/head-2.png";
    //        case Loot: return ;
    //        default: return "";
    //    }
    //}

    // static sf::Color color(const Enum piece)
    //{
    //    switch (piece)
    //    {
    //        case Lazy: return sf::Color(80, 80, 255);
    //        case Greedy: return sf::Color(100, 255, 100);
    //        case Loot: return ;
    //        default: return sf::Color::Transparent;
    //    }
    //}
} // namespace methhead

#endif // METHHEADS_ENUMS_HPP_INCLUDED