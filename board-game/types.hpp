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
            Wall = 0,
            SnakeHead,
            SnakeBody,
            Food,
            Count
        };
    };
} // namespace boardgame

#endif // #define BOARDGAME_TYPES_HPP_INCLUDED