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
    MethHeadBase::MethHeadBase(const BoardPos_t & boardPos, const float waitBetweenActionsSec)
        : m_score(0)
        , m_boardPos(boardPos)
        , m_waitBetweenActionsSec(waitBetweenActionsSec)
        , m_elapsedSinceLastActionSec(0.0f)
    {}

    void MethHeadBase::update(const float elapsedSec, const SimContext & context)
    {
        assert(context.isPickupAt(m_boardPos) == false);
        assert(context.isActorAt(m_boardPos) == true);

        if (isTimeToMove(elapsedSec))
        {
            move(context);
        }
    }

    bool MethHeadBase::isTimeToMove(const float elapsedSec)
    {
        m_elapsedSinceLastActionSec += elapsedSec;

        if (m_elapsedSinceLastActionSec > m_waitBetweenActionsSec)
        {
            // TODO Nell -what is this line doing?  Why not reset to zero?
            m_elapsedSinceLastActionSec = (m_elapsedSinceLastActionSec - m_waitBetweenActionsSec);
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
        std::vector<BoardPos_t> moves;

        if (m_boardPos.x > 0)
        {
            moves.push_back(m_boardPos + sf::Vector2i(-1, 0));
        }

        if (m_boardPos.x < (context.display.cell_countsI.x - 1))
        {
            moves.push_back(m_boardPos + sf::Vector2i(1, 0));
        }

        if (m_boardPos.y > 0)
        {
            moves.push_back(m_boardPos + sf::Vector2i(0, -1));
        }

        if (m_boardPos.y < (context.display.cell_countsI.y - 1))
        {
            moves.push_back(m_boardPos + sf::Vector2i(0, 1));
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

    void MethHeadBase::move(const SimContext & context)
    {
        const auto targetBoardPosOpt(findMostDesiredPickupBoardPos(context));
        if (!targetBoardPosOpt)
        {
            // std::cout << "Methhead could not move: Failed to find pickup." << std::endl;
            return;
        }

        std::vector<BoardPos_t> possibleMoves(makeAllPossibleBoardMoves(context));
        if (possibleMoves.empty())
        {
            // std::cout << "Methhead cannot move because other methheads are in the way."
            //          << std::endl;
            return;
        }

        std::sort(
            std::begin(possibleMoves),
            std::end(possibleMoves),
            [&](const BoardPos_t & left, const BoardPos_t & right) {
                return (
                    walkDistanceBetween(targetBoardPosOpt.value(), left) <
                    walkDistanceBetween(targetBoardPosOpt.value(), right));
            });

        const int closestToTargetDistance{ walkDistanceBetween(
            targetBoardPosOpt.value(), possibleMoves.front()) };

        possibleMoves.erase(
            std::remove_if(
                std::begin(possibleMoves),
                std::end(possibleMoves),
                [&](const BoardPos_t & pos) {
                    return (
                        walkDistanceBetween(targetBoardPosOpt.value(), pos) >
                        closestToTargetDistance);
                }),
            std::end(possibleMoves));

        assert(!possibleMoves.empty());

        if (possibleMoves.empty())
        {
            std::cout << "Methhead cannot move because of unknown error." << std::endl;
            return;
        }

        m_boardPos = context.random.from(possibleMoves);
    }
} // namespace methhead