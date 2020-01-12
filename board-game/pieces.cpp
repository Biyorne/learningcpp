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
    std::string PieceBase::toString() const
    {
        std::ostringstream ss;
        ss << Piece::name(m_piece) << "Piece" << boardPos()
           << ", global=" << m_sprite.getGlobalBounds() << ", local=" << m_sprite.getLocalBounds();
        return ss.str();
    }

    std::string SeekerPieceBase::toString() const
    {
        std::ostringstream ss;
        ss << "Seeking{" << Piece::name(m_targetPiece) << "at" << m_targetBoardPos << "}";
        return (PieceBase::toString() + ss.str());
    }

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
        // if has no target the try to find one
        if (boardPos() == m_targetBoardPos)
        {
            m_targetBoardPos = pickTarget(context);

            // bail if no new target was found
            if (boardPos() == m_targetBoardPos)
            {
                return boardPos();
            }
        }

        const auto movesEndIter{ findAllPossibleMoves(context) };

        // bail if no valid moves remain
        if (std::begin(m_moves) == movesEndIter)
        {
            return boardPos();
        }

        // if there is only one possible move, then move there and bail
        if ((std::begin(m_moves) + 1) == movesEndIter)
        {
            return m_moves[0].pos;
        }

        return pickMoveClosestToTarget(context, movesEndIter);
    }

    MovesArray_t::iterator SeekerPieceBase::findAllPossibleMoves(const Context & context)
    {
        const BoardPos_t currPos{ boardPos() };

        // start with all four possible direction moves
        // clang-format off
        m_moves = { WalkDistance{ 0, (currPos + sf::Vector2i(-1,  0)) },
                    WalkDistance{ 0, (currPos + sf::Vector2i( 1,  0)) },
                    WalkDistance{ 0, (currPos + sf::Vector2i( 0, -1)) },
                    WalkDistance{ 0, (currPos + sf::Vector2i( 0,  1)) } };
        // clang-format on

        // TODO remove this for looping doors etc
        // remove any off-board moves
        auto movesEndIter = std::remove_if(
            std::begin(m_moves), std::end(m_moves), [&](const WalkDistance & walkDist) {
                return !context.board.isPosValid(walkDist.pos);
            });

        // remove any that we cannot walk on
        movesEndIter =
            std::remove_if(std::begin(m_moves), movesEndIter, [&](const WalkDistance & walkDist) {
                const Piece::Enum piece{ context.board.cellAt(walkDist.pos).piece_enum };

                return (
                    std::find(
                        std::begin(m_walkAllowedPieces), std::end(m_walkAllowedPieces), piece) ==
                    std::end(m_walkAllowedPieces));
            });

        // remove any moves already taken by anything other than our target piece
        movesEndIter =
            std::remove_if(std::begin(m_moves), movesEndIter, [&](const WalkDistance & walkDist) {
                const Piece::Enum piece{ context.board.cellAt(walkDist.pos).piece_enum };
                return ((piece != Piece::Count) && (piece != m_targetPiece));
            });

        return movesEndIter;
    }

    BoardPos_t SeekerPieceBase::pickMoveClosestToTarget(
        const Context & context, MovesArray_t::iterator movesEndIter)
    {
        assert(std::distance(std::begin(m_moves), movesEndIter) > 1);

        // calc how far each move is from the target
        for (auto iter(std::begin(m_moves)); iter != movesEndIter; ++iter)
        {
            iter->dist = Board::walkDistance(iter->pos, m_targetBoardPos);
        }

        // find the shortest distance among all remaining moves
        const int shortestWalkDist =
            std::min_element(
                std::begin(m_moves),
                movesEndIter,
                [&](const WalkDistance & left, const WalkDistance & right) {
                    return (left.dist < right.dist);
                })
                ->dist;

        // remove any moves that are farther than the shortest distance
        movesEndIter =
            std::remove_if(std::begin(m_moves), movesEndIter, [&](const WalkDistance & posDist) {
                return posDist.dist > shortestWalkDist;
            });

        // random select any remaining move
        return context.random.from(std::begin(m_moves), movesEndIter).pos;
    }
} // namespace boardgame