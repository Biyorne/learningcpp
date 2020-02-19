// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// meth-head.cpp
//
#include "pieces.hpp"

#include "board.hpp"
#include "random.hpp"
#include "sound-player.hpp"
#include "util.hpp"

#include <cassert>
#include <functional>

namespace boardgame
{
    void PieceBase::move(Context & context, const BoardPos_t & posNew)
    {
        M_ASSERT_OR_THROW(m_hasSetup);
        M_ASSERT_OR_THROW(!m_hasTeardown);

        M_ASSERT_OR_THROW(posNew != position());

        context.board.removePiece(context, posNew);

        // we don't use Board::placePiece() because our Setup() has already been called
        m_position = posNew;
        util::centerInside(m_sprite, context.board.cells().bounds(posNew));

        M_ASSERT_OR_THROW(context.board.pieceEnumAt(posNew) == m_enum);
    }

    void HeadPiece::handleEvent(Context & context, const sf::Event & event)
    {
        if (sf::Event::KeyPressed != event.type)
        {
            return;
        }

        if (util::isArrowKey(event.key.code))
        {
            if (m_directionKeyNext != event.key.code)
            {
                context.audio.play("tap-1-a.ogg");
                m_directionKeyNext = event.key.code;
            }

            return;
        }

        // if (sf::Keyboard::F == event.key.code)
        //{
        //    if (flip(context))
        //    {
        //        context.settings.adjustScore(-100);
        //    }
        //}
    }

    bool HeadPiece::flip(Context &)
    {
        return false;
        // const auto [didFlip, newHeadPos, newDirection] = TailPiece::flip(context);
        // if (!didFlip)
        //{
        //    return false;
        //}
        //
        // context.audio.play("bounce");
        // move(context, newHeadPos);
        // m_directionKeyNext = newDirection;
        // m_directionKeyPrev = newDirection;
        //
        // return true;
    }

    void HeadPiece::update(Context & context, const float frameTimeSec)
    {
        changeDirectionIfArrowKeyIsPressed(context);

        m_timeElapsedSinceLastTurnSec += frameTimeSec;
        if (m_timeElapsedSinceLastTurnSec > context.settings.timeBetweenTurnsSec())
        {
            m_timeElapsedSinceLastTurnSec -= context.settings.timeBetweenTurnsSec();

            takeTurn(context);
        }
    }

    void HeadPiece::changeDirectionIfArrowKeyIsPressed(Context & context)
    {
        const auto before{ m_directionKeyNext };

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            m_directionKeyNext = sf::Keyboard::Up;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            m_directionKeyNext = sf::Keyboard::Down;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            m_directionKeyNext = sf::Keyboard::Left;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            m_directionKeyNext = sf::Keyboard::Right;
        }

        if (m_directionKeyNext != before)
        {
            context.audio.play("tap-1-a.ogg");
        }
    }

    void HeadPiece::takeTurn(Context & context)
    {
        ++context.settings.total_turns_played;

        // undo the change in direction if it would reverse direction == instant death
        if (util::oppositeDirection(m_directionKeyNext) == m_directionKeyPrev)
        {
            m_directionKeyNext = m_directionKeyPrev;
        }

        m_directionKeyPrev = m_directionKeyNext;

        const BoardPos_t posOld{ m_position };
        const BoardPos_t posNew{ changePositionWithArrowKey(m_position, m_directionKeyNext) };
        M_ASSERT_OR_THROW(posOld != posNew);

        const bool didMissFood{ isPositionNextToFood(context, posOld) &&
                                !isPositionNextToFood(context, posNew) };

        const Piece::Enum posNewPieceEnum{ context.board.pieceEnumAt(posNew) };

        // move head
        move(context, posNew);
        M_ASSERT_OR_THROW(context.board.pieceEnumAt(posNew) == Piece::Head);
        M_ASSERT_OR_THROW(context.board.pieceEnumAt(posOld) == Piece::Count);

        // place a new tail piece behind the head
        context.board.placePiece(context, Piece::Tail, posOld);

        if (context.settings.isTailGrowing() || (posNewPieceEnum == Piece::Food))
        {
            context.settings.adjustScore(static_cast<int>(TailPiece::tailLength()) + 1);
        }
        else
        {
            TailPiece::removeLastTailPiece(context);
        }

        // handle whatever we just ate
        if (posNewPieceEnum == Piece::Count)
        {
            if (didMissFood)
            {
                // context.settings.adjustScore(-5);
                context.audio.play("miss");
            }
        }
        else if (posNewPieceEnum == Piece::Food)
        {
            context.audio.play("shine", m_eatSfxPitch);
            m_eatSfxPitch += m_eatSfxPitchAdj;

            context.settings.adjustScore(50);
            context.settings.handleEat();
        }
        else if (posNewPieceEnum == Piece::Tail)
        {
            // if (context.settings.will_eating_tail_turn_it_into_wall)
            //{
            //    context.audio.play("mario-break-block");
            //    TailPiece::turnSeveredTailIntoWall(context, posNew);
            //}
            // else
            {
                handleDeathFromCollision(context, posNewPieceEnum);
            }
        }
        else if (posNewPieceEnum == Piece::Wall)
        {
            handleDeathFromCollision(context, posNewPieceEnum);
        }
        else if (posNewPieceEnum == Piece::Head)
        {
            M_ASSERT_OR_THROW(posNewPieceEnum != Piece::Head);
        }
    }

    void HeadPiece::handleDeathFromCollision(Context & context, const Piece::Enum which)
    {
        context.audio.play("cannon-miss");

        std::cout << "You bit into " << Piece::name(which);

        if (context.settings.is_god_mode)
        {
            std::cout << " -but god mode saved you." << std::endl;
            return;
        }

        context.audio.play("rpg-game-over");
        m_sprite.setColor(sf::Color::Red);
        context.settings.is_game_over = true;
        std::cout << "!  YOU LOSE!" << std::endl;
    }

    bool HeadPiece::isPositionNextToFood(Context & context, const BoardPos_t & pos)
    {
        const auto abovePos{ changePositionWithArrowKey(pos, sf::Keyboard::Up) };
        const auto belowPos{ changePositionWithArrowKey(pos, sf::Keyboard::Down) };
        const auto leftPos{ changePositionWithArrowKey(pos, sf::Keyboard::Left) };
        const auto rightPos{ changePositionWithArrowKey(pos, sf::Keyboard::Right) };

        const auto isFoodAbove{ context.board.pieceEnumAt(abovePos) == Piece::Food };
        const auto isFoodBelow{ context.board.pieceEnumAt(belowPos) == Piece::Food };
        const auto isFoodLeft{ context.board.pieceEnumAt(leftPos) == Piece::Food };
        const auto isFoodRight{ context.board.pieceEnumAt(rightPos) == Piece::Food };

        return (isFoodAbove || isFoodBelow || isFoodLeft || isFoodRight);
    }

    BoardPos_t HeadPiece::changePositionWithArrowKey(
        const BoardPos_t & oldPos, const sf::Keyboard::Key key)
    {
        BoardPos_t newPos{ oldPos };

        if (sf::Keyboard::Up == key)
        {
            --newPos.y;
        }
        else if (sf::Keyboard::Down == key)
        {
            ++newPos.y;
        }
        else if (sf::Keyboard::Left == key)
        {
            --newPos.x;
        }
        else if (sf::Keyboard::Right == key)
        {
            ++newPos.x;
        }

        return newPos;
    }
} // namespace boardgame