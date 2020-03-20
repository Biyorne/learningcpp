#ifndef SIMPLE_EFFECTS_MOVEMENT_HPP_INCLUDED
#define SIMPLE_EFFECTS_MOVEMENT_HPP_INCLUDED

#include "util.hpp"

#include <SFML/Graphics.hpp>

namespace entity
{
    struct MovementVector
    {
        MovementVector() = default;

        explicit MovementVector(const sf::Vector2f & vel)
            : vector(vel)
        {}

        sf::Vector2f updateDelta(const float frameTimeSec) const { return (vector * frameTimeSec); }

        sf::Vector2f updateAbsolute(const float frameTimeSec, const sf::Vector2f & pos) const
        {
            return (pos + updateDelta(frameTimeSec));
        }

        sf::Vector2f vector{ 0.0f, 0.0f };
    };

    //

    using BaseRatios_t = sf::Vector2f;
    using RangeRatios_t = sf::Vector2f;

    struct MoverRatios
    {
        BaseRatios_t velocity;
        BaseRatios_t acceleration;
        float speed_limit = { 0.0f };
    };

    struct MoverRanges
    {
        MoverRanges() = default;

        MoverRanges(const float vel, const float acc)
            : MoverRanges(vel, acc, vel)
        {}

        MoverRanges(const float vel, const float acc, const float spd)
            : velocity(vel, vel)
            , acceleration(acc, acc)
            , speed_limit(spd)
        {}

        RangeRatios_t velocity;
        RangeRatios_t acceleration;
        float speed_limit = { 0.0f };
    };

    //

    struct MoverFactory;

    class Mover
    {
        friend MoverFactory;

      public:
        Mover() = default;

        explicit Mover(
            const sf::Vector2f & vel,
            const sf::Vector2f & acc = { 0.0f, 0.0f },
            const float speedLimit = { 0.0f })
        {
            setup(vel, acc, speedLimit);
        }

        virtual ~Mover() = default;

        virtual void setup(
            const sf::Vector2f & vel,
            const sf::Vector2f & acc = { 0.0f, 0.0f },
            const float speedLimit = { 0.0f })
        {
            m_velocity.vector = vel;
            m_acceleration.vector = acc;
            m_speedLimit = speedLimit;
        }

        virtual void aim(const sf::Vector2f & from, const sf::Vector2f & to)
        {
            m_acceleration.vector =
                util::vectorDirectionOnlySetFromTo(m_acceleration.vector, from, to);
        }

        virtual sf::Vector2f updateDelta(const float frameTimeSec)
        {
            m_velocity.vector += m_acceleration.updateDelta(frameTimeSec);

            if (m_speedLimit > 0.0f)
            {
                m_velocity.vector = util::vectorMagnitudeLimit(m_velocity.vector, m_speedLimit);
            }

            return m_velocity.updateDelta(frameTimeSec);
        }

        virtual sf::Vector2f updateAbsolute(const float frameTimeSec, const sf::Vector2f & pos)
        {
            return (pos + updateDelta(frameTimeSec));
        }

        // const MovementVector & velocity() { return m_velocity; }
        //
        // const MovementVector & acceleration() { return m_acceleration; }
        //
        // const float & speedLimit() { return m_speedLimit; }

      protected:
        MovementVector m_velocity;
        MovementVector m_acceleration;
        float m_speedLimit{ 0.0f };
    };

    //

    // too many constructors, lets give them names
    // There is too much code required to make a Mover.
    struct MoverFactory
    {
        static Mover makeFromRatios(const float base, const MoverRatios & ratios)
        {
            return Mover(
                (base * ratios.velocity),
                (base * ratios.acceleration),
                (base * ratios.speed_limit));
        }

        static Mover makeFromRatios(
            const float base,
            const BaseRatios_t & velocity,
            const BaseRatios_t & acceleration = { 0.0f, 0.0f },
            const float spdLimit = { 0.0f })
        {
            const MoverRatios ratios{ velocity, acceleration, spdLimit };
            return makeFromRatios(base, ratios);
        }

