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

        virtual Piece::Enum which() const = 0;
        virtual BoardPos_t position() const = 0;
        virtual sf::FloatRect bounds() const = 0;

        virtual void takeTurn(Context &) = 0;
        virtual void update(Context &, const float) = 0;
        virtual void handleEvent(Context &, const sf::Event &) = 0;
        void draw(sf::RenderTarget &, sf::RenderStates) const override = 0;

        virtual void move(Context & context, const BoardPos_t & posNew) = 0;
    };

    //

    using IPieceOpt_t = std::optional<std::reference_wrapper<IPiece>>;
    using IPieceUPtr_t = std::unique_ptr<IPiece>;
    using IPieceUVec_t = std::vector<IPieceUPtr_t>;

    //

    struct PieceBase : public IPiece
    {
        PieceBase(
            Context &, const Piece::Enum which, const BoardPos_t & pos, const sf::Sprite & sprite)
            : m_enum(which)
            , m_position(pos)
            , m_sprite(sprite)
        {}

        PieceBase(
            Context & context,
            const Piece::Enum piece,
            const BoardPos_t & pos,
            const sf::Color & color)
            : PieceBase(context, piece, pos, context.resources.sprite(context, piece, pos, color))
        {}

        virtual ~PieceBase() = default;

        bool isInPlay() const override { return (which() != Piece::NotInPlay); }
        void removeFromPlay() override
        {
            M_ASSERT_OR_THROW(isInPlay());
            m_enum = Piece::NotInPlay;
        }

        Piece::Enum which() const override { return m_enum; }
        BoardPos_t position() const override { return m_position; }
        sf::FloatRect bounds() const override { return m_sprite.getGlobalBounds(); }

        void takeTurn(Context &) override {}
        void update(Context &, const float) override {}
        void handleEvent(Context &, const sf::Event &) override {}

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            target.draw(m_sprite, states);
        }

        void move(Context & context, const BoardPos_t & posNew) override;

      protected:
        Piece::Enum m_enum;
        BoardPos_t m_position;
        sf::Sprite m_sprite;
    };

    //

    struct WallPiece : public PieceBase
    {
        WallPiece(Context & context, const BoardPos_t & pos)
            : PieceBase(context, Piece::Wall, pos, sf::Color(100, 60, 5))
        {}

        virtual ~WallPiece() = default;
    };

    //

    struct FoodPiece : public PieceBase
    {
        FoodPiece(Context & context, const BoardPos_t & pos)
            : PieceBase(context, Piece::Food, pos, sf::Color::Yellow)
        {
            ++m_count;
        }

        virtual ~FoodPiece() { --m_count; }

        static void spawn(Context & context)
        {
            if (m_count > 0)
            {
                return;
            }

            const BoardPosOpt_t posOpt{ context.board.findRandomEmptyPos(context) };
            if (!posOpt)
            {
                return;
            }

            context.board.addPiece(context, Piece::Food, posOpt.value());
        }

      private:
        static inline std::size_t m_count{ 0 };
    };

    //

    class TailPiece : public PieceBase
    {
      public:
        TailPiece(Context & context, const BoardPos_t & pos)
            : PieceBase(context, Piece::Tail, pos, sf::Color(32, 192, 0))
        {
            context.settings.handleTailLengthIncrease();
            m_tailPositions.insert(std::begin(m_tailPositions), pos);
        }

        virtual ~TailPiece() = default;

        static void reset() { m_tailPositions.clear(); }

        static void removeLastPiece(Context & context)
        {
            if (m_tailPositions.empty())
            {
                return;
            }

            const BoardPos_t pos{ m_tailPositions.back() };
            m_tailPositions.pop_back();

            M_ASSERT_OR_THROW(context.board.pieceEnumAt(pos) == Piece::Tail);
            context.board.removeFromPlay(pos);
        }

        static std::size_t tailLength() { return m_tailPositions.size(); }

      private:
        static inline std::vector<BoardPos_t> m_tailPositions;
    };

    //

    class HeadPiece : public PieceBase
    {
      public:
        HeadPiece(Context & context, const BoardPos_t & pos)
            : PieceBase(context, Piece::Head, pos, sf::Color(0, 255, 0))
            , m_directionKeyPrev(sf::Keyboard::Up)
            , m_directionKeyNext(sf::Keyboard::Up)
            , m_timeElapsedSinceLastTurnSec(0.0f)
        {}

        virtual ~HeadPiece() = default;

        void handleEvent(Context &, const sf::Event &) override;
        void update(Context & context, const float frameTimeSec) override;

        void changeDirectionIfArrowKeyIsPressed(Context & context);
        void changeDirection(const sf::Keyboard::Key newDirectionKey);

        void takeTurn(Context & context) override;

        static BoardPos_t
            changePositionWithArrowKey(const BoardPos_t & pos, const sf::Keyboard::Key key);

        static bool isPositionNextToFood(Context & context, const BoardPos_t & pos);

      private:
        sf::Keyboard::Key m_directionKeyPrev;
        sf::Keyboard::Key m_directionKeyNext;
        float m_timeElapsedSinceLastTurnSec;
    };
} // namespace boardgame

#endif // BOARDGAME_PIECES_HPP_INCLUDED