#ifndef MARIO_MOVER_HPP_INCLUDED
#define MARIO_MOVER_HPP_INCLUDED
//
// mover.hpp
//
#include "util.hpp"

#include <SFML/Graphics.hpp>

namespace mario
{
    struct Mover
    {
        Mover()
            : Mover({ 0.0f, 0.0f }, { -1.0f, -1.0f })
        {}

        // negative speedMax prevents any movement in that direction
        Mover(const sf::Vector2f & vel, const sf::Vector2f max)
            : velocity(vel)
            , velocity_max(max)
            , acceleration(0.0f, 0.5f) // gravity always pulling down
        {}

        void updateVelocity(const bool willFall)
        {
            if (willFall)
            {
                velocity += acceleration;
            }

            limit(velocity, velocity_max);
        }

        static void limit(sf::Vector2f & vel, const sf::Vector2f & speedMax)
        {
            limit(vel.x, speedMax.x);
            limit(vel.y, speedMax.y);
        }

        static void limit(float & vel, const float max)
        {
            if (max < 0.0f)
            {
                vel = 0.0f;
                return;
            }

            if (max > 0.0f)
            {
                if (std::abs(vel) < max)
                {
                    return;
                }

                if (vel < 0.0f)
                {
                    vel = -max;
                }
                else
                {
                    vel = max;
                }
            }
        }

        sf::Vector2f velocity;
        sf::Vector2f velocity_max;
        sf::Vector2f acceleration;
    };
} // namespace mario

#endif // MARIO_MOVER_HPP_INCLUDED