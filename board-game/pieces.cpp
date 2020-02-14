// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// meth-head.cpp
//
#include "pieces.hpp"

#include "board.hpp"
#include "random.hpp"
#include "util.hpp"

#include <cassert>
#include <functional>

namespace boardgame
{
    void PieceBase::move(const Context & context, const BoardPos_t & posNew)
    {
        if (!isInPlay())
        {
            return;
        }

        M_ASSERT_OR_THROW(posNew != m_boardPos);
        M_ASSERT_OR_THROW(context.board.pieceEnumAt(posNew) == Piece::Count);

        m_boardPos = posNew;
        util::centerInside(m_sprite, context.board.cellBounds(m_boardPos));

        M_ASSERT_OR_THROW(context.board.pieceEnumAt(posNew) == m_piece);
    }

    void PieceBase::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        if (!isInPlay())
        {
            return;
        }

        target.draw(m_sprite, states);
    }

    void TailPiece::takeTurn(Context & context)
    {
        if (!isInPlay())
        {
            return;
        }

        if (context.settings.willRemoveTailPiece(m_id))
        {
            removeFromPlay();
        }
    }

    void HeadPiece::move(const Context & context, const BoardPos_t & posNew)
    {
        if (!isInPlay())
        {
            return;
        }

        const BoardPos_t posOld{ m_boardPos };
        PieceBase::move(context, posNew);
        context.board.placePiece(context, Piece::Tail, posOld);
        M_ASSERT_OR_THROW(context.board.pieceEnumAt(posOld) == Piece::Tail);
    }

    void HeadPiece::update(Context &, const float)
    {
        if (!isInPlay())
        {
            return;
        }

        const auto directionNextBeforeChanges{ m_directionKeyNext };

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

        // prevent from reversing direction (instant death)
        if (util::oppositeDirection(m_directionKeyNext) == directionNextBeforeChanges)
        {
            m_directionKeyNext = directionNextBeforeChanges;
        }
    }

    void HeadPiece::takeTurn(Context & context)
    {
        if (!isInPlay())
        {
            return;
        }

        context.settings.handleSnakeHeadTurnStart();

        const BoardPos_t posOld{ m_boardPos };
        const BoardPos_t posNew{ m_boardPos +
                                 util::arrowKeyToPositionAdj(m_directionKeyNext, false) };

        IPieceOpt_t bitePieceOpt{ context.board.pieceAt(posNew) };

        const Piece::Enum bitePieceEnum{ (bitePieceOpt) ? bitePieceOpt->get().piece()
                                                        : Piece::Count };

        if (bitePieceEnum == Piece::Count)
        {
            move(context, posNew);
            return;
        }

        if (bitePieceEnum == Piece::Food)
        {
            context.settings.handleSnakeHeadEat();

            const BoardPosOpt_t foodPosOpt{ context.board.findRandomEmptyPos(context) };
            if (foodPosOpt)
            {
                context.board.placePiece(context, Piece::Food, foodPosOpt.value());
            }
            else
            {
                std::cout << "YOU WIN by filling the entire screen!" << std::endl;
                context.settings.is_game_over = true;
            }
        }
        else
        {
            m_sprite.setColor(sf::Color::Red);

            std::cout << "You bit into "
                      << ((bitePieceEnum == Piece::Wall) ? "the wall" : "yourself") << "! YOU LOSE!"
                      << std::endl;

            context.settings.is_game_over = true;
        }

        IPiece & bitePiece{ bitePieceOpt.value().get() };
        bitePiece.removeFromPlay();

        move(context, posNew);
        context.board.placePiece(context, Piece::Tail, posOld);
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
    //    if ((Piece::Count != targetEnum) && (Piece::Door != targetEnum))
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
    //    //    // std::cout << "You walked into a monster and lost like an idiot." << std::endl;
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
    //        if (piece->isInPlay() && (piece->piece() == Piece::Player))
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
    //        M_ASSERT_OR_THROW(targetPiece.piece() == Piece::Player);
    //
    //        if (targetPiece.piece() == Piece::Player)
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
    //            [&](const MaybeMove & target) { return !context.map.isPosValid(target.position);
    //            }),
    //        std::end(moves));
    //
    //    // remove all moves that this piece is not allowed to step on
    //    moves.erase(
    //        std::remove_if(
    //            std::begin(moves),
    //            std::end(moves),
    //            [&](const MaybeMove & target) { return !Piece::canWalkOn(m_piece, target.piece);
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