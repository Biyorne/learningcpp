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
        : BoardPositionHandler_t(context)
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

    void MethHeadBase::makeAllPossibleBoardMoves(const SimContext & context) const
    {
        const BoardPos_t currentPos{ boardPos() };

        m_possibleMoves.clear();

        // clang-format off
        if (currentPos.x > 0)                                     { m_possibleMoves.push_back(currentPos + sf::Vector2i(-1,  0)); }
        if (currentPos.x < (context.display.cell_countsI.x - 1))  { m_possibleMoves.push_back(currentPos + sf::Vector2i( 1,  0)); }
        if (currentPos.y > 0)                                     { m_possibleMoves.push_back(currentPos + sf::Vector2i( 0, -1)); }
        if (currentPos.y < (context.display.cell_countsI.y - 1))  { m_possibleMoves.push_back(currentPos + sf::Vector2i( 0,  1)); }
        // clang-format on

        m_possibleMoves.erase(
            std::remove_if(
                std::begin(m_possibleMoves),
                std::end(m_possibleMoves),
                [&context](const BoardPos_t & possiblePos) {
                    return context.isActorAt(possiblePos);
                }),
            std::end(m_possibleMoves));
    }

    bool MethHeadBase::move(const SimContext & context)
    {
        if (context.pickups.empty())
        {
            return false;
        }

        const auto targetBoardPos(findMostDesiredPickupBoardPos(context));

        makeAllPossibleBoardMoves(context);
        if (m_possibleMoves.empty())
        {
            // std::cout << "Methhead cannot move because other methheads are in the way."
            //          << std::endl;
            return false;
        }

        std::sort(
            std::begin(m_possibleMoves),
            std::end(m_possibleMoves),
            [&](const BoardPos_t & left, const BoardPos_t & right) {
                return (
                    walkDistanceBetween(targetBoardPos, left) <
                    walkDistanceBetween(targetBoardPos, right));
            });

        const int closestToTargetDistance{ walkDistanceBetween(
            targetBoardPos, m_possibleMoves.front()) };

        m_possibleMoves.erase(
            std::remove_if(
                std::begin(m_possibleMoves),
                std::end(m_possibleMoves),
                [&](const BoardPos_t & pos) {
                    return (walkDistanceBetween(targetBoardPos, pos) > closestToTargetDistance);
                }),
            std::end(m_possibleMoves));

        assert(!m_possibleMoves.empty());

        if (m_possibleMoves.empty())
        {
            std::cout << "Methhead cannot move because of unknown error." << std::endl;
            return false;
        }

        BoardPositionHandler_t::set(context, context.random.from(m_possibleMoves));
        return true;
    }
} // namespace methhead