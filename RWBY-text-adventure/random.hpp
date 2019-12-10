#ifndef RWBY_TEXT_ADVENTURE_RANDOM_HPP_INCLUDED
#define RWBY_TEXT_ADVENTURE_RANDOM_HPP_INCLUDED

#include <limits>
#include <random>
#include <stdexcept>

namespace RWBYadventure
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
        std::size_t indexInto(const T & CONTAINER) const
        {
            if (CONTAINER.empty())
            {
                return 0;
            }
            else
            {
                return rollInteger(static_cast<std::size_t>(0), (CONTAINER.size() - 1));
            }
        }

        template <typename T>
        std::size_t indexInto(T & container) const
        {
            if (container.empty())
            {
                return 0;
            }
            else
            {
                return rollInteger(static_cast<std::size_t>(0), (container.size() - 1));
            }
        }

        template <typename T>
        const auto & select(const T & CONTAINER) const
        {
            if (CONTAINER.empty())
            {
                throw std::runtime_error("Random::select(const) but the container was empty!");
            }
            else
            {
                auto iter(std::begin(CONTAINER));
                std::advance(iter, indexInto(CONTAINER));
                return *iter;
            }
        }

        template <typename T>
        auto & select(T & container) const
        {
            if (container.empty())
            {
                throw std::runtime_error("Random::select(non-const) but the container was empty!");
            }
            else
            {
                auto iter(std::begin(container));
                std::advance(iter, indexInto(container));
                return *iter;
            }
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

} // namespace RWBYadventure

#endif // RWBY_TEXT_ADVENTURE_RANDOM_HPP_INCLUDED
