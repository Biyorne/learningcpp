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

#include <memory>
#include <optional>

#include <SFML/Graphics.hpp>

namespace boardgame
{
    // Responsible for wrapping as much info about a board position as possible,
    // so that non-player pieces can more easily and consistently decide where to move.
    struct MaybeMove
    {
        MaybeMove(
            const Context & context, const BoardPos_t & maybePos, const BoardPos_t & targetPos)
            : piece(context.board.pieceEnumAt(maybePos))
            , position(maybePos)
            , distance_to_target(walkDistance(maybePos, targetPos))
        {}

        Piece::Enum piece;
        BoardPos_t position;
        int distance_to_target; // not an accurate count of steps
    };

    using MaybeMoves_t = std::vector<MaybeMove>;

    //

    struct IPiece : public sf::Drawable
    {
        virtual ~IPiece() = default;

        virtual bool isAlive() const = 0;
        virtual void kill() = 0;

        virtual Piece::Enum piece() const = 0;
        virtual BoardPos_t boardPos() const = 0;
        virtual sf::FloatRect bounds() const = 0;

        // returns true if turn is finished and the next piece can take their turn
        virtual bool takeTurn(Context &, const sf::Event & event = {}) = 0;

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override = 0;

        virtual void update(Context & context, const float elapsedTimeSec) = 0;
    };

    //

    using IPieceOpt_t = std::optional<std::reference_wrapper<IPiece>>;
    using IPieceUPtr_t = std::unique_ptr<IPiece>;
    using IPieceUVec_t = std::vector<IPieceUPtr_t>;

    //

    struct PieceBase : public IPiece
    {
        PieceBase(
            const Context &,
            const Piece::Enum piece,
            const BoardPos_t & boardPos,
            const sf::Sprite & sprite)
            : m_isAlive(true)
            , m_piece(piece)
            , m_boardPos(boardPos)
            , m_sprite(sprite)
        {}

        PieceBase(const Context & context, const Piece::Enum piece, const BoardPos_t & boardPos)
            : PieceBase(context, piece, boardPos, makeSprite(context, piece, boardPos))
        {}

        virtual ~PieceBase() = default;

        bool isAlive() const override { return m_isAlive; }
        void kill() override { m_isAlive = false; }

        Piece::Enum piece() const override { return m_piece; }
        BoardPos_t boardPos() const override { return m_boardPos; }
        sf::FloatRect bounds() const override { return m_sprite.getGlobalBounds(); }

        // default is to do nothing and let the next piece take its turn
        bool takeTurn(Context &, const sf::Event &) override { return true; }

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            if (!isAlive())
            {
                return;
            }

            target.draw(m_sprite, states);
        }

        // default is to do nothing
        void update(Context &, const float) override {}

      protected:
        virtual sf::Sprite makeSprite(
            const Context & context, const Piece::Enum piece, const BoardPos_t & boardPos) const;

        virtual void move(const Context & context, const BoardPos_t & targetPos);

        virtual MaybeMoves_t makeAllFourAdjacentMaybeMoves(
            const Context & context, const BoardPos_t & targetPos) const;

      protected:
        bool m_isAlive;
        Piece::Enum m_piece;
        BoardPos_t m_boardPos;
        sf::Sprite m_sprite;
    };

    //

    struct WallPiece : public PieceBase
    {
        WallPiece(const Context & context, const BoardPos_t & boardPos)
            : PieceBase(context, Piece::Wall, boardPos)
        {}

        virtual ~WallPiece() = default;
    };

    //

    struct DoorPiece : public PieceBase
    {
        DoorPiece(const Context & context, const BoardPos_t & boardPos)
            : PieceBase(context, Piece::Door, boardPos)
        {}

        virtual ~DoorPiece() = default;
    };

    //

    struct PlayerPiece : public PieceBase
    {
        PlayerPiece(const Context & context, const BoardPos_t & boardPos)
            : PieceBase(context, Piece::Player, boardPos)
        {}

        virtual ~PlayerPiece() = default;

        bool takeTurn(Context & context, const sf::Event & event) override;

      private:
        bool attemptMove(Context & context, const BoardPos_t & targetPos);
    };

    //

    // these are pieces that take turns, either the player or np/ai
    struct VillanPiece : public PieceBase
    {
        VillanPiece(const Context & context, const BoardPos_t & boardPos)
            : PieceBase(context, Piece::Villan, boardPos)
        {}

        virtual ~VillanPiece() = default;

        bool takeTurn(Context & context, const sf::Event & event) override;

      protected:
        BoardPosOpt_t findTargetPos(const Context &);
        BoardPosOpt_t selectWhereToMove(const Context &, const BoardPos_t & targetPos);
    };
} // namespace boardgame

#endif // BOARDGAME_PIECES_HPP_INCLUDED