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
    struct Collider;

    //

    struct WalkTarget
    {
        WalkTarget() = default;

        WalkTarget(const Context & context, const BoardPos_t & boardPos, const int walkDistance = 0)
            : position(boardPos)
            , piece(context.board.cellAt(boardPos).piece_enum)
            , distance(walkDistance)
        {}

        BoardPos_t position = { 0, 0 };
        Piece::Enum piece = Piece::Count;
        int distance = 0;
    };

    using WalkTargetArray_t = std::array<WalkTarget, 4>;

    //

    struct IPiece : public sf::Drawable
    {
        virtual ~IPiece() = default;

        virtual Piece::Enum piece() const = 0;
        virtual BoardPos_t boardPos() const = 0;

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override = 0;

        // returns the new pos that this piece wants to walk on, but has not yet done so
        // only Collider decides if so, and actually moves pieces as needed
        virtual BoardPos_t update(const Context & context, const float elapsedTimeSec) = 0;
    };

    using IPieceUPtr_t = std::unique_ptr<IPiece>;
    using IPieceUVec_t = std::vector<IPieceUPtr_t>;

    using IPieceOpt_t = std::optional<std::reference_wrapper<IPiece>>;
    using IPieceCOpt_t = std::optional<std::reference_wrapper<const IPiece>>;

    //

    struct PieceBase : public IPiece
    {
        friend Collider;

        PieceBase(
            const Context & context,
            const BoardPos_t & position,
            const Piece::Enum piece,
            const sf::Color & color,
            const Image::Enum image,
            const std::vector<Piece::Enum> & walkablePieces = {},
            const float speed = 0.0f)
            : m_piece(piece)
            , m_boardPos(position)
            , m_image(image)
            , m_sprite()
            , m_windowPosPrev()
            , m_color(color)
            , m_speed(speed)
            , m_walkablePieces(walkablePieces)
        {
            assert(m_piece != Piece::Count);
            assert(context.board.isPosValid(position));

            setupSprite(context);
            m_windowPosPrev = m_sprite.getPosition();
        }

        virtual ~PieceBase() = default;

        Piece::Enum piece() const final { return m_piece; }
        BoardPos_t boardPos() const final { return m_boardPos; }

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            target.draw(m_sprite, states);
        }

        // default does nothing
        BoardPos_t update(const Context &, const float) override { return boardPos(); }

      protected:
        void setupSprite(const Context & context)
        {
            m_sprite.setTexture(context.resources.tile_texture);

            const sf::IntRect textureCoords{ Image::coords(m_image) };
            if ((textureCoords.width > 0) && (textureCoords.height))
            {
                m_sprite.setTextureRect(textureCoords);
            }

            m_sprite.setColor(m_color);
            util::scale(m_sprite, context.board.cell_size);
            util::setOriginToCenter(m_sprite);
            m_sprite.setPosition(context.board.cellAt(boardPos()).window_rect_center);
        }

        // default turn is to do nothing
        virtual BoardPos_t takeTurn(const Context &) { return boardPos(); }

      protected:
        Piece::Enum m_piece;
        BoardPos_t m_boardPos;
        Image::Enum m_image;
        sf::Sprite m_sprite;
        sf::Vector2f m_windowPosPrev;
        sf::Color m_color;
        float m_speed;
        std::vector<Piece::Enum> m_walkablePieces;
    }; // namespace boardgame

    //

    // these are pieces that take turns, either the player or np/ai
    struct SeekerPieceBase : public PieceBase
    {
        SeekerPieceBase(
            const Context & context,
            const BoardPos_t & pos,
            const Piece::Enum selfPiece,
            const Piece::Enum targetPiece,
            const sf::Color & color,
            const Image::Enum image,
            const float speed,
            const std::vector<Piece::Enum> & walkablePieces = {})
            : PieceBase(context, pos, selfPiece, color, image, walkablePieces, speed)
            , m_targetPiece(targetPiece)
            , m_targetBoardPos(pos)
            , m_isDoneMovingThisTurn(false)

        {
            m_walkablePieces.push_back(Piece::Count);
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
        Piece::Enum m_targetPiece;
        BoardPos_t m_targetBoardPos;
        bool m_isDoneMovingThisTurn;
    };

    //

    struct WallPiece : public PieceBase
    {
        friend Collider;

        // transparent color prevents drawing because walls are not drawn as pieces,
        // they are drawn to a common background
        WallPiece(const Context & context, const BoardPos_t & pos, const Image::Enum image)
            : PieceBase(context, pos, Piece::Wall, sf::Color::Transparent, image)
        {}

        virtual ~WallPiece() = default;
    };

    //

    struct PlayerPiece : public SeekerPieceBase
    {
        friend Collider;

        PlayerPiece(const Context & context, const BoardPos_t & pos)
            : SeekerPieceBase(
                  context,
                  pos,
                  Piece::Player,
                  Piece::Victim,
                  sf::Color(32, 255, 32),
                  Image::Player,
                  10.0f,
                  { Piece::Count, Piece::Victim, Piece::Demon })
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

    struct VictimPiece : public SeekerPieceBase
    {
        friend Collider;

        VictimPiece(const Context & context, const BoardPos_t & pos)
            : SeekerPieceBase(
                  context,
                  pos,
                  Piece::Victim,
                  Piece::Player,
                  sf::Color(255, 32, 255),
                  Image::Victim,
                  1.0f)
        {}

        // victims only move half the time
        BoardPos_t takeTurn(const Context & context) override
        {
            if (context.random.boolean())
            {
                return SeekerPieceBase::takeTurn(context);
            }
            else
            {
                return boardPos();
            }
        }

        virtual ~VictimPiece() = default;
    };

    //

    struct DemonPiece : public SeekerPieceBase
    {
        friend Collider;

        DemonPiece(const Context & context, const BoardPos_t & pos)
            : SeekerPieceBase(
                  context, pos, Piece::Demon, Piece::Player, sf::Color::White, Image::Demon, 4.0f)
        {}

        virtual ~DemonPiece() = default;
    };
} // namespace boardgame

#endif // BOARDGAME_PIECES_HPP_INCLUDED