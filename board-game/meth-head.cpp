// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// meth-head.cpp
//
#include "meth-head.hpp"

#include "display-constants.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "util.hpp"

#include <array>
#include <cassert>
#include <iostream>

namespace boardgame
{
    BoardPos_t PieceBase::takeTurn(const Context & context)
    {
        if (boardPos() == m_targetBoardPos)
        {
            const BoardPos_t newPos{ pickTarget(context) };

            // bail if no new target was found,
            if (boardPos() == newPos)
            {
                return boardPos();
            }
        }

        return pickPossibleMove(context, findAllPossibleMoves(context));
    }

    MovesArray_t::iterator PieceBase::findAllPossibleMoves(const Context & context)
    {
        const BoardPos_t currPos{ boardPos() };
        const sf::Vector2i cellCounts{ context.display.cell_countsI };

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
                const BoardPos_t & pos{ walkDist.pos };
                return (
                    (pos.x < 0) || (pos.y < 0) || (pos.x >= cellCounts.x) ||
                    (pos.y >= cellCounts.y));
            });

        // remove any moves already taken by anything other than our target piece
        movesEndIter =
            std::remove_if(std::begin(m_moves), movesEndIter, [&](const WalkDistance & walkDist) {
                const Piece piece{ ScopedBoardPosition::pieceAt(context, walkDist.pos) };
                return ((piece != Piece::None) && (piece != m_targetPiece));
            });

        return movesEndIter;
    }

    BoardPos_t
        PieceBase::pickPossibleMove(const Context & context, MovesArray_t::iterator movesEndIter)
    {
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

        assert(std::distance(std::begin(m_moves), movesEndIter) > 1);

        // calc how far each move is from the target
        for (auto iter(std::begin(m_moves)); iter != movesEndIter; ++iter)
        {
            iter->dist = walkDistance(iter->pos, m_targetBoardPos);
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

        // random select one
        const BoardPos_t newPos{ context.random.from(std::begin(m_moves), movesEndIter).pos };

        return newPos;
    }
} // namespace boardgame