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
        : ScopedBoardPosition(context)
        , m_targetBoardPos(0, 0)
        , m_score(0)
        , m_moveDelaySec(s_turnDelayDefaultSec)
        , m_turnDelaySoFarSec(0.0f)
    {}

    bool ActorBase::update(const SimContext & context, const float elapsedSec)
    {
        if (!isTimeToMove(elapsedSec))
        {
            return false;
        }

        return move(context);
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

    bool ActorBase::move(const SimContext & context)
    {
        if (context.pickups.empty())
        {
            return false;
        }

        if (boardPos() == m_targetBoardPos)
        {
            pickTarget(context);
        }

        const BoardPos_t currPos{ boardPos() };
        const sf::Vector2i counts{ context.display.cell_countsI };

        // start with all four possible direction moves
        // clang-format off
        m_moves.resize(4);
        m_moves[0].pos = (currPos + sf::Vector2i(-1,  0));
        m_moves[1].pos = (currPos + sf::Vector2i( 1,  0));
        m_moves[2].pos = (currPos + sf::Vector2i( 0, -1));
        m_moves[3].pos = (currPos + sf::Vector2i( 0,  1));
        // clang-format on

        // remove any off-board moves
        WalkDIstVec_t::iterator endIter = std::remove_if(
            std::begin(m_moves), std::end(m_moves), [&](const WalkDistance & walkDist) {
                const BoardPos_t & pos{ walkDist.pos };
                return ((pos.x < 0) || (pos.y < 0) || (pos.x >= counts.x) || (pos.y >= counts.y));
            });

        // remove any moves already taken by other actors
        endIter = std::remove_if(std::begin(m_moves), endIter, [&](const WalkDistance & walkDist) {
            if (isPosFree(context, walkDist.pos))
            {
                return false;
            }

            const auto foundIter = std::find_if(
                std::begin(context.actors),
                std::end(context.actors),
                [&](const IActorUPtr_t & actor) { return (walkDist.pos == actor->boardPos()); });

            return foundIter != std::end(context.actors);
        });

        // bail if no valid moves remain
        if (std::begin(m_moves) == endIter)
        {
            return false;
        }

        // finish here if there is only one possible move
        if ((std::begin(m_moves) + 1) == endIter)
        {
            ScopedBoardPosition::set(context, m_moves[0].pos);
            return true;
        }

        assert(std::distance(std::begin(m_moves), endIter) > 1);

        // calc how far each move is from the target
        for (auto iter(std::begin(m_moves)); iter != endIter; ++iter)
        {
            iter->dist = walkDistance(iter->pos, m_targetBoardPos);
        }

        // find the shortest distance among all remaining moves
        const int shortestWalkDist =
            std::min_element(
                std::begin(m_moves),
                endIter,
                [&](const WalkDistance & left, const WalkDistance & right) {
                    return (left.dist < right.dist);
                })
                ->dist;

        // remove any moves that are farther than the shortest distance
        endIter = std::remove_if(std::begin(m_moves), endIter, [&](const WalkDistance & posDist) {
            return posDist.dist > shortestWalkDist;
        });

        // random select one
        const BoardPos_t newPos{ context.random.from(std::begin(m_moves), endIter).pos };

        ScopedBoardPosition::set(context, newPos);
        return true;
    }
} // namespace methhead