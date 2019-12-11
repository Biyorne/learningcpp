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
    ActorBase::ActorBase(const SimContext & context)
        : ScopedBoardPosHandler(context)
        , m_score(0)
        , m_moveDelaySec(s_turnDelayDefaultSec)
        , m_turnDelaySoFarSec(0.0f)
    {}

    void ActorBase::update(const SimContext & context, const float elapsedSec)
    {
        if (!isTimeToMove(elapsedSec))
        {
            return;
        }

        move(context);
    }

    bool ActorBase::isTimeToMove(const float elapsedSec) noexcept
    {
        m_turnDelaySoFarSec += elapsedSec;

        if (m_turnDelaySoFarSec > m_moveDelaySec)
        {
            m_turnDelaySoFarSec = (m_turnDelaySoFarSec - m_moveDelaySec);
            return true;
        }

        return false;
    }

    void ActorBase::move(const SimContext & context)
    {
        if (context.pickups.empty())
        {
            return;
        }

        WalkDIstVec_t::iterator endIter = findAllPossibleMoves(context);

        // NOTE: This ensures tha m_moves is NOT empty, which would cause crashes below.
        if (std::begin(m_moves) == endIter)
        {
            return;
        }

        assert(std::distance(std::begin(m_moves), endIter) > 0);

        // handle the case where there is only one possible move
        if ((std::begin(m_moves) + 1) == endIter)
        {
            ScopedBoardPosHandler::setPos(context, m_moves[0].pos);
            return;
        }

        assert(std::distance(std::begin(m_moves), endIter) > 1);

        setPos(context, findBestMoveTowardTarget(context, endIter));
    }

    WalkDIstVec_t::iterator ActorBase::findAllPossibleMoves(const SimContext & context)
    {
        m_moves.clear();

        const BoardPos_t pos{ boardPos() };
        const sf::Vector2i counts{ context.display.cell_countsI };

        // clang-format off
        if (pos.x > 0)              m_moves.push_back({(pos + sf::Vector2i(-1,  0)), 0 });
        if (pos.x < (counts.x - 1)) m_moves.push_back({(pos + sf::Vector2i( 1,  0)), 0 });
        if (pos.y > 0)              m_moves.push_back({(pos + sf::Vector2i( 0, -1)), 0 });
        if (pos.y < (counts.y - 1)) m_moves.push_back({(pos + sf::Vector2i( 0,  1)), 0 });
        // clang-format on

        return std::remove_if(
            std::begin(m_moves), std::end(m_moves), [&](const WalkDistance & walkDist) {
                if (isPosFree(context, walkDist.pos))
                {
                    return false;
                }

                const auto foundIter = std::find_if(
                    std::begin(context.actors),
                    std::end(context.actors),
                    [&](const IActorUPtr_t & actor) {
                        return (walkDist.pos == actor->boardPos());
                    });

                return foundIter != std::end(context.actors);
            });
    }

    BoardPos_t ActorBase::findBestMoveTowardTarget(
        const SimContext & context, WalkDIstVec_t::iterator & endIter)
    {
        assert(std::distance(std::begin(m_moves), endIter) > 1);

        const BoardPos_t targetPos{ findTargetPos(context) };

        assert(targetPos != boardPos());

        int shortestWalkingDistance{ std::numeric_limits<int>::max() };
        for (auto iter(std::begin(m_moves)); iter != endIter; ++iter)
        {
            iter->dist = walkDistance(iter->pos, targetPos);

            if (iter->dist < shortestWalkingDistance)
            {
                shortestWalkingDistance = iter->dist;
            }
        }

        endIter = std::remove_if(std::begin(m_moves), endIter, [&](const WalkDistance & posDist) {
            return posDist.dist > shortestWalkingDistance;
        });

        const auto beginIter{ std::begin(m_moves) };
        BoardPos_t newPos{ beginIter->pos };

        if ((beginIter + 2) == endIter)
        {
            if (context.random.boolean())
            {
                newPos = m_moves[1].pos;
            }
        }
        else if ((beginIter + 3) == endIter)
        {
            newPos = m_moves[context.random.fromTo(0_st, 2_st)].pos;
        }

        return newPos;
    }
} // namespace methhead