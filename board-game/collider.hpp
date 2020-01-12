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
        static void handle(Context & context, IPiece & pieceMoving, const BoardPos_t & targetPos)
        {
            assert(context.board.isPosValid(targetPos));

            const Piece::Enum pieceMovingEnum{ pieceMoving.piece() };
            assert(pieceMovingEnum != Piece::Count);
            assert(pieceMovingEnum != Piece::Wall);

            const Piece::Enum pieceTargetEnum{ context.board.cellAt(targetPos).piece_enum };

            const auto & walkablePieces{ pieceMoving.walkablePieces() };

            const bool canWalkOnTargetPiece{
                std::find(std::begin(walkablePieces), std::end(walkablePieces), pieceTargetEnum) !=
                std::end(walkablePieces)
            };

            if (!canWalkOnTargetPiece)
            {
                return;
            }

            if (pieceTargetEnum != Piece::Count)
            {
                return;
            }

            switch (pieceMovingEnum)
            {
                case Piece::Player: {
                    move(context, dynamic_cast<PlayerPiece &>(pieceMoving), targetPos);
                    break;
                }
                case Piece::Demon: {
                    move(context, dynamic_cast<DemonPiece &>(pieceMoving), targetPos);
                    break;
                }
                case Piece::Victim: {
                    move(context, dynamic_cast<VictimPiece &>(pieceMoving), targetPos);
                    break;
                }

                case Piece::Wall:
                case Piece::Count:
                default: break;
            }
        }

        template <typename Piece_t>
        static void move(Context & context, Piece_t & pieceMoving, const BoardPos_t & targetPos)
        {
            Cell & fromCell{ context.board.cellAt(pieceMoving.boardPos()) };
            Cell & toCell{ context.board.cellAt(targetPos) };

            fromCell.piece_enum = Piece::Count;
            toCell.piece_enum = pieceMoving.piece();

            pieceMoving.m_boardPos = targetPos;
            pieceMoving.m_sprite.setPosition(toCell.window_rect_center);
        }
    };
} // namespace boardgame

#endif // BOARDGAME_COLLIDER_HPP_INCLUDED