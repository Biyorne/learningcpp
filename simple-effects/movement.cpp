// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// movement.cpp
//
#include "movement.hpp"

namespace entity
{
    Mover::Mover(const sf::Vector2f & vel, const sf::Vector2f & acc, const float speedLimit)
    {
        setup(vel, acc, speedLimit);
    }

    void Mover::setup(const sf::Vector2f & vel, const sf::Vector2f & acc, const float speedLimit)
    {
        m_velocity.vector = vel;
        m_acceleration.vector = acc;
        m_speedLimit = speedLimit;
    }

    void Mover::aim(const sf::Vector2f & from, const sf::Vector2f & to)
    {
        m_acceleration.vector = util::vectorDirectionOnlySetFromTo(m_acceleration.vector, from, to);
    }

    sf::Vector2f Mover::updateDelta(const float frameTimeSec)
    {
        m_velocity.vector += m_acceleration.updateDelta(frameTimeSec);

        if (m_speedLimit > 0.0f)
        {
            m_velocity.vector = util::vectorMagnitudeLimit(m_velocity.vector, m_speedLimit);
        }

        return m_velocity.updateDelta(frameTimeSec);
    }

    sf::Vector2f Mover::updateAbsolute(const float frameTimeSec, const sf::Vector2f & pos)
    {
        return (pos + updateDelta(frameTimeSec));
    }

    //
    //
    //

    Mover MoverFactory::makeFromRanges(
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

    void MoverFactory::getRandomRange(
        const Context & context, float & midpoint, const float base, const float ratioOfBase)
    {
        const float rangeHalf(std::abs((base * ratioOfBase) / 2.0f));
        midpoint = context.random.fromTo((midpoint - rangeHalf), (midpoint + rangeHalf));
    }

    void MoverFactory::getRandomRange(
        const Context & context,
        sf::Vector2f & midpoints,
        const float base,
        const RangeRatios_t & ranges)
    {
        getRandomRange(context, midpoints.x, base, ranges.x);
        getRandomRange(context, midpoints.y, base, ranges.y);
    }

    sf::Vector2f Fence::updateDelta(const sf::FloatRect & entityBounds) const
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

    //
    //
    //

    BounceOpt_t
        Fence::updateDeltaBounce(const sf::FloatRect & entityBounds, const sf::Vector2f & vel) const
    {
        BounceResult result{ updateDelta(entityBounds), vel };

        bool didBounce = false;

        if ((result.pos_delta.x > 0.0f) || (result.pos_delta.x < 0.0f))
        {
            result.velocity.x *= -1.0f;
            didBounce = true;
        }

        if ((result.pos_delta.y > 0.0f) || (result.pos_delta.y < 0.0f))
        {
            result.velocity.y *= -1.0f;
            didBounce = true;
        }

        if (didBounce)
        {
            return result;
        }
        else
        {
            return std::nullopt;
        }
    }

    //
    //
    //

    sf::Vector2f FencedMover::updateDelta(const sf::FloatRect & bounds, const float frameTimeSec)
    {
        const BounceOpt_t resultOpt(m_fence.updateDeltaBounce(bounds, m_velocity.vector));

        if (resultOpt)
        {
            m_velocity.vector = resultOpt.value().velocity;
            return resultOpt.value().pos_delta;
        }
        else
        {
            return sf::Vector2f(Mover::updateDelta(frameTimeSec));
        }
    }

    sf::Vector2f FencedMover::updateAbsolute(
        const sf::FloatRect & bounds, const float frameTimeSec, const sf::Vector2f & pos)
    {
        return (pos + updateDelta(bounds, frameTimeSec));
    }

} // namespace entity