        static Mover makeFromRanges(
            const Context & context,
            const float base,
            const MoverRatios & ratios,
            const MoverRanges & ranges)
        {
            Mover mover(makeFromRatios(base, ratios));

            getRandomRange(context, mover.m_velocity.vector, base, ranges.velocity);
            getRandomRange(context, mover.m_acceleration.vector, base, ranges.acceleration);
            getRandomRange(context, mover.m_speedLimit, base, ranges.speed_limit);

            return mover;
        }

      private:
        static void getRandomRange(
            const Context & context, float & midpoint, const float base, const float ratioOfBase)
        {
            const float rangeHalf(std::abs((base * ratioOfBase) / 2.0f));
            midpoint = context.random.fromTo((midpoint - rangeHalf), (midpoint + rangeHalf));
        }

        static void getRandomRange(
            const Context & context,
            sf::Vector2f & midpoints,
            const float base,
            const RangeRatios_t & ranges)
        {
            getRandomRange(context, midpoints.x, base, ranges.x);
            getRandomRange(context, midpoints.y, base, ranges.y);
        }
    };

    //

    struct BounceResult
    {
        sf::Vector2f pos_delta{ 0.0f, 0.0f };
        sf::Vector2f velocity{ 0.0f, 0.0f };
    };

    //

    struct Fence
    {
        Fence() = default;

        explicit Fence(const sf::FloatRect & bnd)
            : bounds(bnd)
        {}

        // Returns change to entityBounds position
        sf::Vector2f updateDelta(const sf::FloatRect & entityBounds) const
        {
            sf::Vector2f posDelta(0.0f, 0.0f);

            if (entityBounds.left < bounds.left)
            {
                posDelta.x = (bounds.left - entityBounds.left);
            }

            if (entityBounds.top < bounds.top)
            {
                posDelta.y = (bounds.top - entityBounds.top);
            }

            const float entityRight{ util::right(entityBounds) };
            const float boundsRight{ util::right(bounds) };
            if (entityRight > boundsRight)
            {
                posDelta.x = (boundsRight - entityRight);
            }

            const float entityBottom{ util::bottom(entityBounds) };
            const float boundsBottom{ util::bottom(bounds) };
            if (entityBottom > boundsBottom)
            {
                posDelta.y = (boundsBottom - entityBottom);
            }

            return posDelta;
        }

        // Returns posDelta AND new absolute velocity
        BounceResult
            updateDeltaBounce(const sf::FloatRect & entityBounds, const sf::Vector2f & vel) const
        {
            BounceResult result{ updateDelta(entityBounds), vel };

            if ((result.pos_delta.x > 0.0f) || (result.pos_delta.x < 0.0f))
            {
                result.velocity.x *= -1.0f;
            }

            if ((result.pos_delta.y > 0.0f) || (result.pos_delta.y < 0.0f))
            {
                result.velocity.y *= -1.0f;
            }

            return result;
        }

        sf::FloatRect bounds;
    };

    //

    class FencedMover : public Mover
    {
      public:
        FencedMover() = default;

        FencedMover(
            const sf::FloatRect & fence,
            const sf::Vector2f & vel,
            const sf::Vector2f & acc = { 0.0f, 0.0f },
            const float speedLimit = { 0.0f })
            : Mover(vel, acc, speedLimit)
            , m_fence(fence)
        {
            setup(vel, acc, speedLimit);
        }

        virtual ~FencedMover() = default;

        sf::Vector2f updateDelta(const sf::FloatRect & bounds, const float frameTimeSec)
        {
            sf::Vector2f posDelta(Mover::updateDelta(frameTimeSec));

            const BounceResult bounceResult(m_fence.updateDeltaBounce(bounds, m_velocity.vector));

            m_velocity.vector = bounceResult.velocity;

            return (posDelta - bounceResult.pos_delta);
        }

        sf::Vector2f updateAbsolute(
            const sf::FloatRect & bounds, const float frameTimeSec, const sf::Vector2f & pos)
        {
            return (pos + updateDelta(bounds, frameTimeSec));
        }

      private:
        sf::Vector2f updateDelta(const float) override { return { 0.0f, 0.0f }; }

        sf::Vector2f updateAbsolute(const float, const sf::Vector2f &) override
        {
            return { 0.0f, 0.0f };
        }

      private:
        Fence m_fence;
    };

} // namespace entity

#endif // SIMPLE_EFFECTS_MOVEMENT_HPP_INCLUDED
