#ifndef BOARDGAME_PIECES_HPP_INCLUDED
#define BOARDGAME_PIECES_HPP_INCLUDED
//
// pieces.hpp
//
#include "board.hpp"
#include "context.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "settings.hpp"
#include "types.hpp"

#include <array>
#include <memory>
#include <optional>

#include <SFML/Graphics.hpp>

namespace boardgame
{
    //

    struct IPiece : public sf::Drawable
    {
        virtual ~IPiece() = default;

        virtual bool isInPlay() const = 0;
        virtual void removeFromPlay() = 0;

        virtual Piece::Enum piece() const = 0;
        virtual BoardPos_t boardPos() const = 0;
        virtual sf::FloatRect bounds() const = 0;

        virtual void move(const Context & context, const BoardPos_t & posNew) = 0;

        virtual void takeTurn(Context &) = 0;
        virtual void handleEvent(Context &, const sf::Event &) = 0;
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
            : m_isInPlay(true)
            , m_piece(piece)
            , m_boardPos(boardPos)
            , m_sprite(sprite)
        {}

        PieceBase(
            const Context & context,
            const Piece::Enum piece,
            const BoardPos_t & boardPos,
            const sf::Color & color)
            : PieceBase(
                  context,
                  piece,
                  boardPos,
                  context.resources.sprite(context, piece, boardPos, color))
        {}

        virtual ~PieceBase() = default;

        bool isInPlay() const override { return m_isInPlay; }
        void removeFromPlay() override { m_isInPlay = false; }

        Piece::Enum piece() const override { return m_piece; }
        BoardPos_t boardPos() const override { return m_boardPos; }
        sf::FloatRect bounds() const override { return m_sprite.getGlobalBounds(); }

        void takeTurn(Context &) override {}
        void update(Context &, const float) override {}
        void handleEvent(Context &, const sf::Event &) override {}

        void move(const Context & context, const BoardPos_t & posNew) override;
        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

        // virtual void move(const Context & context, const BoardPos_t & targetPos);
        //
        // static FourMovesArray_t
        //    movesVec_ResetToAllFourDirections(const Context & context, const BoardPos_t &
        //    fromPos);
        //
        // static Moves_t makeAllValidMoves(const Context & context, const BoardPos_t & targetPos);

      protected:
        bool m_isInPlay;
        Piece::Enum m_piece;
        BoardPos_t m_boardPos;
        sf::Sprite m_sprite;
    };

    //

    struct WallPiece : public PieceBase
    {
        WallPiece(const Context & context, const BoardPos_t & boardPos)
            : PieceBase(context, Piece::Wall, boardPos, sf::Color(100, 60, 5))
        {}

        virtual ~WallPiece() = default;
    };

    //

    struct FoodPiece : public PieceBase
    {
        FoodPiece(const Context & context, const BoardPos_t & boardPos)
            : PieceBase(context, Piece::Food, boardPos, sf::Color::Yellow)
        {}

        virtual ~FoodPiece() = default;
    };

    //

    class TailPiece : public PieceBase
    {
      public:
        TailPiece(const Context & context, const BoardPos_t & boardPos)
            : PieceBase(context, Piece::Tail, boardPos, sf::Color(32, 192, 0))
            , m_id(context.settings.consumeNextTailId())
        {}

        virtual ~TailPiece() = default;

        void takeTurn(Context & context) override;

      protected:
        std::size_t m_id;
    };

    //

    class HeadPiece : public PieceBase
    {
      public:
        HeadPiece(const Context & context, const BoardPos_t & boardPos)
            : PieceBase(context, Piece::Head, boardPos, sf::Color(0, 255, 0))
            , m_directionKeyNext(sf::Keyboard::Up) // games always start with snake moving up
        {}

        virtual ~HeadPiece() = default;

        void takeTurn(Context & context) override;
        void update(Context & context, const float) override;
        void move(const Context & context, const BoardPos_t & posNew) override;

      protected:
        sf::Keyboard::Key m_directionKeyNext;
    };
} // namespace boardgame

#endif // BOARDGAME_PIECES_HPP_INCLUDED