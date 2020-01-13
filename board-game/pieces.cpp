// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// meth-head.cpp
//
#include "pieces.hpp"

#include "board.hpp"
#include "random.hpp"
#include "util.hpp"

namespace boardgame
{
    // TODO this just picks the first, so eventually it will hae to do something smarter
    BoardPos_t SeekerPieceBase::pickTarget(const Context & context)
    {
        for (const IPieceUPtr_t & piece : context.board.pieces)
        {
            if (piece->piece() == m_targetPiece)
            {
                return piece->boardPos();
            }
        }

        // default to no target
        return boardPos();
    }

    BoardPos_t SeekerPieceBase::takeTurn(const Context & context)
    {
        m_targetBoardPos = pickTarget(context);

        WalkTargetArray_t moves{ makePossibleMovesArray(context, boardPos()) };
        auto moveEndIter{ findAllPossibleMoves(context, moves) };
        return pickMoveClosestToTarget(context, moves, moveEndIter);
    }

    WalkTargetArray_t::iterator
        SeekerPieceBase::findAllPossibleMoves(const Context & context, WalkTargetArray_t & moves)
    {
        // start with all four possible direction moves

        // remove any off-board moves
        auto movesEndIter =
            std::remove_if(std::begin(moves), std::end(moves), [&](const WalkTarget & target) {
                return !context.board.isPosValid(target.position);
            });

        // remove any that cannot be walked on
        movesEndIter =
            std::remove_if(std::begin(moves), movesEndIter, [&](const WalkTarget & target) {
                return (
                    std::find(
                        std::begin(m_walkablePieces), std::end(m_walkablePieces), target.piece) ==
                    std::end(m_walkablePieces));
            });

        return movesEndIter;
    }

    BoardPos_t SeekerPieceBase::pickMoveClosestToTarget(
        const Context & context,
        WalkTargetArray_t & moves,
        WalkTargetArray_t::iterator & movesEndIter)
    {
        // bail if no valid moves remain
        if (std::begin(moves) == movesEndIter)
        {
            return boardPos();
        }

        // if there is only one possible move, then move there and bail
        if ((std::begin(moves) + 1) == movesEndIter)
        {
            return moves.front().position;
        }

        // calc how far each move is from the target
        for (auto iter(std::begin(moves)); iter != movesEndIter; ++iter)
        {
            iter->distance = Board::walkDistance(iter->position, m_targetBoardPos);
        }

        // find the shortest distance among all remaining moves
        const int shortestWalkDist = std::min_element(
                                         std::begin(moves),
                                         movesEndIter,
                                         [&](const WalkTarget & left, const WalkTarget & right) {
                                             return (left.distance < right.distance);
                                         })
                                         ->distance;

        // remove any moves that are farther than the shortest distance
        movesEndIter =
            std::remove_if(std::begin(moves), movesEndIter, [&](const WalkTarget & posDist) {
                return posDist.distance > shortestWalkDist;
            });

        // random select any remaining move
        return context.random.from(std::begin(moves), movesEndIter).position;
    }
} // namespace boardgame