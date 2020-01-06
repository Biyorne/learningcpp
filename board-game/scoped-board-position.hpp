#ifndef BOARDGAME_SCOPED_BOARD_POS_HANDLER_HPP_INCLUDED
#define BOARDGAME_SCOPED_BOARD_POS_HANDLER_HPP_INCLUDED
//
// scoped-board-position.hpp
//
#include "context.hpp"
#include "display-constants.hpp"
#include "types.hpp"
#include "util.hpp"

#include <vector>

namespace boardgame
{
    class ScopedBoardPosition
    {
      protected:
        // use this constructor for random placement
        ScopedBoardPosition(const Context & context, const Piece piece);

        // use this constructor for when you already know the pos (i.e. walls)
        ScopedBoardPosition(const Context & context, const Piece piece, const BoardPos_t & pos);

        void set(const Context & context, const Piece piece, const BoardPos_t & newPos) noexcept;

      public:
        virtual ~ScopedBoardPosition() { free(); }

        BoardPos_t get() const noexcept { return m_boardPos; }

        static Piece pieceAt(const Context & context, const BoardPos_t & pos)
        {
            return m_pieceAtIndexes[context.display.boardPosToIndex(pos)];
        }

        static bool isPosFree(const Context & context, const BoardPos_t & pos)
        {
            return (pieceAt(context, pos) == Piece::None);
        }

        static bool isAnyPosFree(const Context & context) noexcept
        {
            return (context.pieces.size() < context.display.cell_count);
        }

        static void reset(const Context & context);

      private:
        // assumes there is at least one free space
        static std::size_t findRandomFreePosIndex(const Context & context) noexcept;

        void free() const noexcept { m_pieceAtIndexes[m_index] = Piece::None; }
        void occupy(const Piece piece) const noexcept { m_pieceAtIndexes[m_index] = piece; }

      private:
        std::size_t m_index;
        BoardPos_t m_boardPos;

        static std::vector<Piece> m_pieceAtIndexes;
    };
} // namespace boardgame

#endif // BOARDGAME_SCOPED_BOARD_POS_HANDLER_HPP_INCLUDED