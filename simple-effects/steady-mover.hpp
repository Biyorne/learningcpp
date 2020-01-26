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

    void update(const float elapsedTimeSec)
    {
        velocity += acceleration;
        velocity *= (elapsedTimeSec * speed);

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

// Encapsulates constant speed that changes a position.
struct SteadyMover
{
    SteadyMover(const float speed, const sf::Vector2f & vel)
        : speed(speed)
        , velocity(vel)
    {}

    sf::Vector2f movePerFrame(const sf::Vector2f & pos, const float elapsedTimeSec) const
    {
        // Normalize the velocity
        const sf::Vector2f velNormal(util::vecNormal(velocity));

        // Scale the vector to match the speed, then multiply by the elapsed time.
        const sf::Vector2f velAtSpeed(velNormal * speed * elapsedTimeSec);

        return (pos + velAtSpeed);
    }

    float speed;
    sf::Vector2f velocity;
};

#endif // SIMPLE_EFFECTS_STEADY_MOVER_HPP_INCLUDED
