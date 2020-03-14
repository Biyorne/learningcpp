#ifndef SIMPLE_EFFECTS_MOVEMENT_HPP_INCLUDED
#define SIMPLE_EFFECTS_MOVEMENT_HPP_INCLUDED

#include "util.hpp"

#include <SFML/Graphics.hpp>

namespace entity
{
    struct MovementVector
    {
        explicit MovementVector(const sf::Vector2f & vel)
            : vector(vel)
        {}

        sf::Vector2f updateDelta(const float frameTimeSec) const { return (vector * frameTimeSec); }

        sf::Vector2f updateAbsolute(const float frameTimeSec, const sf::Vector2f & pos) const
        {
            return (pos + updateDelta(frameTimeSec));
        }

        sf::Vector2f vector;
    };

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

} // namespace entity

#endif // SIMPLE_EFFECTS_MOVEMENT_HPP_INCLUDED
