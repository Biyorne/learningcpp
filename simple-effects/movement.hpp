#ifndef SIMPLE_EFFECTS_MOVEMENT_HPP_INCLUDED
#define SIMPLE_EFFECTS_MOVEMENT_HPP_INCLUDED

#include "context.hpp"
#include "util.hpp"

#include <optional>

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
        float speed_limit{ 0.0f };
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
        float speed_limit{ 0.0f };
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
            const float speedLimit = 0.0f);

        virtual ~Mover() = default;

        virtual void setup(
            const sf::Vector2f & vel,
            const sf::Vector2f & acc = { 0.0f, 0.0f },
            const float speedLimit = 0.0f);

        virtual void aim(const sf::Vector2f & from, const sf::Vector2f & to);

        virtual sf::Vector2f updateDelta(const float frameTimeSec);

        virtual sf::Vector2f updateAbsolute(const float frameTimeSec, const sf::Vector2f & pos);

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
            const float spdLimit = 0.0f)
        {
            const MoverRatios ratios{ velocity, acceleration, spdLimit };
            return makeFromRatios(base, ratios);
        }

        static Mover makeFromRanges(
            const Context & context,
            const float base,
            const MoverRatios & ratios,
            const MoverRanges & ranges);

      private:
        static void getRandomRange(
            const Context & context, float & midpoint, const float base, const float ratioOfBase);

        static void getRandomRange(
            const Context & context,
            sf::Vector2f & midpoints,
            const float base,
            const RangeRatios_t & ranges);
    };

    //

    struct BounceResult
    {
        sf::Vector2f pos_delta{ 0.0f, 0.0f };
        sf::Vector2f velocity{ 0.0f, 0.0f };
    };

    using BounceOpt_t = std::optional<BounceResult>;

    struct Fence
    {
        Fence() = default;

        // TODO- Ask if constructor with one member needs to be in CPP
        explicit Fence(const sf::FloatRect & bnd)
            : bounds(bnd)
        {}

        // Returns change to entityBounds position
        sf::Vector2f updateDelta(const sf::FloatRect & entityBounds) const;

        // Returns posDelta AND new absolute velocity
        BounceOpt_t
            updateDeltaBounce(const sf::FloatRect & entityBounds, const sf::Vector2f & vel) const;

        sf::FloatRect bounds;
    };

    class FencedMover : public Mover
    {
      public:
        FencedMover() = default;

        FencedMover(
            const sf::FloatRect & fence,
            const sf::Vector2f & vel,
            const sf::Vector2f & acc = { 0.0f, 0.0f },
            const float speedLimit = 0.0f)
            : Mover(vel, acc, speedLimit)
            , m_fence(fence)
        {
            setup(vel, acc, speedLimit);
        }

        virtual ~FencedMover() = default;

        sf::Vector2f updateDelta(const sf::FloatRect & bounds, const float frameTimeSec);

        sf::Vector2f updateAbsolute(
            const sf::FloatRect & bounds, const float frameTimeSec, const sf::Vector2f & pos);

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
