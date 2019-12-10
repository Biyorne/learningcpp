#ifndef METHHEADS_BASE_HPP_INCLUDED
#define METHHEADS_BASE_HPP_INCLUDED

#include "display-constants.hpp"
#include "random.hpp"
#include "scoped-board-pos-handler.hpp"
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
        , public ScopedBoardPosHandler
    {
      public:
        explicit Loot(const SimContext & context)
            : ScopedBoardPosHandler(context)
            , m_value(context.random.fromTo(1, 99))
        {}

        virtual ~Loot() = default;

        inline int value() const noexcept final { return m_value; }

        inline BoardPos_t boardPos() const noexcept final
        {
            return ScopedBoardPosHandler::getPos();
        }

        inline void changeActor(IActor & actor) override { actor.score(actor.score() + m_value); }

      private:
        int m_value;
    };

    //

    class MethHeadBase
        : public IActor
        , public ScopedBoardPosHandler
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

        inline BoardPos_t boardPos() const noexcept final
        {
            return ScopedBoardPosHandler::getPos();
        }

      protected:
        bool isTimeToMove(const float elapsedSec) noexcept;

        void move(const SimContext & context);

        inline int walkDistanceTo(const BoardPos_t & to) const noexcept
        {
            return walkDistance(boardPos(), to);
        }

        WalkDIstVec_t::iterator findAllPossibleMovesEndIter(const SimContext & context);

        BoardPos_t
            findMostDesiredMovePos(const SimContext & context, WalkDIstVec_t::iterator & endIter);

        virtual BoardPos_t findMostDesiredPickup(const SimContext & context) const = 0;

      private:
        int m_score;
        float m_turnDelaySec;
        float m_turnDelaySoFarSec;

        static inline const float s_turnDelayDefaultSec{ 0.3f };

        // this vector was pulled from the move() funciton only as a runtime
        // optimization, so no other functions should read or write to this member
        static inline WalkDIstVec_t m_moves{ 4, WalkDistance{} };
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
        BoardPos_t findMostDesiredPickup(const SimContext & context) const final
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
        BoardPos_t findMostDesiredPickup(const SimContext & context) const final
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