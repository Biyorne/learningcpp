#ifndef METHHEADS_BASE_HPP_INCLUDED
#define METHHEADS_BASE_HPP_INCLUDED

#include "display-constants.hpp"
#include "enums.hpp"
#include "random.hpp"
#include "scoped-board-position.hpp"
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
    struct IPickup
    {
        virtual ~IPickup() = default;
        virtual int value() const noexcept = 0;
        virtual BoardPos_t boardPos() const noexcept = 0;
    };

    using IPickupUPtr_t = std::unique_ptr<IPickup>;

    //

    struct Loot
        : public IPickup
        , public ScopedBoardPosition
    {
        explicit Loot(const SimContext & context)
            : ScopedBoardPosition(context)
            , m_loot(context.random.fromTo(1, 99))
        {}

        virtual ~Loot() = default;

        int value() const noexcept final { return m_loot; }
        BoardPos_t boardPos() const noexcept final { return ScopedBoardPosition::get(); }

      private:
        int m_loot;
    };

    //

    struct IActor
    {
        virtual ~IActor() = default;

        virtual int score() const noexcept = 0;
        virtual BoardPos_t boardPos() const noexcept = 0;
        virtual Motivation motivation() const noexcept = 0;
        virtual void pickTarget(const SimContext & context) = 0;
        virtual void pickup(const SimContext & context, const IPickup & pickup) = 0;
        virtual bool update(const SimContext & context, const float elapsedMs) = 0;
    };

    using IActorUPtr_t = std::unique_ptr<IActor>;

    //

    class ActorBase
        : public IActor
        , public ScopedBoardPosition
    {
      protected:
        explicit ActorBase(const SimContext & context);

      public:
        virtual ~ActorBase() = default;

        BoardPos_t boardPos() const noexcept final { return ScopedBoardPosition::get(); }
        bool update(const SimContext & context, const float elapsedSec) override;
        inline int score() const noexcept final { return m_score; }

        void pickup(const SimContext &, const IPickup & pickup) override
        {
            m_score += pickup.value();
        }

      private:
        bool isTimeToMove(const float elapsedSec) noexcept;

        inline int walkDistanceTo(const BoardPos_t & to) const noexcept
        {
            return walkDistance(boardPos(), to);
        }

        bool move(const SimContext & context);

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

    struct Lazy : public ActorBase
    {
        explicit Lazy(const SimContext & context)
            : ActorBase(context)
        {}

        virtual ~Lazy() = default;

        inline Motivation motivation() const noexcept final { return Motivation::lazy; }

      private:
        void pickTarget(const SimContext & context) final
        {
            const BoardPos_t currPos{ boardPos() };

            if (context.pickups.empty())
            {
                m_targetBoardPos = currPos;
                return;
            }

            const auto foundIter = std::min_element(
                std::begin(context.pickups),
                std::end(context.pickups),
                [&](const IPickupUPtr_t & left, const IPickupUPtr_t & right) {
                    return (
                        walkDistance(currPos, left->boardPos()) <
                        walkDistance(currPos, right->boardPos()));
                });

            m_targetBoardPos = (*foundIter)->boardPos();
        }
    };

    //

    struct Greedy : public ActorBase
    {
        explicit Greedy(const SimContext & context)
            : ActorBase(context)
        {}

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