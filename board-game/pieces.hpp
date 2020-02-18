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
#include <set>
#include <tuple>

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

            // start with a copy of all valid/on-board positions
            std::vector<BoardPos_t> positions;
            positions.reserve(context.board.cells().count_total);

            for (int vert(0); vert < context.board.cells().counts_int.y; ++vert)
            {
                for (int horiz(0); horiz < context.board.cells().counts_int.x; ++horiz)
                {
                    positions.push_back(BoardPos_t{ horiz, vert });
                }
            }

            auto endIter{ std::end(positions) };

            // remove any that are alraedy occupied
            for (const IPieceUPtr_t & piece : context.board.pieces())
            {
                if (!piece->isInPlay())
                {
                    continue;
                }

                endIter =
                    std::remove_if(std::begin(positions), endIter, [&](const BoardPos_t & pos) {
                        return (pos == piece->position());
                    });
            }

            // remove any too close to the edges
            const int eatCount{ static_cast<int>(context.settings.foodEatenCount()) / 3 };
            const sf::Vector2i outerSize{ context.board.cells().counts_int / 4 };

            sf::IntRect allowedRect(
                outerSize, (context.board.cells().counts_int - (outerSize * 2)));

            allowedRect.left -= eatCount;
            allowedRect.top -= eatCount;
            allowedRect.width += (eatCount * 2);
            allowedRect.height += (eatCount * 2);

            endIter = std::remove_if(std::begin(positions), endIter, [&](const BoardPos_t & pos) {
                return !allowedRect.contains(pos);
            });

            positions.erase(endIter, std::end(positions));
            if (positions.empty())
            {
                return;
            }

            const BoardPos_t spawnPos{ context.random.from(positions) };
            context.board.addPiece(context, Piece::Food, spawnPos);
        }

      private:
        static inline std::size_t m_count{ 0 };
    };

    //

    class TailPiece : public PieceBase
    {
      public:
        TailPiece(Context & context, const BoardPos_t & pos)
            : PieceBase(context, Piece::Tail, pos, sf::Color::White)
            , m_colorLight(sf::Color(64, 255, 0))
            , m_colorDark((m_colorLight.r / 4), (m_colorLight.g / 4), (m_colorLight.b / 4))
        {
            context.settings.handleTailLengthIncrease();
            m_tailPositions.insert(std::begin(m_tailPositions), pos);
            updateColor();
        }

        virtual ~TailPiece() = default;

        static void reset() { m_tailPositions.clear(); }

        void update(Context &, const float) override { updateColor(); }

        void updateColor()
        {
            const float count{ static_cast<float>(tailLength() + 1) };

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

        static void removeLastPiece(Context & context)
        {
            while (!m_tailPositions.empty())
            {
                const BoardPos_t pos{ m_tailPositions.back() };
                m_tailPositions.pop_back();

                // this check makes good sense, but it prevent some fun features
                // M_ASSERT_OR_THROW(context.board.pieceEnumAt(pos) == Piece::Tail);
                if (context.board.pieceEnumAt(pos) == Piece::Tail)
                {
                    context.board.removeFromPlay(pos);
                    break;
                }
            }
        }

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
                    context.board.addPiece(context, Piece::Wall, pos);
                }

                if (pos == startingPos)
                {
                    break;
                }
            }

            context.settings.adjustScore(-100 * tailsTurnedIntoWallsCount);
        }

        static std::size_t tailLength() { return m_tailPositions.size(); }

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

            context.board.removeFromPlay(lastTailPos);
            std::reverse(std::begin(m_tailPositions), std::end(m_tailPositions));

            return { true, lastTailPos, newDirection };
        }

      private:
        sf::Color m_colorLight;
        sf::Color m_colorDark;

        static inline std::vector<BoardPos_t> m_tailPositions;
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

      private:
        sf::Keyboard::Key m_directionKeyPrev;
        sf::Keyboard::Key m_directionKeyNext;
        float m_timeElapsedSinceLastTurnSec;
        float m_eatSfxPitch;
        float m_eatSfxPitchAdj;
    };
} // namespace boardgame

#endif // BOARDGAME_PIECES_HPP_INCLUDED