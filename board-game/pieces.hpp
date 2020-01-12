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

    struct IPiece : public sf::Drawable
    {
        virtual ~IPiece() = default;

        virtual Piece::Enum piece() const = 0;

        virtual BoardPos_t boardPos() const = 0;

        virtual Score_t score() const = 0;
        virtual void scoreAdj(const Score_t adj) = 0;

        virtual const std::vector<Piece::Enum> & walkablePieces() const = 0;

        virtual std::string toString() const = 0;

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override = 0;

        // returns the new desired pos, but the Collider has to allow it and make it happen
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
            const std::vector<Piece::Enum> & walkablePieces = {})
            : m_piece(piece)
            , m_boardPos(position)
            , m_image(image)
            , m_score(0)
            //, m_text("", context.resources.font, 99)
            , m_sprite()
            , m_color(color)
            , m_waitedSoFarSec(0.0f)
            , m_waitBetweenTurnsSec(0.5f)
            , m_walkAllowedPieces(walkablePieces)
        {
            assert(m_piece != Piece::Count);
            assert(context.board.isPosValid(position));

            setupSprite(context);
            // m_text.setFillColor(m_color);

            // making this random prevents all pieces from moving at the same time, which looks lame
            m_waitedSoFarSec = context.random.zeroTo(m_waitBetweenTurnsSec);
        }

        virtual ~PieceBase() = default;

        Piece::Enum piece() const final { return m_piece; }
        BoardPos_t boardPos() const final { return m_boardPos; }

        Score_t score() const final { return m_score; }

        void scoreAdj(const Score_t adj) final
        {
            m_score += adj;

            if (m_score < 0)
            {
                m_score = 0;
            }
        }

        const std::vector<Piece::Enum> & walkablePieces() const final
        {
            return m_walkAllowedPieces;
        }

        std::string toString() const override;

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            if (m_sprite.getColor().a > 0)
            {
                target.draw(m_sprite, states);
            }

            // target.draw(m_text, states);
        }

        // returns the new desired pos, but the Collider has to allow it and make it happen
        BoardPos_t update(const Context & context, const float elapsedTimeSec) override
        {
            if (!isTimeToTakeTurn(elapsedTimeSec))
            {
                return boardPos();
            }

            return takeTurn(context);
        }

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

        float turnWaitElapsedRatio() const
        {
            return std::clamp((m_waitedSoFarSec / m_waitBetweenTurnsSec), 0.0f, 1.0f);
        }

        virtual bool isTimeToTakeTurn(const float elapsedTimeSec)
        {
            m_waitedSoFarSec += elapsedTimeSec;
            if (m_waitedSoFarSec < m_waitBetweenTurnsSec)
            {
                return false;
            }

            m_waitedSoFarSec = (m_waitedSoFarSec - m_waitBetweenTurnsSec);
            return true;
        }

        // default turn is to do nothing
        virtual BoardPos_t takeTurn(const Context &) { return boardPos(); }

      protected:
        Piece::Enum m_piece;
        BoardPos_t m_boardPos;
        Image::Enum m_image;
        Score_t m_score;
        // sf::Text m_text;
        sf::Sprite m_sprite;
        sf::Color m_color;
        float m_waitedSoFarSec;
        float m_waitBetweenTurnsSec;
        std::vector<Piece::Enum> m_walkAllowedPieces;
    };

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
            const Image::Enum image)
            : PieceBase(context, pos, selfPiece, color, image, { Piece::Count, targetPiece })
            , m_targetPiece(targetPiece)
            , m_targetBoardPos(pos)
        {}

        virtual ~SeekerPieceBase() = default;

        std::string toString() const override;

      protected:
        // if returns false then m_targetBoardPos=currentBoardPos, meaning no target was found
        virtual BoardPos_t pickTarget(const Context &);

        // default simply moves toward m_targetBoardPos if != currentBoardPos
        BoardPos_t takeTurn(const Context & context) override;

        virtual MovesArray_t::iterator findAllPossibleMoves(const Context & context);

        virtual BoardPos_t
            pickMoveClosestToTarget(const Context & context, MovesArray_t::iterator movesEndIter);

      protected:
        Piece::Enum m_targetPiece;
        BoardPos_t m_targetBoardPos;

        static inline MovesArray_t m_moves;
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

    struct PlayerPiece : public PieceBase
    {
        friend Collider;

        PlayerPiece(const Context & context, const BoardPos_t & pos)
            : PieceBase(
                  context,
                  pos,
                  Piece::Player,
                  sf::Color(32, 255, 32),
                  Image::Player,
                  { Piece::Count, Piece::Victim, Piece::Demon })
            , m_nextMovePosAdj(0, 0)
        {}

        virtual ~PlayerPiece() = default;

        BoardPos_t nextMovePosAdj() const { return m_nextMovePosAdj; }
        void nextMovePosAdj(const BoardPos_t & posAdj) { m_nextMovePosAdj = posAdj; }

        BoardPos_t takeTurn(const Context & context) override
        {
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
                  Image::Victim)
        {
            // victims wait a lot longer before moving
            m_waitBetweenTurnsSec = 5.0f;
            m_waitedSoFarSec = 0.0f;
        }

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
                  context, pos, Piece::Demon, Piece::Player, sf::Color::White, Image::Demon)
        {}

        virtual ~DemonPiece() = default;
    };
} // namespace boardgame

#endif // BOARDGAME_PIECES_HPP_INCLUDED