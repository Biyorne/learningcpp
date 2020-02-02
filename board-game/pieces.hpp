#ifndef BOARDGAME_PIECES_HPP_INCLUDED
#define BOARDGAME_PIECES_HPP_INCLUDED
//
// pieces.hpp
//
#include "board.hpp"
#include "context.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "types.hpp"

#include <cassert>
#include <memory>
#include <optional>
#include <string>

#include <SFML/Graphics.hpp>

namespace boardgame
{
    struct WalkTarget
    {
        WalkTarget(const Context & context, const BoardPos_t & boardPos, const int walkDistance = 0)
            : position(boardPos)
            , piece(context.board.map.posToPiece(boardPos))
            , distance(walkDistance)
        {}

        BoardPos_t position;
        Piece piece;
        int distance;
    };

    using WalkTargetArray_t = std::array<WalkTarget, 4>;

    //

    struct IPiece : public sf::Drawable
    {
        virtual ~IPiece() = default;

        virtual Piece piece() const = 0;
        virtual BoardPos_t boardPos() const = 0;
        virtual sf::FloatRect bounds() const = 0;

        virtual void handleEvent(Context &, const sf::Event & event) = 0;
        void draw(sf::RenderTarget & target, sf::RenderStates states) const override = 0;
        virtual void update(Context & context, const float elapsedTimeSec) = 0;
    };

    using IPieceUPtr_t = std::unique_ptr<IPiece>;
    using IPieceUVec_t = std::vector<IPieceUPtr_t>;

    //

    struct PieceBase : public IPiece
    {
        PieceBase(
            const Piece piece,
            const BoardPos_t & pos,
            const sf::FloatRect & bounds,
            const sf::Sprite & sprite);

        virtual ~PieceBase() = default;

        Piece piece() const final { return m_piece; }
        BoardPos_t boardPos() const final { return m_boardPos; }
        sf::FloatRect bounds() const final { return m_bounds; }

        void handleEvent(Context &, const sf::Event &) override {}

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            target.draw(m_sprite, states);
        }

        void update(Context &, const float) override {}

