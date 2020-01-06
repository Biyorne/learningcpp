#ifndef BOARDGAME_BASE_HPP_INCLUDED
#define BOARDGAME_BASE_HPP_INCLUDED

#include "context.hpp"
#include "display-constants.hpp"
#include "scoped-board-position.hpp"
#include "types.hpp"

#include <SFML/Graphics.hpp>

namespace boardgame
{
    class Collider;

    //

    struct PieceBase
        : public sf::Drawable
        , public ScopedBoardPosition
    {
        friend Collider;

        PieceBase(
            const Context & context,
            const Piece piece,
            const sf::Color & color,
            const sf::Texture & texture,
            const Piece target = Piece::None)
            : ScopedBoardPosition(context, piece)
            , m_piece(piece)
            , m_text(context.resources.default_text)
            , m_sprite(texture)
            , m_color(color)
            , m_waitBetweenTurnsSec(0.3f)
            , m_waitedSoFarSec(0.0f)
            , m_targetPiece(target)
            , m_targetBoardPos(boardPos())
        {
            m_text.setFillColor(m_color);
            m_sprite.setColor(m_color);
        }

        // allows a specific starting board pos, caller is responsible it being valid
        PieceBase(
            const Context & context,
            const Piece piece,
            const sf::Color & color,
            const sf::Texture & texture,
            const Piece target,
            const BoardPos_t & pos)
            : PieceBase(context, piece, color, texture, target)
        {
            ScopedBoardPosition::set(context, piece, pos);
            m_targetBoardPos = boardPos();
        }

        virtual ~PieceBase() = default;

        Piece piece() const { return m_piece; }
        BoardPos_t boardPos() const { return ScopedBoardPosition::get(); }

        // TODO move
        static inline int walkDistance(const BoardPos_t & from, const BoardPos_t & to)
        {
            return (std::abs(to.x - from.x) + std::abs(to.y - from.y));
        }
        int walkDistanceTo(const BoardPos_t & to) const { return walkDistance(boardPos(), to); }
        int walkDistanceTo(const PieceBase & to) const
        {
            return walkDistance(boardPos(), to.boardPos());
        }

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            target.draw(m_sprite, states);
            target.draw(m_text, states);
        }

        // returns the new desired pos, but the Collider has to allow it and make it happen
        virtual BoardPos_t update(const Context & context, const float elapsedTimeSec)
        {
            if ((Piece::None == m_targetPiece) || !isTimeToTakeTurn(elapsedTimeSec))
            {
                return boardPos();
            }
            else
            {
                return takeTurn(context);
            }
        }

      protected:
        virtual bool isTimeToTakeTurn(const float elapsedSec)
        {
            m_waitedSoFarSec += elapsedSec;
            if (m_waitedSoFarSec < m_waitBetweenTurnsSec)
            {
                return false;
            }

            m_waitedSoFarSec = (m_waitedSoFarSec - m_waitBetweenTurnsSec);
            return true;
        }

        // default to returning the current position meaning no target was found
        virtual BoardPos_t pickTarget(const Context & context) const;

        // default simply moves toward m_targetBoardPos if it is not the same as boardPos()
        virtual BoardPos_t takeTurn(const Context & context);

        virtual MovesArray_t::iterator findAllPossibleMoves(const Context & context);

        virtual BoardPos_t
            pickPossibleMove(const Context & context, MovesArray_t::iterator movesEndIter);

      protected:
        Piece m_piece;
        sf::Text m_text;
        sf::Sprite m_sprite;
        sf::Color m_color;

        float m_waitBetweenTurnsSec;
        float m_waitedSoFarSec;

        Piece m_targetPiece;
        BoardPos_t m_targetBoardPos;

        static inline MovesArray_t m_moves;
    };

    using PieceUPtr_t = std::unique_ptr<PieceBase>;
    using PieceUPtrVec_t = std::vector<PiceUPtr_t>;

    //

    struct WallPiece : public PieceBase
    {
        friend Collider;

        explicit WallPiece(const Context & context, const BoardPos_t & pos)
            : PieceBase(
                  context,
                  Piece::Wall,
                  sf::Color(200, 105, 5),
                  context.resources.wall_texture,
                  Piece::None,
                  pos)
        {}

        virtual ~WallPiece() = default;

        BoardPos_t update(const Context & context, const float elapsedTimeSec) override
        {
            return boardPos();
        }

        static std::string imageRelativePath() { return "/image/tile/brick-tiny-bw.png"; }
    };

    //

    struct ChildPiece : public PieceBase
    {
        friend Collider;

        explicit ChildPiece(const Context & context)
            : PieceBase(
                  context, Piece::Child, sf::Color(255, 32, 255), context.resources.child_texture)
        {}

        virtual ~ChildPiece() = default;

        BoardPos_t update(const Context & context, const float elapsedTimeSec) override
        {
            return boardPos();
        }

        static std::string imageRelativePath() { return "/image/child.png"; }
    };

    //

    struct DemonPiece : public PieceBase
    {
        friend Collider;

        explicit DemonPiece(const Context & context)
            : PieceBase(
                  context,
                  Piece::Demon,
                  sf::Color(255, 32, 32),
                  context.resources.demon_texture,
                  Piece::Hero)
        {}

        virtual ~DemonPiece() = default;

        static std::string imageRelativePath() { return "/image/demon.png"; }
    };

    //

    struct PlayerPiece : public PieceBase
    {
        friend Collider;

        PlayerPiece(const Context & context)
            : PieceBase(
                  context,
                  Piece::Hero,
                  sf::Color(32, 255, 32),
                  context.resources.hero_texture,
                  Piece::Child)
        {}

        void setTargetBoardPos(const BoardPos_t & newPos) { m_targetBoardPos = newPos; }

        BoardPos_t takeTurn(const Context & context) override { return context.player_pos_change; }

        static std::string imageRelativePath() { return "/image/hero.png"; }
    };
} // namespace boardgame

#endif // BOARDGAME_BASE_HPP_INCLUDED