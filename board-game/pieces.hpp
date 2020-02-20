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

    struct IPiece : public sf::Drawable
    {
        virtual ~IPiece() = default;

        // don't call this yourself, let the Board call this after adding it
        virtual void setup(Context &) = 0;

        // don't call this yourself, let the Board call this just before removing it
        virtual void teardown(Context &) = 0;

        virtual Piece::Enum which() const = 0;
        virtual BoardPos_t position() const = 0;
        virtual sf::FloatRect bounds() const = 0;

        virtual void takeTurn(Context &) = 0;
        virtual void update(Context &, const float) = 0;
        virtual void handleEvent(Context &, const sf::Event &) = 0;
        void draw(sf::RenderTarget &, sf::RenderStates) const override = 0;

        // don't change m_position yourself, use this every time
        virtual void move(Context & context, const BoardPos_t & posNew) = 0;
    };

    //

    using IPieceOpt_t = std::optional<std::reference_wrapper<IPiece>>;
    using IPieceUPtr_t = std::unique_ptr<IPiece>;
    using IPieceUList_t = std::list<IPieceUPtr_t>;

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

        void setup(Context &) override
        {
            M_ASSERT_OR_THROW(!m_hasTeardown);
            m_hasSetup = true;
        }
        void teardown(Context &) override
        {
            M_ASSERT_OR_THROW(m_hasSetup);
            M_ASSERT_OR_THROW(!m_hasTeardown);
            m_hasTeardown = true;
        }

        Piece::Enum which() const override { return m_enum; }
        BoardPos_t position() const override { return m_position; }
        sf::FloatRect bounds() const override { return m_sprite.getGlobalBounds(); }

        void takeTurn(Context &) override {}
        void update(Context &, const float) override {}
        void handleEvent(Context &, const sf::Event &) override {}

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            M_ASSERT_OR_THROW(m_hasSetup);
            M_ASSERT_OR_THROW(!m_hasTeardown);
            target.draw(m_sprite, states);
        }

        void move(Context & context, const BoardPos_t & posNew) override;

      protected:
        Piece::Enum m_enum;
        BoardPos_t m_position;
        sf::Sprite m_sprite;

        // remove after testing TODO
        bool m_hasSetup{ false };
        bool m_hasTeardown{ false };
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

        void teardown(Context & context) override
        {
            context.board.placePieceAtRandomPos(context, Piece::Food);
        }
    };

    //

    class TailPiece : public PieceBase
    {
      public:
        TailPiece(Context & context, const BoardPos_t & pos)
            : PieceBase(context, Piece::Tail, pos, sf::Color::White)
            , m_timeElapsedSinceLastTurnSec(0.0f)
        {}

        virtual ~TailPiece() = default;

        static void reset() { m_tailPositions.clear(); }

        void setup(Context & context) override
        {
            PieceBase::setup(context);
            context.settings.handleTailLengthIncrease();
            m_tailPositions.push_front(position());
            updateColor();
        }

        void update(Context & context, const float frameTimeSec) override
        {
            m_timeElapsedSinceLastTurnSec += frameTimeSec;
            if (m_timeElapsedSinceLastTurnSec > context.settings.timeBetweenTurnsSec())
            {
                m_timeElapsedSinceLastTurnSec -= context.settings.timeBetweenTurnsSec();

                updateColor();
            }
        }

        static void removeLastTailPiece(Context & context)
        {
            if (m_tailPositions.empty())
            {
                return;
            }

            const BoardPos_t pos{ m_tailPositions.back() };
            m_tailPositions.pop_back();

            if (context.board.pieceEnumAt(pos) != Piece::Tail)
            {
                return;
            }

            context.board.removePiece(context, pos);
        }

        static std::size_t tailLength() { return m_tailPositions.size(); }

      private:
        void updateColor()
        {
            const float count{ static_cast<float>(m_tailPositions.size() + 1) };

            float index{ 0.0f };
            for (const BoardPos_t & pos : m_tailPositions)
            {
                if (position() == pos)
                {
                    break;
                }
                else
                {
                    index += 1.0f;
                }
            }

            const float ratio{ index / count };

            m_sprite.setColor(util::colorBlend(ratio, m_colorLight, m_colorDark));
        }

      private:
        float m_timeElapsedSinceLastTurnSec;

        static inline sf::Color m_colorLight{ 64, 255, 0 };

        static inline sf::Color m_colorDark{ static_cast<sf::Uint8>(m_colorLight.r / 4),
                                             static_cast<sf::Uint8>(m_colorLight.g / 4),
                                             static_cast<sf::Uint8>(m_colorLight.b / 4) };

        static inline std::list<BoardPos_t> m_tailPositions;
    };

    //

    class HeadPiece : public PieceBase
    {
      public:
        HeadPiece(Context & context, const BoardPos_t & pos)
            : PieceBase(context, Piece::Head, pos, sf::Color(64, 255, 0))
            , m_directionKeyPrev(sf::Keyboard::Up)
            , m_directionKeyNext(sf::Keyboard::Up)
            , m_timeElapsedSinceLastTurnSec(0.0f)
            , m_eatSfxPitch(0.4f)
            , m_eatSfxPitchAdj(0.033f)
        {}

        virtual ~HeadPiece() = default;

        void handleEvent(Context &, const sf::Event &) override;
        void update(Context & context, const float frameTimeSec) override;

        void changeDirectionIfArrowKeyIsPressed(Context & context);

        void takeTurn(Context & context) override;

        void handleDeathFromCollision(Context & context, const Piece::Enum which);

        static BoardPos_t
            changePositionWithArrowKey(const BoardPos_t & pos, const sf::Keyboard::Key key);

        static bool isPositionNextToFood(Context & context, const BoardPos_t & pos);

        bool flip(Context & context);

        sf::Keyboard::Key selfTestDirectionSet(Context & context);
        BoardPos_t pickTarget(Context & context);

      private:
        sf::Keyboard::Key m_directionKeyPrev;
        sf::Keyboard::Key m_directionKeyNext;
        float m_timeElapsedSinceLastTurnSec;
        float m_eatSfxPitch;
        float m_eatSfxPitchAdj;

        BoardPos_t m_selfTestTargretPos{ 2, 2 };
        std::size_t m_selfTestMoveCount{ 0 };
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