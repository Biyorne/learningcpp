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

        virtual float moveDelaySec() const noexcept = 0;
        virtual void moveDelaySec(const float sec) noexcept = 0;

        // returns true if the actor moved or needs attention from the Simulator
        virtual bool update(const SimContext & context, const float elapsedMs) = 0;

        virtual void pickTarget(const SimContext & context) = 0;
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

    class ActorBase
        : public IActor
        , public ScopedBoardPosHandler
    {
      protected:
        explicit ActorBase(const SimContext & context);
        virtual ~ActorBase() = default;

      public:
        bool update(const SimContext & context, const float elapsedSec) override;

        inline int score() const noexcept final { return m_score; }
        inline void score(const int value) noexcept final { m_score = value; }

        inline float moveDelaySec() const noexcept final { return m_moveDelaySec; }
        inline void moveDelaySec(const float sec) noexcept final { m_moveDelaySec = sec; }

        inline BoardPos_t boardPos() const noexcept final
        {
            return ScopedBoardPosHandler::getPos();
        }

        void pickTarget(const SimContext & context) override = 0;

      protected:
        bool isTimeToMove(const float elapsedSec) noexcept;

        bool move(const SimContext & context);

        inline int walkDistanceTo(const BoardPos_t & to) const noexcept
        {
            return walkDistance(boardPos(), to);
        }

      protected:
        BoardPos_t m_targetBoardPos;

      private:
        int m_score;
        float m_moveDelaySec;
        float m_turnDelaySoFarSec;

        static inline const float s_turnDelayDefaultSec{ 0.3f };

        // this vector was pulled from the move() funciton only as a runtime
        // optimization, so no other functions should read or write to this member
        static inline WalkDIstVec_t m_moves;
    };

    //

    class Lazy : public ActorBase
    {
      public:
        explicit Lazy(const SimContext & context)
            : ActorBase(context)
        {
            pickTarget(context);
        }

        virtual ~Lazy() = default;

        // TODO TEACH
        inline Motivation motivation() const noexcept final { return Motivation::lazy; }

      private:
        void pickTarget(const SimContext & context) final
        {
            const BoardPos_t pos{ boardPos() };

            if (context.pickups.empty())
            {
                m_targetBoardPos = pos;
                return;
            }

            const auto foundIter = std::min_element(
                std::begin(context.pickups),
                std::end(context.pickups),
                [&](const IPickupUPtr_t & left, const IPickupUPtr_t & right) {
                    return (
                        walkDistance(pos, left->boardPos()) < walkDistance(pos, right->boardPos()));
                });

            m_targetBoardPos = (*foundIter)->boardPos();
        }
    };

    //

    class Greedy : public ActorBase
    {
      public:
        explicit Greedy(const SimContext & context)
            : ActorBase(context)
        {
            pickTarget(context);
        }

        virtual ~Greedy() = default;

        inline Motivation motivation() const noexcept final { return Motivation::greedy; }

      private:
        void pickTarget(const SimContext & context) final
        {
            if (context.pickups.empty())
            {
                m_targetBoardPos = boardPos();
                return;
            }

            const auto foundIter = std::max_element(
                std::begin(context.pickups),
                std::end(context.pickups),
                [&](const IPickupUPtr_t & left, const IPickupUPtr_t & right) {
                    return (left->value() < right->value());
                });

            m_targetBoardPos = (*foundIter)->boardPos();
        }
    };
} // namespace methhead

#endif // METHHEADS_BASE_HPP_INCLUDED