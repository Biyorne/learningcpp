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

        virtual bool isAlive() const = 0;
        virtual void kill() = 0;

        virtual Piece::Enum piece() const = 0;
        virtual BoardPos_t boardPos() const = 0;
        virtual sf::FloatRect bounds() const = 0;

        // returns true if turn is finished and the next piece can take their turn
        virtual bool takeTurn(Context &) = 0;

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
            : m_isAlive(true)
            , m_piece(piece)
            , m_boardPos(boardPos)
            , m_sprite(sprite)
        {}

        PieceBase(
            const Context & context,
            const Piece::Enum piece,
            const BoardPos_t & boardPos,
            const sf::Color & color = sf::Color::White)
            : PieceBase(context, piece, boardPos, makeSprite(context, piece, boardPos, color))
        {}

        virtual ~PieceBase() = default;

        bool isAlive() const override { return m_isAlive; }
        void kill() override { m_isAlive = false; }

        Piece::Enum piece() const override { return m_piece; }
        BoardPos_t boardPos() const override { return m_boardPos; }
        sf::FloatRect bounds() const override { return m_sprite.getGlobalBounds(); }

        // default is to do nothing and let the next piece take its turn, even if dead
        bool takeTurn(Context &) override { return true; }

        void handleEvent(Context &, const sf::Event &) override {}

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            if (!isAlive())
            {
                return;
            }

            target.draw(m_sprite, states);
        }

        void update(Context &, const float) override {}

      protected:
        virtual sf::Sprite makeSprite(
            const Context & context,
            const Piece::Enum piece,
            const BoardPos_t & boardPos,
            const sf::Color & color) const;

        // virtual void move(const Context & context, const BoardPos_t & targetPos);
        //
        // static FourMovesArray_t
        //    movesVec_ResetToAllFourDirections(const Context & context, const BoardPos_t &
        //    fromPos);
        //
        // static Moves_t makeAllValidMoves(const Context & context, const BoardPos_t & targetPos);

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

    struct HeadPiece : public PieceBase
    {
        HeadPiece(const Context & context, const BoardPos_t & boardPos)
            : PieceBase(context, Piece::SnakeHead, boardPos, sf::Color(0, 255, 0))
        {}

        virtual ~HeadPiece() = default;
    };

    //

    struct BodyPiece : public PieceBase
    {
        BodyPiece(const Context & context, const BoardPos_t & boardPos)
            : PieceBase(context, Piece::SnakeBody, boardPos, sf::Color(32, 192, 0))
        {}

        virtual ~BodyPiece() = default;
    };

    // struct PlayerPiece : public PieceBase
    //{
    //    PlayerPiece(const Context & context, const BoardPos_t & boardPos)
    //        : PieceBase(context, Piece::Player, boardPos)
    //    {}
    //
    //    virtual ~PlayerPiece() = default;
    //
    //    bool takeTurn(Context & context, const sf::Event & event) override;
    //
    //  private:
    //    bool attemptMove(Context & context, const BoardPos_t & targetPos);
    //    BoardPos_t walkPosFromArrowKeypress_Normal(Context & context, const sf::Event &
    //    eventOrig); BoardPos_t walkPosFromArrowKeypress_Random(Context & context, const sf::Event
    //    & eventOrig);
    //};

    //

    // these are pieces that take turns, either the player or np/ai
    // struct VillanPiece : public PieceBase
    //{
    //    VillanPiece(const Context & context, const BoardPos_t & boardPos)
    //        : PieceBase(context, Piece::Villan, boardPos)
    //    {}
    //
    //    virtual ~VillanPiece() = default;
    //
    //    bool takeTurn(Context & context, const sf::Event & event) override;
    //
    //  protected:
    //    BoardPosOpt_t findTargetPos(const Context &);
    //    BoardPosOpt_t selectWhereToMove(const Context &, const BoardPos_t & targetPos);
    //};
} // namespace boardgame

#endif // BOARDGAME_PIECES_HPP_INCLUDED