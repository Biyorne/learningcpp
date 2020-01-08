#ifndef BOARDGAME_COLLIDER_HPP_INCLUDED
#define BOARDGAME_COLLIDER_HPP_INCLUDED
//
// collider.hpp
//
#include "context.hpp"
#include "pieces.hpp"
#include "sound-player.hpp"

namespace boardgame
{
    struct Collider
    {
        template <typename T>
        static void collide(const Context & context, T & movingPiece, const BoardPos_t & newPos)
        {
            if (context.board.pieceAt(newPos) == Piece::Empty)
            {
                movingPiece.m_boardPos = newPos;
                movingPiece.m_sprite.setPosition(context.board.cell(newPos).center);
            }
        }

        /*
        void collide(
            const Context & context, PieceBase & mover, const PieceUPtrVec_t::iterator targetIter)
        {
            PieceBase & target{ **targetIter };

            if (target.piece() == Piece::Wall)
            {
                return;
            }

            if (target.piece() == Piece::Empty)
            {
                mover.set(context, target.piece(), target.boardPos());
                return;
            }

            if (mover.piece() == Piece::Hero)
            {
                if (target.piece() == Piece::Child)
                {
                    util::swapAndPop(context.pieces, targetIter);
                    mover.set(context, target.piece(), target.boardPos());
                }
            }
            else if (mover.piece() == Piece::Demon)
            {
                if (target.piece() == Piece::Hero)
                {
                    target.m_waitedSoFarSec = -3.0f;
                }
            }
        }

        void collide(const Context & context, PieceBase & mover, const BoardPos_t & newPos)
        {
            const auto foundIter = std::find(
                std::begin(context.pieces),
                std::end(context.pieces),
                [&](const PieceUPtr_t & uptr) { return (uptr->boardPos() == newPos); });

            if (foundIter != std::end(context.pieces))
            {
                collide(context, mover, foundIter);
            }
        }
        */
    };
} // namespace boardgame

#endif // BOARDGAME_COLLIDER_HPP_INCLUDED