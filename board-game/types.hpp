#ifndef BOARDGAME_TYPES_HPP_INCLUDED
#define BOARDGAME_TYPES_HPP_INCLUDED
//
// types.hpp
//
#include <string>

namespace boardgame
{
    struct Piece
    {
        enum Enum
        {
            Head = 0,
            Tail,
            Food,
            Wall,
            Count
        };

        static std::string name(const Enum which)
        {
            switch (which)
            {
                case Head: return "Head";
                case Tail: return "Tail";
                case Food: return "Food";
                case Wall: return "Wall";
                case Count: return "";
                default: return "";
            }
        }
    };
} // namespace boardgame

#endif // #define BOARDGAME_TYPES_HPP_INCLUDED