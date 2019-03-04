#ifndef RANDOM_HPP_INCLUDED
#define RANDOM_HPP_INCLUDED

#include <random>

class RandomRoll
{

public:
    RandomRoll()
        : m_engine()
    {
        std::random_device randomDevice;
        std::seed_seq seedSequence { randomDevice() };
        m_engine.seed(seedSequence);
    }

    int rollInt(const int RANGE_MIN, const int RANGE_MAX) const
    {
        if (RANGE_MAX < RANGE_MIN)
        {
            return rollInt(RANGE_MAX, RANGE_MIN);
        }

        if (RANGE_MAX == RANGE_MIN)
        {
            return RANGE_MAX;
        }

        std::uniform_int_distribution<int> intDistribution(RANGE_MIN, RANGE_MAX);
        return intDistribution(m_engine);
    }

    double rollDouble(const double RANGE_MIN, const double RANGE_MAX) const
    {
        if (RANGE_MAX < RANGE_MIN)
        {
            return rollDouble(RANGE_MAX, RANGE_MIN);
        }

        // Todo: fix for floats
        // if (RANGE_MAX == RANGE_MIN)
        //{
        //    return RANGE_MAX;
        //}

        std::uniform_real_distribution<double> doubleDistribution;
        return doubleDistribution(m_engine);
    }

private:
    mutable std::mt19937 m_engine;
};

#endif // RANDOM_HPP_INCLUDED
