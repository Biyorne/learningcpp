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
        virtual ~IPiece() = default;

        virtual Piece piece() const = 0;
        virtual BoardPos_t position() const = 0;
        virtual sf::FloatRect bounds() const = 0;

        virtual void takeTurn(Context &) = 0;
        virtual void update(Context &, const float) = 0;
        virtual void handleEvent(Context &, const sf::Event &) = 0;
        void draw(sf::RenderTarget &, sf::RenderStates) const = 0;

        // never change m_position yourself, let this function keep in sync with the Board
        virtual void move(Context & context, const BoardPos_t & newPos) = 0;
    };

    //

    using IPieceOpt_t = std::optional<std::reference_wrapper<IPiece>>;
    using IPieceUPtr_t = std::unique_ptr<IPiece>;
    using IPieceUList_t = std::list<IPieceUPtr_t>;

    //

    class SimplePiece : public IPiece
    {
      public:
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
        BoardPos_t position() const override { return m_position; }
        sf::FloatRect bounds() const override { return m_sprite.getGlobalBounds(); }

        void takeTurn(Context &) override {}
        void update(Context &, const float) override {}
        void handleEvent(Context &, const sf::Event &) override {}
        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

        void move(Context & context, const BoardPos_t & newPos) override;

      protected:
        Piece m_piece;
        BoardPos_t m_position; // always use move() to change this to keep in sync with Board
        sf::Sprite m_sprite;
    };

    //

    class CellPiece : public SimplePiece
    {
      public:
        CellPiece(Context & context, const BoardPos_t & pos, const Piece piece);
        virtual ~CellPiece() = default;

        static sf::Sprite makeDefaultSprite(
            const Context & context, const BoardPos_t & boardPos, const Piece piece);
    };
} // namespace boardgame

#endif // BOARDGAME_PIECES_HPP_INCLUDED