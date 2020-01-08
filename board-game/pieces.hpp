#ifndef BOARDGAME_PIECES_HPP_INCLUDED
#define BOARDGAME_PIECES_HPP_INCLUDED
//
// pieces.hpp
//
#include "context.hpp"
#include "types.hpp"

#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

namespace boardgame
{
    struct Collider;

    //

    struct IPiece : public sf::Drawable
    {
        virtual ~IPiece() = default;

        virtual Piece piece() const = 0;
        virtual BoardPos_t boardPos() const = 0;

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override = 0;

        // returns the new desired pos, but the Collider has to allow it and make it happen
        virtual BoardPos_t update(const Context & context, const float elapsedTimeSec) = 0;
    };

    using IPieceUPtr_t = std::unique_ptr<IPiece>;

    //

    struct PieceBase : public IPiece
    {
        friend Collider;

        PieceBase(
            const Context & context,
            const Piece piece,
            const sf::Color & color,
            const sf::Texture & texture,
            const std::vector<Piece> & walkablePieces = { Piece::Empty },
            const BoardPos_t & pos = InvalidBoardPos);

        virtual ~PieceBase() = default;

        Piece piece() const final { return m_piece; }
        BoardPos_t boardPos() const final { return m_boardPos; }

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            target.draw(m_sprite, states);
            target.draw(m_text, states);
        }

        // returns the new desired pos, but the Collider has to allow it and make it happen
        BoardPos_t update(const Context & context, const float elapsedTimeSec) override
        {
            if (!isTimeToTakeTurn(elapsedTimeSec))
            {
                return boardPos();
            }
            else
            {
                return takeTurn(context);
            }
        }

      protected:
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
        Piece m_piece;
        BoardPos_t m_boardPos;
        sf::Text m_text;
        sf::Sprite m_sprite;
        sf::Color m_color;
        float m_waitedSoFarSec;
        float m_waitBetweenTurnsSec;
        std::vector<Piece> m_walkAllowedPieces;
    };

    //

    // these are pieces that take turns, either the player or np/ai
    struct SeekerPieceBase : public PieceBase
    {
        SeekerPieceBase(
            const Context & context,
            const Piece piece,
            const sf::Color & color,
            const sf::Texture & texture,
            const Piece targetPiece,
            const BoardPos_t & pos = InvalidBoardPos)
            : PieceBase(context, piece, color, texture, { Piece::Empty, targetPiece }, pos)
            , m_targetPiece(targetPiece)
            , m_targetBoardPos(boardPos()) // when these are equal it calls pickTarget()
        {}

        virtual ~SeekerPieceBase() = default;

      protected:
        // if returns false m_targetBoardPos is set to current position, meaning no target was found
        virtual BoardPos_t pickTarget(const Context &);

        // default simply moves toward m_targetBoardPos if != boardPos()
        BoardPos_t takeTurn(const Context & context) override;

        virtual MovesArray_t::iterator findAllPossibleMoves(const Context & context);

        virtual BoardPos_t
            pickMoveClosestToTarget(const Context & context, MovesArray_t::iterator movesEndIter);

      protected:
        Piece m_targetPiece;
        BoardPos_t m_targetBoardPos;

        static inline MovesArray_t m_moves;
    };

    //

    struct WallPiece : public PieceBase
    {
        friend Collider;
        WallPiece(const Context & context, const BoardPos_t & pos);
        virtual ~WallPiece() = default;
        static const std::string imageRelativePath() { return "image/tile/brick-tiny-bw.png"; }
    };

    using WallUPtr_t = std::unique_ptr<WallPiece>;

    //

    struct ChildPiece : public PieceBase
    {
        friend Collider;
        explicit ChildPiece(const Context & context, const BoardPos_t & pos = InvalidBoardPos);
        virtual ~ChildPiece() = default;
        static const std::string imageRelativePath() { return "image/child.png"; }
    };

    using ChildUPtr_t = std::unique_ptr<ChildPiece>;

    //

    struct PlayerPiece : public PieceBase
    {
        friend Collider;

        explicit PlayerPiece(const Context & context, const BoardPos_t & pos = InvalidBoardPos);
        virtual ~PlayerPiece() = default;

        BoardPos_t nextMovePos() const { return m_nextMovePos; }
        void nextMovePos(const BoardPos_t & pos) { m_nextMovePos = pos; }

        BoardPos_t takeTurn(const Context &) override
        {
            if (InvalidBoardPos == m_nextMovePos)
            {
                return boardPos();
            }

            // reset next move after moving
            const BoardPos_t nextMovePosOrig{ m_nextMovePos };
            m_nextMovePos = nextMovePosOrig;
            return nextMovePosOrig;
        }

        static std::string imageRelativePath() { return "image/hero.png"; }

      protected:
        BoardPos_t m_nextMovePos;
    };

    using PlayerUPtr_t = std::unique_ptr<PlayerPiece>;

    //

    struct DemonPiece : public SeekerPieceBase
    {
        friend Collider;
        explicit DemonPiece(const Context & context, const BoardPos_t & pos = InvalidBoardPos);
        virtual ~DemonPiece() = default;
        static std::string imageRelativePath() { return "image/demon.png"; }
    };

    using DemonUPtr_t = std::unique_ptr<DemonPiece>;
} // namespace boardgame

#endif // BOARDGAME_PIECES_HPP_INCLUDED