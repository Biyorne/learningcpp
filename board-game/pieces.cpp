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
    sf::Sprite PieceBase::makeSprite(
        const Context & context, const Piece::Enum piece, const BoardPos_t & boardPos) const
    {
        M_ASSERT_OR_THROW(Piece::Count != piece);
        M_ASSERT_OR_THROW(context.map.isPosValid(boardPos));

        sf::Sprite sprite(context.resources.texture(piece));
        util::setOriginToCenter(sprite);
        util::scaleAndCenterInside(sprite, context.board.calcCellBounds(boardPos));
        sprite.setColor(Piece::color(piece));
        return sprite;
    }

    void PieceBase::move(const Context & context, const BoardPos_t & targetPos)
    {
        M_ASSERT_OR_THROW(isAlive());
        M_ASSERT_OR_THROW(context.map.isPosValid(targetPos));
        M_ASSERT_OR_THROW(targetPos != m_boardPos);

        m_boardPos = targetPos;
        util::centerInside(m_sprite, context.board.calcCellBounds(targetPos));
    }

    MaybeMoves_t PieceBase::makeAllFourAdjacentMaybeMoves(
        const Context & context, const BoardPos_t & targetPos) const
    {
        M_ASSERT_OR_THROW(isAlive());
        M_ASSERT_OR_THROW(context.map.isPosValid(targetPos));

        // clang-format off
        return { MaybeMove(context, (m_boardPos + sf::Vector2i(-1,  0)), targetPos ),
                 MaybeMove(context, (m_boardPos + sf::Vector2i( 1,  0)), targetPos ),
                 MaybeMove(context, (m_boardPos + sf::Vector2i( 0, -1)), targetPos ),
                 MaybeMove(context, (m_boardPos + sf::Vector2i( 0,  1)), targetPos ) };
        // clang-format on
    }

    //

    bool PlayerPiece::takeTurn(Context & context, const sf::Event & eventOrig)
    {
        M_ASSERT_OR_THROW(isAlive());
        sf::Event event(eventOrig);

        if (context.is_testing_enabled)
        {
            event.type = sf::Event::EventType::KeyPressed;
            event.key = sf::Event::KeyEvent{};

            auto randomArrowKeyCode = [&]() {
                switch (context.random.zeroTo(3))
                {
                    case 0: return sf::Keyboard::Left;
                    case 1: return sf::Keyboard::Right;
                    case 2: return sf::Keyboard::Up;
                    default: return sf::Keyboard::Down;
                }
            };

            event.key.code = randomArrowKeyCode();
        }

        if (sf::Keyboard::Right == event.key.code)
        {
            return attemptMove(context, (m_boardPos + BoardPos_t(1, 0)));
        }
        else if (sf::Keyboard::Left == event.key.code)
        {
            return attemptMove(context, (m_boardPos + BoardPos_t(-1, 0)));
        }
        else if (sf::Keyboard::Up == event.key.code)
        {
            return attemptMove(context, (m_boardPos + BoardPos_t(0, -1)));
        }
        else if (sf::Keyboard::Down == event.key.code)
        {
            return attemptMove(context, (m_boardPos + BoardPos_t(0, 1)));
        }

        return false;
    }

    bool PlayerPiece::attemptMove(Context & context, const BoardPos_t & targetPos)
    {
        M_ASSERT_OR_THROW(isAlive());
        M_ASSERT_OR_THROW(context.map.isPosValid(targetPos));
        M_ASSERT_OR_THROW(targetPos != m_boardPos);

        const Piece::Enum targetEnum{ context.board.pieceEnumAt(targetPos) };

        // if ((Piece::Player == targetEnum) || (Piece::Wall == targetEnum) ||
        //    (Piece::Villan == targetEnum))
        //{
        //    return false;
        //}

        if ((Piece::Count != targetEnum) && (Piece::Door != targetEnum))
        {
            return false;
        }

        move(context, targetPos);

        if (Piece::Door == targetEnum)
        {
            kill();
            // std::cout << "You are free!  You WIN!" << std::endl;
            // context.is_game_over = true;
        }
        // else if (Piece::Villan == targetEnum)
        //{
        //    // std::cout << "You walked into a monster and lost like an idiot." << std::endl;
        //    context.is_game_over = true;
        //    kill();
        //}

        return true;
    }

    //

    BoardPosOpt_t VillanPiece::findTargetPos(const Context & context)
    {
        M_ASSERT_OR_THROW(isAlive());

        for (const IPieceUPtr_t & piece : context.board.pieces())
        {
            if (piece->isAlive() && (piece->piece() == Piece::Player))
            {
                return piece->boardPos();
            }
        }

        return std::nullopt;
    }

    // always returns true because AI or Villans or Non-Players only get one chance to move
    bool VillanPiece::takeTurn(Context & context, const sf::Event &)
    {
        M_ASSERT_OR_THROW(isAlive());

        const BoardPosOpt_t targetPosOpt = findTargetPos(context);
        if (!targetPosOpt)
        {
            return true;
        }

        const BoardPosOpt_t newPosOpt = selectWhereToMove(context, targetPosOpt.value());
        if (!newPosOpt)
        {
            return true;
        }

        const BoardPos_t targetPos = newPosOpt.value();

        const auto targetPieceOpt = context.board.pieceAt(targetPos);
        if (targetPieceOpt)
        {
            IPiece & targetPiece = targetPieceOpt.value();
            M_ASSERT_OR_THROW(targetPiece.piece() == Piece::Player);

            if (targetPiece.piece() == Piece::Player)
            {
                // std::cout << "The monster caught you!  You lose." << std::endl;
                // context.is_game_over = true;
                targetPiece.kill();
            }
        }

        move(context, targetPos);

        return true;
    }

    BoardPosOpt_t
        VillanPiece::selectWhereToMove(const Context & context, const BoardPos_t & targetPos)
    {
        M_ASSERT_OR_THROW(isAlive());
        M_ASSERT_OR_THROW(context.map.isPosValid(targetPos));
        M_ASSERT_OR_THROW(targetPos != m_boardPos);

        MaybeMoves_t moves = makeAllFourAdjacentMaybeMoves(context, targetPos);
        M_ASSERT_OR_THROW(moves.size() == 4);

        // remove all invalid/off-board moves
        moves.erase(
            std::remove_if(
                std::begin(moves),
                std::end(moves),
                [&](const MaybeMove & target) { return !context.map.isPosValid(target.position); }),
            std::end(moves));

        // remove all moves that this piece is not allowed to step on
        moves.erase(
            std::remove_if(
                std::begin(moves),
                std::end(moves),
                [&](const MaybeMove & target) { return !Piece::canWalkOn(m_piece, target.piece); }),
            std::end(moves));

        const std::size_t possibleMovesCount = moves.size();

        // end turn without moving if no possible moves remain
        if (0 == possibleMovesCount)
        {
            return std::nullopt;
        }

        M_ASSERT_OR_THROW(!moves.empty());

        if (!context.is_testing_enabled)
        {
            // how close will the closest possible move get us?
            const int closestDistance =
                std::min_element(
                    std::begin(moves),
                    std::end(moves),
                    [&](const MaybeMove & left, const MaybeMove & right) {
                        return (left.distance_to_target < right.distance_to_target);
                    })
                    ->distance_to_target;

            // remove any moves that won't get us that close
            // (it is possible that there are more than one closest move)
            moves.erase(
                std::remove_if(
                    std::begin(moves),
                    std::end(moves),
                    [&](const MaybeMove & posDist) {
                        return posDist.distance_to_target > closestDistance;
                    }),
                std::end(moves));

            M_ASSERT_OR_THROW((moves.size() == 1) || (moves.size() == 2));
        }

        // random select from the remaining moves
        return context.random.from(moves).position;
    }
} // namespace boardgame