#ifndef MARIO_RANDOM_HPP_INCLUDED
#define MARIO_RANDOM_HPP_INCLUDED
//
// random.hpp
//
#include <initializer_list>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>

namespace mario
{
    // Remember:  It's 10-100x faster to re-generate random numbers than to shuffle containers.

    class Random
    {
      public:
        Random()
            : Random(std::random_device{}())
        {}

        explicit Random(const std::random_device::result_type seed)
            : m_engine()
        {
            std::seed_seq seedSequence{ seed };
            m_engine.seed(seedSequence);

            // Warm-up-skipping is good standard practice when working with PRNGs, but the Mersenne
            // Twister is notoriously predictable in the beginning.  This is especially true when
            // you don't provide a good (full sized) seed, which I am not because I want the ease of
            // troubleshooting games. Anything from thousands to hundreds-thousands works fine here.
            m_engine.discard(123456);
        }

        template <typename T>
        T fromTo(const T from, const T to) const
        {
            if (to < from)
            {
                return fromTo(to, from);
            }

            if constexpr (std::is_floating_point_v<T>)
            {
                if (isRealClose(from, to))
                {
                    return from;
                }

                std::uniform_real_distribution<T> distribution(
                    from, std::nextafter(to, std::numeric_limits<T>::max()));

                return distribution(m_engine);
            }
            else
            {
                static_assert(std::is_integral_v<T>);
                std::uniform_int_distribution<T> distribution(from, to);
                return distribution(m_engine);
            }
        }

        template <typename T>
        T zeroTo(const T to) const
        {
            return fromTo(T(0), to);
        }

        template <typename T = float>
        T ratio() const
        {
            return zeroTo(T(1));
        }

        template <typename T>
        std::size_t index(const T size) const
        {
            if (size <= 1)
            {
                return 0;
            }

            return static_cast<std::size_t>(zeroTo(size - T(1)));
        }

        template <typename T>
        std::size_t indexFrom(const T & container) const
        {
            if (container.size() == 0)
            {
                std::string message;
                message += "Random::index(container<";
                message += nameof::nameof_full_type<T>();
                message += ">) but that container.size() == 0!";

                throw std::runtime_error(message);
            }

            return index(container.size());
        }

        inline bool boolean() const { return (zeroTo(1) == 0); }

        template <typename Iter_t>
        auto & from(Iter_t first, const Iter_t last) const
        {
            if (last == first)
            {
                std::string message;
                message += "Random::from(<";
                message += nameof::nameof_full_type<Iter_t>();
                message += ">) but the container was empty!";

                throw std::runtime_error(message);
            }

            const auto offset{ zeroTo(std::distance(first, last) - 1) };
            std::advance(first, static_cast<std::ptrdiff_t>(offset));
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

        template <typename Iter_t>
        void shuffle(const Iter_t first, const Iter_t last) const
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
} // namespace mario

#endif // MARIO_RANDOM_HPP_INCLUDED