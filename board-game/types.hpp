#ifndef BOARDGAME_TYPES_HPP_INCLUDED
#define BOARDGAME_TYPES_HPP_INCLUDED
//
// types.hpp
//

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
    };
} // namespace boardgame

#endif // #define BOARDGAME_TYPES_HPP_INCLUDED