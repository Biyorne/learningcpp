// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// meth-head.cpp
//
#include "meth-head.hpp"

#include "error-handling.hpp"
#include "utils.hpp"

#include <cassert>
#include <iostream>

namespace methhead
{
    MethHeadBase::MethHeadBase(const SimContext & context)
        : ScopedBoardPosition(context)
        , m_score(0)
        , m_turnDelaySec(s_turnDelayDefaultSec)
        , m_turnDelaySoFarSec(0.0f)
    {}

    void MethHeadBase::update(const SimContext & context, const float elapsedSec)
    {
        assert(context.isPickupAt(boardPos()) == false);
        assert(context.isActorAt(boardPos()) == true);

        if (isTimeToMove(elapsedSec))
        {
            move(context);
        }
    }

    bool MethHeadBase::isTimeToMove(const float elapsedSec) noexcept
    {
        m_turnDelaySoFarSec += elapsedSec;

        if (m_turnDelaySoFarSec > m_turnDelaySec)
        {
            m_turnDelaySoFarSec = (m_turnDelaySoFarSec - m_turnDelaySec);
            return true;
        }

        return false;
    }

    // TODO Nel:
    //  Look closely at the if statements below.   Where are errors most likely to be?  Are there
    //  any?
    //
    // Note that each if will be considered every time this function runs.  Is that required, or is
    // it possible to optimize by skipping some of those ifs some of the time?
    std::vector<BoardPos_t>
        MethHeadBase::makeAllPossibleBoardMoves(const SimContext & context) const
    {
        const BoardPos_t currentPos{ boardPos() };

        std::vector<BoardPos_t> moves;

        if (currentPos.x > 0)
        {
            moves.push_back(currentPos + sf::Vector2i(-1, 0));
        }

        if (currentPos.x < (context.display.cell_countsI.x - 1))
        {
            moves.push_back(currentPos + sf::Vector2i(1, 0));
        }

        if (currentPos.y > 0)
        {
            moves.push_back(currentPos + sf::Vector2i(0, -1));
        }

        if (currentPos.y < (context.display.cell_countsI.y - 1))
        {
            moves.push_back(currentPos + sf::Vector2i(0, 1));
        }

        moves.erase(
            std::remove_if(
                std::begin(moves),
                std::end(moves),
                [&context](const BoardPos_t & possiblePos) {
                    return context.isActorAt(possiblePos);
                }),
            std::end(moves));

        return moves;
    }

    bool MethHeadBase::move(const SimContext & context)
    {
        if (context.pickups.empty())
        {
            return false;
        }

        const auto targetBoardPos(findMostDesiredPickupBoardPos(context));

        std::vector<BoardPos_t> possibleMoves(makeAllPossibleBoardMoves(context));
        if (possibleMoves.empty())
        {
            // std::cout << "Methhead cannot move because other methheads are in the way."
            //          << std::endl;
            return false;
        }

        std::sort(
            std::begin(possibleMoves),
            std::end(possibleMoves),
            [&](const BoardPos_t & left, const BoardPos_t & right) {
                return (
                    walkDistanceBetween(targetBoardPos, left) <
                    walkDistanceBetween(targetBoardPos, right));
            });

        const int closestToTargetDistance{ walkDistanceBetween(
            targetBoardPos, possibleMoves.front()) };

        possibleMoves.erase(
            std::remove_if(
                std::begin(possibleMoves),
                std::end(possibleMoves),
                [&](const BoardPos_t & pos) {
                    return (walkDistanceBetween(targetBoardPos, pos) > closestToTargetDistance);
                }),
            std::end(possibleMoves));

        assert(!possibleMoves.empty());

        if (possibleMoves.empty())
        {
            std::cout << "Methhead cannot move because of unknown error." << std::endl;
            return false;
        }

        ScopedBoardPosition::set(context, context.random.from(possibleMoves));
        return true;
    }
} // namespace methhead