      protected:
        Piece m_piece;
        BoardPos_t m_boardPos;
        sf::FloatRect m_bounds;
        sf::Sprite m_sprite;
    };

    //

    struct EmptyPiece : public PieceBase
    {
        explicit EmptyPiece(
            const ImageHandler & images, const BoardPos_t & pos, const sf::FloatRect & bounds)
            : PieceBase(Piece::Empty, pos, bounds, images.makeSprite(Image::Blank, bounds))
        {}

        virtual ~EmptyPiece() = default;
    };

    //

    struct WallPiece : public PieceBase
    {
        WallPiece(
            const ImageHandler & images,
            const Image whichWallImage,
            const BoardPos_t & pos,
            const sf::FloatRect & bounds,
            const bool hasShadow = false)
            : PieceBase(Piece::Obstacle, pos, bounds, images.makeSprite(whichWallImage, bounds))
            , m_shadowSprite()
        {
            if (hasShadow)
            {
                m_shadowSprite = images.makeSprite(Image::Shadow, m_bounds);
            }
        }

        virtual ~WallPiece() = default;

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            PieceBase::draw(target, states);
            target.draw(m_shadowSprite, states);
        }

      private:
        sf::Sprite m_shadowSprite;
    };

    //

    struct PlayerPiece : public PieceBase
    {
        PlayerPiece(
            const ImageHandler & images, const BoardPos_t & pos, const sf::FloatRect & bounds)
            : PieceBase(
                  Piece::Obstacle,
                  pos,
                  bounds,
                  images.makeSprite(Image::Player, bounds, sf::Color::Green))
        {}

        virtual ~PlayerPiece() = default;

        void handleEvent(Context & context, const sf::Event & event) override
        {
            if (sf::Event::KeyPressed != event.type)
            {
                return;
            }

            if (sf::Keyboard::Right == event.key.code)
            {
                move(context, (m_boardPos + BoardPos_t(1, 0)));
            }
            else if (sf::Keyboard::Left == event.key.code)
            {
                move(context, (m_boardPos + BoardPos_t(-1, 0)));
            }
            else if (sf::Keyboard::Up == event.key.code)
            {
                move(context, (m_boardPos + BoardPos_t(0, -1)));
            }
            else if (sf::Keyboard::Down == event.key.code)
            {
                move(context, (m_boardPos + BoardPos_t(0, 1)));
            }
        }

        void attemptToMove(Context & context, const BoardPos_t & newPos)
        {
            const std::size_t targetIndex{ context.board.posToIndex(newPos) };

            const IPiece & targetPiece{ context.board.pieces.at(targetIndex) };

            if (targetPiece.piece() == Piece::Empty)
            {
            }
        }

        void move(Context & context, const BoardPos_t & newPos) {}
    };

    //

    // these are pieces that take turns, either the player or np/ai
    struct SeekerPieceBase : public PieceBase
    {
        SeekerPieceBase(
            const Piece selfPiece,
            const BoardPos_t & pos,
            const sf::Sprite & sprite,
            const Piece targetPiece,
            const float speed,
            const std::vector<Piece> & walkablePieces = {})
            : PieceBase(selfPiece, pos, sprite, walkablePieces, speed)
            , m_targetPiece(targetPiece)
            , m_targetBoardPos(pos)
            , m_isDoneMovingThisTurn(false)
        {
            m_walkablePieces.push_back(Piece::Empty);
            m_walkablePieces.push_back(targetPiece);
        }

        virtual ~SeekerPieceBase() = default;

        // returns the new desired pos, but the Collider has to allow it and make it happen
        BoardPos_t update(const Context & context, const float elapsedTimeSec) override
        {
            if (!(m_speed > 0.0f))
            {
                return boardPos();
            }

            if (m_isDoneMovingThisTurn)
            {
                return takeTurn(context);
            }

            const sf::Vector2f windowTargetPos{
                context.board.cellAt(m_boardPos).window_rect_center
            };

            const sf::Vector2f posDiff{ util::difference(m_sprite.getPosition(), windowTargetPos) };
            const float distanceBefore{ util::magnitude(posDiff) };
            if (distanceBefore < 1.0f)
            {
                m_sprite.setPosition(windowTargetPos);
                m_isDoneMovingThisTurn = true;
                return takeTurn(context);
            }

            const float movePerFrame{ (m_speed / elapsedTimeSec) / context.board.cell_size.x };
            m_sprite.move(util::normalize(posDiff) * movePerFrame);

            const float distanceAfter{ util::distance(m_sprite.getPosition(), windowTargetPos) };
            if (distanceBefore < distanceAfter)
            {
                m_sprite.setPosition(windowTargetPos);
                m_isDoneMovingThisTurn = true;
                return takeTurn(context);
            }

            return boardPos();
        }

      protected:
        void setBoardPos(const Context & context, const BoardPos_t & newPos) override
        {
            PieceBase::setBoardPos(context, newPos);
            m_isDoneMovingThisTurn = false;
        }

        WalkTargetArray_t
            makePossibleMovesArray(const Context & context, const BoardPos_t & pos) const
        {
            // clang-format off
            return { WalkTarget{ context, (pos + sf::Vector2i(-1,  0)) },
                     WalkTarget{ context, (pos + sf::Vector2i( 1,  0)) },
                     WalkTarget{ context, (pos + sf::Vector2i( 0, -1)) },
                     WalkTarget{ context, (pos + sf::Vector2i( 0,  1)) } };
            // clang-format on
        }

        // if returns false then m_targetBoardPos=currentBoardPos, meaning no target was found
        virtual BoardPos_t pickTarget(const Context &);

        // default simply moves toward m_targetBoardPos if != currentBoardPos
        BoardPos_t takeTurn(const Context & context) override;

        virtual WalkTargetArray_t::iterator
            findAllPossibleMoves(const Context & context, WalkTargetArray_t & moves);

        virtual BoardPos_t pickMoveClosestToTarget(
            const Context & context,
            WalkTargetArray_t & moves,
            WalkTargetArray_t::iterator & movesEndIter);

      protected:
        Piece m_targetPiece;
        BoardPos_t m_targetBoardPos;
        bool m_isDoneMovingThisTurn;
    };

    //

    struct PickupPiece : public PieceBase
    {
        friend Collider;

        PickupPiece(const BoardPos_t & pos, const sf::Sprite & sprite)
            : PieceBase(Piece::Pickup, pos, sprite)
        {}

        virtual ~PickupPiece() = default;
    };

    //
    struct PlayerPiece : public SeekerPieceBase
    {
        friend Collider;

        PlayerPiece(const BoardPos_t & pos, const sf::Sprite & sprite)
            : SeekerPieceBase(
                  Piece::Player,
                  pos,
                  sprite,
                  Piece::Pickup,
                  10.0f,
                  { Piece::Empty, Piece::Pickup, Piece::Villan })
            , m_nextMovePosAdj(0, 0)
        {}

        virtual ~PlayerPiece() = default;

        BoardPos_t nextMovePosAdj() const { return m_nextMovePosAdj; }
        void nextMovePosAdj(const BoardPos_t & posAdj) { m_nextMovePosAdj = posAdj; }

        BoardPos_t takeTurn(const Context & context) override
        {
            if (sf::Vector2i(0, 0) == m_nextMovePosAdj)
            {
                return boardPos();
            }

            const BoardPos_t newPos{ boardPos() + m_nextMovePosAdj };

            // reset next move if we actually will move
            if ((boardPos() != newPos) && context.board.isPosValid(newPos))
            {
                m_nextMovePosAdj = { 0, 0 };
            }

            return newPos;
        }

      protected:
        BoardPos_t m_nextMovePosAdj; // only ever {0,1}, {-1,0}, etc.
    };

    //

    struct VillanPiece : public SeekerPieceBase
    {
        friend Collider;

        VillanPiece(const BoardPos_t & pos, const sf::Sprite & sprite)
            : SeekerPieceBase(
                  Piece::Villan,
                  pos,
                  sprite,
                  Piece::Player,
                  1.0f,
                  { Piece::Empty, Piece::Pickup, Piece::Villan })
        {}

        virtual ~VillanPiece() = default;
    };
} // namespace boardgame

#endif // BOARDGAME_PIECES_HPP_INCLUDED