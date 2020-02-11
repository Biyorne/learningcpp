#ifndef SIMPLE_EFFECTS_STEADY_MOVER_HPP_INCLUDED
#define SIMPLE_EFFECTS_STEADY_MOVER_HPP_INCLUDED

#include "util.hpp"

#include <SFML/Graphics.hpp>

struct Mover
{
    explicit Mover(
        const float spd = 1.0f,
        const sf::Vector2f & pos = sf::Vector2f(0.0f, 0.0f),
        const sf::Vector2f & vel = sf::Vector2f(0.0f, 0.0f),
        const sf::Vector2f & acc = sf::Vector2f(0.0f, 0.0f),
        const float spdLimit = 0.0f)
        : speed(spd)
        , position(pos)
        , velocity(vel)
        , acceleration(acc)
        , speed_limit(spdLimit)
    {}

    void update(const float frameTimeSec)
    {
        velocity += acceleration;
        velocity *= (frameTimeSec * speed);

        if (velocity.x > speed_limit)
        {
            velocity.x = speed_limit;
        }

        if (velocity.y > speed_limit)
        {
            velocity.y = speed_limit;
        }

        position += velocity;
    }

    float speed;
    sf::Vector2f position;     // the result we wanted in the first place
    sf::Vector2f velocity;     // change to position
    sf::Vector2f acceleration; // changes to velocity
    float speed_limit;
};

//

namespace entity
{

    // Encapsulates constant speed that changes a position.
    struct Velocity
    {
        explicit Velocity(const sf::Vector2f & vel)
            : vector(vel)
        {}

        sf::Vector2f updateDelta(const float frameTimeSec) const { return (vector * frameTimeSec); }

        sf::Vector2f updateAbsolute(const float frameTimeSec, const sf::Vector2f & pos) const
        {
            return (pos + updateDelta(frameTimeSec));
        }

        sf::Vector2f vector;
    };

    //

    struct Acceleration
    {
        explicit Acceleration(const sf::Vector2f & acc)
            : vector(acc)
        {}

        sf::Vector2f updateDelta(const float frameTimeSec) const { return (vector * frameTimeSec); }

        sf::Vector2f updateAbsolute(const float frameTimeSec, const sf::Vector2f & vel) const
        {
            return (vel + updateDelta(frameTimeSec));
        }

        sf::Vector2f vector;
    };

    //

    // Fencing hard stop:
    // sf::Floatrect fence
    // sf::Floatrect objectSize
    // Change position
    //
    // Fencing Bounce:
    // sf::Vector2f velocity
    // Change position and flip velocity

    //

    struct BounceResult
    {
        sf::Vector2f pos_delta{ 0.0f, 0.0f };
        sf::Vector2f velocity{ 0.0f, 0.0f };
    };

    //

    struct Fence
    {
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

    // struct SpeedLimit
    // {
    //
    //     float speed_limit;
    // };

} // namespace entity

#endif // SIMPLE_EFFECTS_STEADY_MOVER_HPP_INCLUDED
