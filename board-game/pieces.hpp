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
#include "util.hpp"

#include <array>
#include <iterator>
#include <list>
#include <memory>
#include <optional>
#include <set>
#include <tuple>

#include <SFML/Graphics.hpp>

namespace boardgame
{
    //

    struct IPiece : public IEntity
    {
        virtual ~IPiece() = default;

        virtual Piece piece() const = 0;
        virtual BoardPos_t position() const = 0;
        virtual sf::FloatRect bounds() const override = 0;

        virtual void takeTurn(Context &) = 0;
        virtual void update(Context &, const float) override = 0;
        virtual void handleEvent(Context &, const sf::Event &) override = 0;
        void draw(sf::RenderTarget &, sf::RenderStates) const override = 0;

        virtual void move(Context & context, const BoardPos_t & posNew) = 0;
    };

    //

    using IPieceOpt_t = std::optional<std::reference_wrapper<IPiece>>;
    using IPieceUPtr_t = std::unique_ptr<IPiece>;
    using IPieceUList_t = std::list<IPieceUPtr_t>;
    using IPieceUListIter_t = IPieceUList_t::iterator;

    //

    struct PieceBase : public IPiece
    {
        PieceBase(Context &, const Piece piece, const BoardPos_t & pos, const sf::Sprite & sprite)
            : m_piece(piece)
            , m_position(pos)
            , m_sprite(sprite)
        {}

        PieceBase(
            Context & context, const Piece piece, const BoardPos_t & pos, const sf::Color & color)
            : PieceBase(
                  context,
                  piece,
                  pos,
                  context.resources.makeDefaultSprite(context, piece, pos, color))
        {}

        virtual ~PieceBase() = default;

        Piece piece() const override { return m_piece; }
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
        Piece m_piece;
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
        {}

        virtual ~FoodPiece() = default;
    };

    //

    class TailPiece : public PieceBase
    {
      public:
        TailPiece(Context & context, const BoardPos_t & pos)
            : PieceBase(context, Piece::Tail, pos, sf::Color::Green)
        {}

        virtual ~TailPiece() = default;
    };

    //

    class HeadPiece : public PieceBase
    {
      public:
        HeadPiece(Context & context, const BoardPos_t & pos)
            : PieceBase(context, Piece::Head, pos, sf::Color(64, 255, 0))
            , m_directionKeyNext(sf::Keyboard::Up)
            , m_eatSfxPitch(0.4f)
            , m_eatSfxPitchAdj(0.033f)
            , m_timeBetweenTurnsElapsedSec(0.0f)
            , m_timeBetweenTurnsSec(0.1f)
            , m_timeBetweenTurnsShrinkRatio(0.96f)
        {}

        virtual ~HeadPiece() = default;

        void handleEvent(Context &, const sf::Event &) override;
        void update(Context & context, const float elapsedTimeSec) override;
        void takeTurn(Context & context) override;

        void handleDeathFromCollision(Context & context, const Piece piece);

        static BoardPos_t
            changePositionWithArrowKey(const BoardPos_t & pos, const sf::Keyboard::Key key);

        static bool isPositionNextToFood(Context & context, const BoardPos_t & pos);

      private:
        sf::Keyboard::Key m_directionKeyNext;
        float m_eatSfxPitch;
        float m_eatSfxPitchAdj;
        float m_timeBetweenTurnsElapsedSec;
        float m_timeBetweenTurnsSec;
        float m_timeBetweenTurnsShrinkRatio;
    };
} // namespace boardgame

#endif // BOARDGAME_PIECES_HPP_INCLUDED

/*
static void turnSeveredTailIntoWall(Context & context, const BoardPos_t & startingPos)
{
    int tailsTurnedIntoWallsCount{ 0 };

    while (!m_tailPositions.empty())
    {
        const BoardPos_t pos{ m_tailPositions.back() };
        m_tailPositions.pop_back();

        if (context.board.pieceEnumAt(pos) == Piece::Tail)
        {
            ++tailsTurnedIntoWallsCount;
            context.board.placePiece(context, Piece::Wall, pos);
        }

        if (pos == startingPos)
        {
            break;
        }
    }

    context.settings.adjustScore(-100 * tailsTurnedIntoWallsCount);
}

static sf::Keyboard::Key arrowKeyThatMoves(const BoardPos_t & from, const BoardPos_t & to)
{
    if (from.x < to.x)
    {
        return sf::Keyboard::Right;
    }
    else if (from.x > to.x)
    {
        return sf::Keyboard::Left;
    }
    else if (from.y > to.y)
    {
        return sf::Keyboard::Up;
    }
    else
    {
        return sf::Keyboard::Down;
    }
}

static std::tuple<bool, BoardPos_t, sf::Keyboard::Key> flip(Context & context)
{
    if (m_tailPositions.size() < 2)
    {
        return { false, {}, {} };
    }

    const BoardPos_t lastTailPos{ m_tailPositions.back() };
    m_tailPositions.pop_back();

    const sf::Keyboard::Key newDirection{ util::oppositeDirection(
        arrowKeyThatMoves(lastTailPos, m_tailPositions.back())) };

    context.board.removePiece(lastTailPos);
    std::reverse(std::begin(m_tailPositions), std::end(m_tailPositions));

    return { true, lastTailPos, newDirection };
}

*/