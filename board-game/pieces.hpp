#ifndef BOARDGAME_PIECES_HPP_INCLUDED
#define BOARDGAME_PIECES_HPP_INCLUDED
//
// pieces.hpp
//
//
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
    /*
    using BoardPos_t = sf::Vector2i;
    using BoardPosOpt_t = std::optional<BoardPos_t>;

    //

    struct IPiece : public sf::Drawable
    {
        virtual ~IPiece() = default;

        virtual Piece piece() const = 0;
        virtual BoardPos_t position() const = 0;
        virtual sf::FloatRect bounds() const = 0;

        // should only be called by the Board, and be the only way to change m_position
        virtual void updateAfterMove(Context &, const BoardPos_t & newPos) = 0;

        virtual void takeTurn(Context &) = 0;
        virtual void update(Context &, const float) = 0;
        virtual void handleEvent(Context &, const sf::Event &) = 0;
        void draw(sf::RenderTarget &, sf::RenderStates) const = 0;
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
        // SimplePiece(Context &, const Piece, const BoardPos_t & pos, const sf::Color & color);

        virtual ~SimplePiece() = default;

        Piece piece() const override { return m_piece; }
        BoardPos_t position() const override { return m_position; }
        sf::FloatRect bounds() const override { return m_sprite.getGlobalBounds(); }

        // should only be called by the Board, and be the only way to change m_position
        void updateAfterMove(Context &, const BoardPos_t & newPos) override;

        void takeTurn(Context &) override {}
        void update(Context &, const float) override {}
        void handleEvent(Context &, const sf::Event &) override {}
        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

      protected:
        Piece m_piece;
        sf::Sprite m_sprite;

      private:
        // only the move() function should ever change this member
        BoardPos_t m_position;
    };

    //

    // class FoodPiece : public SimplePiece
    //{
    //  public:
    //    FoodPiece(Context & context, const BoardPos_t & pos, const Piece piece);
    //    virtual ~FoodPiece() = default;
    //};
    */
} // namespace boardgame

#endif // BOARDGAME_PIECES_HPP_INCLUDED
