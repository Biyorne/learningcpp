#ifndef BOARDGAME_PIECES_HPP_INCLUDED
#define BOARDGAME_PIECES_HPP_INCLUDED
//
// pieces.hpp
//
//
#include "board.hpp"
#include "context.hpp"
#include "types.hpp"
#include "util.hpp"

#include <list>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include <SFML/Graphics.hpp>

namespace boardgame
{
    using BoardPos_t = sf::Vector2i;
    using BoardPosOpt_t = std::optional<BoardPos_t>;

    //

    struct IPiece : public sf::Drawable
    {
        friend IBoard;
        virtual ~IPiece() = default;

        virtual Piece piece() const = 0;
        virtual BoardPos_t position() const = 0;
        virtual sf::FloatRect bounds() const = 0;

        virtual void takeTurn(Context &) = 0;
        virtual void update(Context &, const float) = 0;
        virtual void handleEvent(Context &, const sf::Event &) = 0;
        void draw(sf::RenderTarget &, sf::RenderStates) const = 0;

        virtual void move(Context & context, const BoardPos_t & newPos) = 0;
    };

    //

    using IPieceOpt_t = std::optional<std::reference_wrapper<IPiece>>;
    using IPieceUPtr_t = std::unique_ptr<IPiece>;
    using IPieceUList_t = std::list<IPieceUPtr_t>;

    // This (very simple) game engine will fail if two Pieces are ever in the same BoardPos_t at the
    // same time.  Ensuring that never happens is accomplished by:
    //  - ensuring that only the Board class can add/remove/move Pieces
    //  - This BoardPosKeeper Piece base class prevents Pieces from cheating and changing their pos.
    //  - automatically erasing and deleting any piece that gets stepped on by another's move
    class BoardPosKeeper
    {
        friend IBoard;

      public:
        BoardPosKeeper(const BoardPos_t & pos)
            : m_position(pos)
        {}

        virtual ~BoardPosKeeper() = default;

        BoardPos_t get() const { return m_position; }

      private:
        void set(const BoardPos_t & newPos) { m_position = newPos; }

      private:
        BoardPos_t m_position;
    };

    //

    struct SimplePiece
        : public IPiece
        , public BoardPosKeeper
    {
        SimplePiece(Context &, const Piece piece, const BoardPos_t & pos);
        SimplePiece(Context &, const Piece, const BoardPos_t & pos, const sf::Sprite & sprite);

        SimplePiece(
            Context &,
            const Piece,
            const BoardPos_t & pos,
            const sf::Color & color,
            const bool willSkewToFitExactly);

        virtual ~SimplePiece() = default;

        Piece piece() const override { return m_piece; }
        BoardPos_t position() const override { return BoardPosKeeper::get(); }
        sf::FloatRect bounds() const override { return m_sprite.getGlobalBounds(); }

        void takeTurn(Context &) override {}
        void update(Context &, const float) override {}
        void handleEvent(Context &, const sf::Event &) override {}
        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

        void move(Context & context, const BoardPos_t & newPos) override;

      protected:
        Piece m_piece;
        sf::Sprite m_sprite;
    };

    //

    class CellPiece : public SimplePiece
    {
        friend IBoard;

      public:
        CellPiece(Context & context, const BoardPos_t & pos, const Piece piece);
        virtual ~CellPiece() = default;

        // In this game, all cells are the same type (CellPiece) but they change their Piece::enum
        // from on to off.  Since the takeTurn() function is not used, is has been re-purposed to
        // perform the on/off toggle.
        void takeTurn(Context &) override;
    };
} // namespace boardgame

#endif // BOARDGAME_PIECES_HPP_INCLUDED