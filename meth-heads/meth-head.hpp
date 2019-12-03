#ifndef METHHEADS_BASE_HPP_INCLUDED
#define METHHEADS_BASE_HPP_INCLUDED

#include "display-constants.hpp"
#include "pos-ref-counter.hpp"
#include "random.hpp"
#include "sim-context.hpp"
#include "utils.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <cassert>
#include <memory>
#include <string>
#include <vector>

namespace methhead
{
    // TODO in the end of all cleanup this should have a different home...
    //
    // also... TODO In the end, after cleaning up everything, there should be no more need for
    // "none" none/invalid options in enums is not always code smell, it is not always wrong,
    // but on the other hand, always adding none/invalid to your enums as a default/habit just
    // in case it is needed IS ALWAYS A BAD HABIT, becauase any enum that has a none/invalid
    // option that is not needed is often a huge source of errors and mess.
    enum class Motivation
    {
        none,
        lazy,
        greedy
    };

    //

    struct IActor
    {
        virtual ~IActor() = default;

        virtual int score() const noexcept = 0;
        virtual void score(const int value) noexcept = 0;

        virtual Motivation motivation() const noexcept = 0;
        virtual BoardPos_t boardPos() const noexcept = 0;

        virtual float turnDelaySec() const noexcept = 0;
        virtual void turnDelaySec(const float sec) noexcept = 0;

        virtual void update(const SimContext & context, const float elapsedMs) = 0;
    };

    using IActorUPtr_t = std::unique_ptr<IActor>;

    //

    struct IPickup
    {
        virtual ~IPickup() = default;

        virtual void changeActor(IActor & actor) = 0;
        virtual BoardPos_t boardPos() const = 0;
        virtual int value() const = 0;
    };

    using IPickupUPtr_t = std::unique_ptr<IPickup>;

    //

    class Loot
        : public IPickup
        , public BoardPositionHandler_t
    {
      public:
        explicit Loot(const SimContext & context)
            : BoardPositionHandler_t(context)
            , m_value(context.random.fromTo(1, 99))
        {}

        virtual ~Loot() = default;

        inline int value() const noexcept final { return m_value; }
        inline BoardPos_t boardPos() const noexcept final { return BoardPositionHandler_t::get(); }
        inline void changeActor(IActor & actor) override { actor.score(actor.score() + m_value); }

      private:
        int m_value;
    };

    //

    class MethHeadBase
        : public IActor
        , public BoardPositionHandler_t
    {
      protected:
        explicit MethHeadBase(const SimContext & context);
        virtual ~MethHeadBase() = default;

      public:
        void update(const SimContext & context, const float elapsedSec) override;

        inline int score() const noexcept final { return m_score; }
        inline void score(const int value) noexcept final { m_score = value; }

        inline float turnDelaySec() const noexcept final { return m_turnDelaySec; }
        inline void turnDelaySec(const float sec) noexcept final { m_turnDelaySec = sec; }

        inline BoardPos_t boardPos() const noexcept final { return BoardPositionHandler_t::get(); }

      protected:
        bool isTimeToMove(const float elapsedSec) noexcept;

        bool move(const SimContext & context);

        inline int walkDistanceBetween(const BoardPos_t & from, const BoardPos_t & to) const
            noexcept
        {
            const BoardPos_t posDiff(to - from);
            return (std::abs(posDiff.x) + std::abs(posDiff.y));
        }

        inline int walkDistanceTo(const BoardPos_t & to) const noexcept
        {
            return walkDistanceBetween(boardPos(), to);
        }

        void makeAllPossibleBoardMoves(const SimContext & context) const;

        // assumes there are pickups on the board
        virtual BoardPos_t findMostDesiredPickupBoardPos(const SimContext & context) const = 0;

      private:
        int m_score;
        float m_turnDelaySec;
        float m_turnDelaySoFarSec;

        // this was moved out of the makeAllPossibleBoardMoves() funciton only for runtime
        // optimization
        static inline std::vector<BoardPos_t> m_possibleMoves;

        static inline const float s_turnDelayDefaultSec{ 0.333f };
    };

    //

    class Lazy : public MethHeadBase
    {
      public:
        explicit Lazy(const SimContext & context)
            : MethHeadBase(context)
        {}

        virtual ~Lazy() = default;

        inline Motivation motivation() const noexcept final { return Motivation::lazy; }

      private:
        BoardPos_t findMostDesiredPickupBoardPos(const SimContext & context) const final
        {
            std::size_t bestIndex(std::numeric_limits<std::size_t>::max());
            std::size_t bestDistance(std::numeric_limits<std::size_t>::max());

            for (std::size_t i(0); i < context.pickups.size(); ++i)
            {
                const std::size_t distance{ static_cast<std::size_t>(
                    walkDistanceTo(context.pickups.at(i)->boardPos())) };

                if (distance < bestDistance)
                {
                    bestIndex = i;
                    bestDistance = distance;
                }
            }

            assert(bestIndex < context.pickups.size());
            assert(bestDistance <= context.display.cell_count);

            return context.pickups.at(bestIndex)->boardPos();
        }
    };

    //

    class Greedy : public MethHeadBase
    {
      public:
        explicit Greedy(const SimContext & context)
            : MethHeadBase(context)
        {}

        virtual ~Greedy() = default;

        inline Motivation motivation() const noexcept final { return Motivation::greedy; }

      private:
        BoardPos_t findMostDesiredPickupBoardPos(const SimContext & context) const final
        {
            std::size_t bestIndex(std::numeric_limits<std::size_t>::max());
            std::size_t bestValue(0);

            for (std::size_t i(0); i < context.pickups.size(); ++i)
            {
                const std::size_t value{ static_cast<std::size_t>(context.pickups.at(i)->value()) };

                if (value > bestValue)
                {
                    bestIndex = i;
                    bestValue = value;
                }
            }

            assert(bestIndex < context.pickups.size());
            assert(bestValue > 0);

            return context.pickups.at(bestIndex)->boardPos();
        }
    };
} // namespace methhead

#endif // METHHEADS_BASE_HPP_INCLUDED