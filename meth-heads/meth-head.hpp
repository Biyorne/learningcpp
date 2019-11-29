#ifndef METHHEADS_BASE_HPP_INCLUDED
#define METHHEADS_BASE_HPP_INCLUDED

#include "display-constants.hpp"
#include "random.hpp"
#include "sim-context.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <cassert>
#include <memory>
#include <optional>
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

        virtual int score() const = 0;
        virtual void score(const int value) = 0;

        virtual Motivation motivation() const = 0;
        virtual BoardPos_t boardPos() const = 0;

        virtual float timeBetweenMovesSec() const = 0;
        virtual void timeBetweenMovesSec(const float sec) = 0;

        virtual void update(const float elapsedMs, const SimContext & context) = 0;
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

    class Loot : public IPickup
    {
      public:
        Loot(const BoardPos_t & boardPos, const int value)
            : m_value(value)
            , m_boardPos(boardPos)
        {}

        virtual ~Loot() = default;

        inline int value() const final { return m_value; }
        inline BoardPos_t boardPos() const final { return m_boardPos; }
        inline void changeActor(IActor & actor) override { actor.score(actor.score() + m_value); }

      private:
        int m_value;
        BoardPos_t m_boardPos;
    };

    //

    class MethHeadBase : public IActor
    {
      protected:
        explicit MethHeadBase(
            const BoardPos_t & boardPos,
            const float waitBetweenActionsSec = m_waitBetweenActionsSecDefault);

        virtual ~MethHeadBase() = default;

      public:
        void update(const float elapsedSec, const SimContext & context) override;

        inline int score() const final { return m_score; }
        inline void score(const int value) final { m_score = value; }

        inline float timeBetweenMovesSec() const final { return m_waitBetweenActionsSec; }
        inline void timeBetweenMovesSec(const float sec) final { m_waitBetweenActionsSec = sec; }

        inline BoardPos_t boardPos() const final { return m_boardPos; }

      protected:
        bool isTimeToMove(const float elapsedSec);

        void move(const SimContext & context);

        inline int walkDistanceBetween(const BoardPos_t & from, const BoardPos_t & to) const
        {
            const BoardPos_t posDiff(to - from);
            return (std::abs(posDiff.x) + std::abs(posDiff.y));
        }

        inline int walkDistanceTo(const BoardPos_t & to) const
        {
            return walkDistanceBetween(m_boardPos, to);
        }

        std::vector<BoardPos_t> makeAllPossibleBoardMoves(const SimContext & context) const;

        virtual std::optional<BoardPos_t>
            findMostDesiredPickupBoardPos(const SimContext & context) const = 0;

      private:
        int m_score;
        BoardPos_t m_boardPos;
        float m_waitBetweenActionsSec;
        float m_elapsedSinceLastActionSec;

        static inline const float m_waitBetweenActionsSecDefault{ 0.333f };
    };

    //

    class Lazy : public MethHeadBase
    {
      public:
        explicit Lazy(const BoardPos_t & boardPos)
            : MethHeadBase(boardPos)
        {}

        virtual ~Lazy() = default;

        inline Motivation motivation() const final { return Motivation::lazy; }

      private:
        std::optional<BoardPos_t>
            findMostDesiredPickupBoardPos(const SimContext & context) const final
        {
            std::size_t bestIndex(std::numeric_limits<int>::max());
            int bestDistance(std::numeric_limits<int>::max());

            for (std::size_t i(0); i < context.pickups.size(); ++i)
            {
                const int distance{ walkDistanceTo(context.pickups.at(i)->boardPos()) };

                if (distance < bestDistance)
                {
                    bestIndex = i;
                    bestDistance = distance;
                }
            }

            if ((bestIndex >= context.pickups.size()) ||
                (bestDistance == std::numeric_limits<int>::max()))
            {
                return std::nullopt;
            }

            return context.pickups.at(bestIndex)->boardPos();
        }
    };

    //

    class Greedy : public MethHeadBase
    {
      public:
        explicit Greedy(const BoardPos_t & boardPos)
            : MethHeadBase(boardPos)
        {}

        virtual ~Greedy() = default;

        inline Motivation motivation() const final { return Motivation::greedy; }

      private:
        std::optional<BoardPos_t>
            findMostDesiredPickupBoardPos(const SimContext & context) const final
        {
            std::size_t bestIndex(std::numeric_limits<int>::max());
            int bestValue(-1);

            for (std::size_t i(0); i < context.pickups.size(); ++i)
            {
                const int value{ context.pickups.at(i)->value() };

                if (value > bestValue)
                {
                    bestIndex = i;
                    bestValue = value;
                }
            }

            if ((bestIndex >= context.pickups.size()) || (bestValue < 0))
            {
                return std::nullopt;
            }

            return context.pickups.at(bestIndex)->boardPos();
        }
    };
} // namespace methhead

#endif // METHHEADS_BASE_HPP_INCLUDED