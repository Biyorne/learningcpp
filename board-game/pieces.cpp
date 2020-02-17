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
        M_ASSERT_OR_THROW(posNew != position());

        context.board.removeFromPlay(posNew);

        m_position = posNew;
        util::centerInside(m_sprite, context.board.cells().bounds(posNew));

        M_ASSERT_OR_THROW(context.board.pieceEnumAt(posNew) == m_enum);
    }

    void HeadPiece::handleEvent(Context & context, const sf::Event & event)
    {
        if ((sf::Event::KeyPressed != event.type) || !util::isArrowKey(event.key.code))
        {
            return;
        }

        if (m_directionKeyNext != event.key.code)
        {
            context.audio.play("tap-1-a.ogg");
            m_directionKeyNext = event.key.code;
        }
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
            context.audio.play("bounce-small-deep");
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
        M_ASSERT_OR_THROW(context.board.pieceEnumAt(posOld) == Piece::NotInPlay);

        // place a new tail piece behind the head
        context.board.addPiece(context, Piece::Tail, posOld);
        M_ASSERT_OR_THROW(context.board.pieceEnumAt(posOld) == Piece::Tail);

        // handle whatever we just ate
        if (posNewPieceEnum == Piece::Food)
        {
            // const float speedDifficultyRatio{ context.settings.speedDifficultyRatio() };
            static float pitch{ 0.5f };
            std::cout << "pitch=" << pitch << std::endl;
            context.audio.play("shine", pitch);
            pitch += 0.025f;

            context.settings.handleEat();
            FoodPiece::spawn(context);
        }
        else if (posNewPieceEnum == Piece::NotInPlay)
        {
            if (didMissFood)
            {
                context.audio.play("miss");
            }
        }
        else
        {
            context.audio.play("cannon-miss");
            context.audio.play("rpg-game-over");
            m_sprite.setColor(sf::Color::Red);

            context.settings.is_game_over = true;

            std::cout << "You bit into " << Piece::name(posNewPieceEnum) << "!  YOU LOSE!"
                      << std::endl;
        }

        // remove the last tail if not growing
        if (!context.settings.is_game_over && !context.settings.isTailGrowing())
        {
            TailPiece::removeLastPiece(context);
        }
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

    /*

    // void PieceBase::move(const Context & context, const BoardPos_t & targetPos)
    //{
    //    M_ASSERT_OR_THROW(isInPlay());
    //    M_ASSERT_OR_THROW(context.map.isPosValid(targetPos));
    //    M_ASSERT_OR_THROW(targetPos != m_boardPos);
    //
    //    m_boardPos = targetPos;
    //    util::centerInside(m_sprite, context.board.cellBounds(targetPos));
    //}
    //
    // void PieceBase::movesVec_ResetToAllFourDireections(
    //    const Context & context, const BoardPos_t & fromPos)
    //{
    //    m_moves.clear();
    //
    //    // clang-format off
    //    m_moves.push_back( fromPos + sf::Vector2i(-1,  0) );
    //    m_moves.push_back( fromPos + sf::Vector2i( 1,  0) );
    //    m_moves.push_back( fromPos + sf::Vector2i( 0, -1) );
    //    m_moves.push_back( fromPos + sf::Vector2i( 0,  1) );
    //    // clang-format on
    //}
    //
    // void PieceBase::movesVec_RemoveAllInvalid(
    //    const Context & context, const Piece::Enum pieceThatisMoving)
    //{
    //    M_ASSERT_OR_THROW(m_moves.size() == 4);
    //
    //    // remove all invalid/off-board
    //    m_moves.erase(
    //        std::remove_if(
    //            std::begin(m_moves),
    //            std::end(m_moves),
    //            [&](const BoardPos_t & pos) { return !context.map.isPosValid(pos); }),
    //        std::end(m_moves));
    //
    //    // remove all that this piece is not allowed to step on
    //    m_moves.erase(
    //        std::remove_if(
    //            std::begin(m_moves),
    //            std::end(m_moves),
    //            [&](const BoardPos_t & pos) {
    //                return !Piece::canWalkOn(pieceThatisMoving, context.);
    //            }),
    //        std::end(m_moves));
    //}
    //
    // BoardPos_t walkPosFromArrowKeypress(Context & context, const sf::Event & event)
    //{
    //    sf::Event event(eventOrig);
    //
    //    if (context.is_self_testing)
    //    {
    //        event.type = sf::Event::EventType::KeyPressed;
    //        event.key = sf::Event::KeyEvent{};
    //
    //        auto randomArrowKeyCode = [&]() {
    //            switch (context.random.zeroTo(3))
    //            {
    //                case 0: return sf::Keyboard::Left;
    //                case 1: return sf::Keyboard::Right;
    //                case 2: return sf::Keyboard::Up;
    //                default: return sf::Keyboard::Down;
    //            }
    //        };
    //
    //        event.key.code = randomArrowKeyCode();
    //    }
    //
    //    if (sf::Keyboard::Right == event.key.code)
    //    {
    //        return attemptMove(context, (m_boardPos + BoardPos_t(1, 0)));
    //    }
    //    else if (sf::Keyboard::Left == event.key.code)
    //    {
    //        return attemptMove(context, (m_boardPos + BoardPos_t(-1, 0)));
    //    }
    //    else if (sf::Keyboard::Up == event.key.code)
    //    {
    //        return attemptMove(context, (m_boardPos + BoardPos_t(0, -1)));
    //    }
    //    else if (sf::Keyboard::Down == event.key.code)
    //    {
    //        return attemptMove(context, (m_boardPos + BoardPos_t(0, 1)));
    //    }
    //}
    //
    // bool PlayerPiece::attemptMove(Context & context, const BoardPos_t & targetPos)
    //{
    //    M_ASSERT_OR_THROW(isInPlay());
    //    M_ASSERT_OR_THROW(context.map.isPosValid(targetPos));
    //    M_ASSERT_OR_THROW(targetPos != m_boardPos);
    //
    //    const Piece::Enum targetEnum{ context.board.pieceEnumAt(targetPos) };
    //
    //    // if ((Piece::Player == targetEnum) || (Piece::Wall == targetEnum) ||
    //    //    (Piece::Villan == targetEnum))
    //    //{
    //    //    return false;
    //    //}
    //
    //    if ((Piece::NotInPlay != targetEnum) && (Piece::Door != targetEnum))
    //    {
    //        return false;
    //    }
    //
    //    move(context, targetPos);
    //
    //    if (Piece::Door == targetEnum)
    //    {
    //        removeFromPlay();
    //        // std::cout << "You are free!  You WIN!" << std::endl;
    //        // context.is_game_over = true;
    //    }
    //    // else if (Piece::Villan == targetEnum)
    //    //{
    //    //    // std::cout << "You walked into a monster and lost like an idiot." <<
    std::endl;
    //    //    context.is_game_over = true;
    //    //    removeFromPlay();
    //    //}
    //
    //    return true;
    //}
    //
    ////
    //
    // BoardPosOpt_t VillanPiece::findTargetPos(const Context & context)
    //{
    //    M_ASSERT_OR_THROW(isInPlay());
    //
    //    for (const IPieceUPtr_t & piece : context.board.pieces())
    //    {
    //        if (piece->isInPlay() && (piece->which() == Piece::Player))
    //        {
    //            return piece->boardPos();
    //        }
    //    }
    //
    //    return std::nullopt;
    //}
    //
    //// always returns true because AI or Villans or Non-Players only get one chance to move
    // bool VillanPiece::takeTurn(Context & context, const sf::Event &)
    //{
    //    M_ASSERT_OR_THROW(isInPlay());
    //
    //    const BoardPosOpt_t targetPosOpt = findTargetPos(context);
    //    if (!targetPosOpt)
    //    {
    //        return true;
    //    }
    //
    //    const BoardPosOpt_t posNewOpt = selectWhereToMove(context, targetPosOpt.value());
    //    if (!posNewOpt)
    //    {
    //        return true;
    //    }
    //
    //    const BoardPos_t targetPos = posNewOpt.value();
    //
    //    const auto targetPieceOpt = context.board.pieceAt(targetPos);
    //    if (targetPieceOpt)
    //    {
    //        IPiece & targetPiece = targetPieceOpt.value();
    //        M_ASSERT_OR_THROW(targetPiece.which() == Piece::Player);
    //
    //        if (targetPiece.which() == Piece::Player)
    //        {
    //            // std::cout << "The monster caught you!  You lose." << std::endl;
    //            // context.is_game_over = true;
    //            targetPiece.removeFromPlay();
    //        }
    //    }
    //
    //    move(context, targetPos);
    //
    //    return true;
    //}
    //
    // BoardPosOpt_t
    //    VillanPiece::selectWhereToMove(const Context & context, const BoardPos_t & targetPos)
    //{
    //    M_ASSERT_OR_THROW(isInPlay());
    //    M_ASSERT_OR_THROW(context.map.isPosValid(targetPos));
    //    M_ASSERT_OR_THROW(targetPos != m_boardPos);
    //
    //    MaybeMoves_t moves = makeAllFourAdjacentMaybeMoves(context, targetPos);
    //    M_ASSERT_OR_THROW(moves.size() == 4);
    //
    //    // remove all invalid/off-board moves
    //    moves.erase(
    //        std::remove_if(
    //            std::begin(moves),
    //            std::end(moves),
    //            [&](const MaybeMove & target) { return
    //!context.map.isPosValid(target.position);
    //            }),
    //        std::end(moves));
    //
    //    // remove all moves that this piece is not allowed to step on
    //    moves.erase(
    //        std::remove_if(
    //            std::begin(moves),
    //            std::end(moves),
    //            [&](const MaybeMove & target) { return !Piece::canWalkOn(m_piece,
    target.piece);
    //            }),
    //        std::end(moves));
    //
    //    const std::size_t possibleMovesCount = moves.size();
    //
    //    // end turn without moving if no possible moves remain
    //    if (0 == possibleMovesCount)
    //    {
    //        return std::nullopt;
    //    }
    //
    //    M_ASSERT_OR_THROW(!moves.empty());
    //
    //    if (!context.is_testing_enabled)
    //    {
    //        // how close will the closest possible move get us?
    //        const int closestDistance =
    //            std::min_element(
    //                std::begin(moves),
    //                std::end(moves),
    //                [&](const MaybeMove & left, const MaybeMove & right) {
    //                    return (left.distance_to_target < right.distance_to_target);
    //                })
    //                ->distance_to_target;
    //
    //        // remove any moves that won't get us that close
    //        // (it is possible that there are more than one closest move)
    //        moves.erase(
    //            std::remove_if(
    //                std::begin(moves),
    //                std::end(moves),
    //                [&](const MaybeMove & posDist) {
    //                    return posDist.distance_to_target > closestDistance;
    //                }),
    //            std::end(moves));
    //
    //        M_ASSERT_OR_THROW((moves.size() == 1) || (moves.size() == 2));
    //    }
    //
    //    // random select from the remaining moves
    //    return context.random.from(moves).position;
    //}

    */
} // namespace boardgame