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

#include <set>

namespace methhead
{
    MethHeadBase::MethHeadBase(
        const std::string & imagePath,
        const BoardPos_t & boardPos,
        const sf::FloatRect & windowRect,
        const float waitBetweenActionsSec)
        : m_score(0)
        , m_texture()
        , m_sprite()
        , m_boardPos(boardPos)
        , m_waitBetweenActionsSec(waitBetweenActionsSec)
        , m_elapsedSinceLastActionSec(0.0f)
    {
        if (!m_texture.loadFromFile(imagePath))
        {
            throw std::runtime_error("Unable to load image: " + imagePath);
        }

        m_sprite.setTexture(m_texture, true);
        fit(m_sprite, windowRect);
    }

    void MethHeadBase::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        // TODO try skipping draw if off-screen
        target.draw(m_sprite, states);
    }

    bool MethHeadBase::update(const float elapsedSec, const ActorContext & context)
    {
        assert(context.isPickupAtBoardPos(m_boardPos) == false);
        assert(context.isActorAtBoardPos(m_boardPos) == true);

        if (isTimeToMove(elapsedSec))
        {
            return move(context);
        }

        return false;
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
        MethHeadBase::makeAllBoardPosMovesToward(const BoardPos_t & targetBoardPos) const
    {
        std::vector<BoardPos_t> moves;

        if (targetBoardPos.x < m_boardPos.x)
        {
            moves.push_back(m_boardPos + sf::Vector2i(-1, 0));
        }

        if (targetBoardPos.x > m_boardPos.x)
        {
            moves.push_back(m_boardPos + sf::Vector2i(1, 0));
        }

        if (targetBoardPos.y < m_boardPos.y)
        {
            moves.push_back(m_boardPos + sf::Vector2i(0, -1));
        }

        if (targetBoardPos.y > m_boardPos.y)
        {
            moves.push_back(m_boardPos + sf::Vector2i(0, 1));
        }

        return moves;
    }

    bool MethHeadBase::move(const ActorContext & context)
    {
        const BoardPos_t targetPickupBoardPos(findMostDesiredPickupBoardPos(context));

        if (targetPickupBoardPos == m_boardPos)
        {
            std::cout << "Methhead could not move because there are no pickups." << std::endl;
            assert(context.pickups.empty());
            return false;
        }

        const std::vector<BoardPos_t> possibleMoves(
            makeAllBoardPosMovesToward(targetPickupBoardPos));

        if (possibleMoves.empty())
        {
            std::cout << "Methhead cannot move because other methheads are in the way."
                      << std::endl;

            return false;
        }

        m_boardPos = context.random.from(possibleMoves);

        // TODO we don't need to scale each time...only position...
        fit(m_sprite, context.display.boardPosToWindowRect(m_boardPos));

        return true;
    }
} // namespace methhead