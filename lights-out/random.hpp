#ifndef LIGHTS_OUT_RANDOM_HPP_INCLUDED
#define LIGHTS_OUT_RANDOM_HPP_INCLUDED

#include <limits>
#include <random>

namespace lightsout
{
    class Random
    {

    public:
        Random()
            : m_engine()
        {

            std::random_device randomDevice;
            std::seed_seq seedSequence { randomDevice() };
            m_engine.seed(seedSequence);

            const unsigned long long WARMUP_SKIP(10000);
            m_engine.discard(WARMUP_SKIP);
        }

        template <typename T>
        T rollInteger(const T RANGE_MIN, const T RANGE_MAX) const
        {
            if (RANGE_MAX < RANGE_MIN)
            {
                return rollInteger(RANGE_MAX, RANGE_MIN);
            }

            if (RANGE_MAX == RANGE_MIN)
            {
                return RANGE_MAX;
            }

            std::uniform_int_distribution<T> TDistribution(RANGE_MIN, RANGE_MAX);
            return TDistribution(m_engine);
        }

        template <typename T>
        T rollInteger(const T RANGE_MAX) const
        {
            return rollInteger(static_cast<T>(0), RANGE_MAX);
        }

        template <typename T>
        T rollReal(const T RANGE_MIN, const T RANGE_MAX) const
        {
            if (RANGE_MAX < RANGE_MIN)
            {
                return rollReal(RANGE_MAX, RANGE_MIN);
            }

            if (isRealClose(RANGE_MAX, RANGE_MIN))
            {
                return RANGE_MAX;
            }

            std::uniform_real_distribution<T> TDistribution(
                RANGE_MIN, std::nextafter(RANGE_MAX, std::numeric_limits<T>::max()));

            return TDistribution(m_engine);
        }

        template <typename T>
        T rollReal(const T RANGE_MAX) const
        {
            return rollReal(static_cast<T>(0), RANGE_MAX);
        }

        bool rollBool() const { return (rollInteger(0, 1) == 1); }

        template <typename T>
        T & select(std::vector<T> & container) const
        {
            return container.at(rollInteger(static_cast<std::size_t>(0), (container.size() - 1)));
        }

        template <typename T>
        const T & select(const std::vector<T> & CONTAINER) const
        {
            return CONTAINER.at(rollInteger(static_cast<std::size_t>(0), (CONTAINER.size() - 1)));
        }

        template <typename T>
        void shuffle(T & container) const
        {
            std::shuffle(std::begin(container), std::end(container), m_engine);
        }

    private:
        template <typename T>
        bool isRealClose(const T A, const T B) const
        {
            const T STRIDE_SIZE(
                ((B < 1.0) ? std::numeric_limits<T>::epsilon()
                           : (B * std::numeric_limits<T>::epsilon())));

            return (std::abs(B - A) < STRIDE_SIZE);
        }

    private:
        mutable std::mt19937 m_engine;
    };
} // namespace lightsout
#endif // LIGHTS_OUT_RANDOM_HPP_INCLUDED
