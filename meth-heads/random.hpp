#ifndef METHHEADS_RANDOM_HPP_INCLUDED
#define METHHEADS_RANDOM_HPP_INCLUDED

#include "utils.hpp"

#include <initializer_list>
#include <limits>
#include <random>
#include <stdexcept>

namespace methhead
{
    class Random
    {
      public:
        Random()
            : m_engine()
        {
            std::random_device randomDevice;
            std::seed_seq seedSequence{ randomDevice() };
            m_engine.seed(seedSequence);

            // anything from thousands to hundred-thousands will work here
            const unsigned long long WARMUP_SKIP(12345);
            m_engine.discard(WARMUP_SKIP);
        }

        template <typename T>
        T fromTo(const T from, const T to) const
        {
            if (to < from)
            {
                return fromTo(to, from);
            }

            if (isRealClose(from, to))
            {
                return from;
            }

            if constexpr (std::is_floating_point_v<T>)
            {
                std::uniform_real_distribution<T> distribution(
                    from, std::nextafter(to, std::numeric_limits<T>::max()));

                return distribution(m_engine);
            }
            else
            {
                static_assert(std::is_integral_v<T> && !std::is_same_v<std::remove_cv_t<T>, bool>);

                std::uniform_int_distribution<T> distribution(from, to);
                return distribution(m_engine);
            }
        }

        template <typename T>
        T zeroTo(const T to) const
        {
            return fromTo(T(0), to);
        }

        inline bool boolean() const { return (zeroTo(1) == 1); }

        template <typename T>
        std::size_t index(const T & container) const
        {
            if (container.empty())
            {
                throw std::runtime_error("Random::index() but the container was empty!");
            }

            return zeroTo(container.size() - 1);
        }

        template <typename Iterator_t>
        auto & from(Iterator_t first, const Iterator_t last) const
        {
            if (last == first)
            {
                throw std::runtime_error("Random::from() but the container was empty!");
            }

            std::advance(first, zeroTo(std::distance(first, last) - 1));
            return *first;
        }

        template <typename T>
        auto & from(T & container) const
        {
            return from(std::begin(container), std::end(container));
        }

        template <typename T>
        const T & from(const std::initializer_list<T> & list) const
        {
            return from(std::begin(list), std::end(list));
        }

        template <typename Iterator_t>
        void shuffle(const Iterator_t first, const Iterator_t last) const
        {
            std::shuffle(first, last, m_engine);
        }

        template <typename T>
        void shuffle(T & container) const
        {
            shuffle(std::begin(container), std::end(container));
        }

      private:
        mutable std::mt19937 m_engine;
    };
} // namespace methhead

#endif // METHHEADS_RANDOM_HPP_INCLUDED