#ifndef SFUTIL_RANDOM_HPP_INCLUDED
#define SFUTIL_RANDOM_HPP_INCLUDED
//
// random.hpp
//
#include "util.hpp"

#include <algorithm>
#include <cstdlib>
#include <limits>
#include <random>
#include <stdexcept>

namespace util
{
    //
    class Random
    {
    public:
        Random()
            : m_warmupSkipCount(123456)
            , m_seed(std::random_device()())
            , m_engine()
        {
            std::seed_seq seedSequence { m_seed };
            m_engine.seed(seedSequence);
            m_engine.discard(m_warmupSkipCount);
        }

        std::random_device::result_type seed() const noexcept { return m_seed; }

        unsigned long long warmupSkipCount() const noexcept { return m_warmupSkipCount; }

        template <typename T>
        T fromTo(const T from, const T to) const
        {
            static_assert(std::is_arithmetic_v<T>);
            static_assert(!std::is_same_v<std::remove_cv<T>, bool>);

            if (to < from)
            {
                return fromTo(to, from);
            }

            if (isRealClose(from, to))
            {
                return from;
            }

            if constexpr (std::is_integral_v<T>)
            {
                // std::uniform_int_distribution does not support single-byte types (chars)
                if constexpr (1 == sizeof(T))
                {
                    return static_cast<T>(fromTo(static_cast<int>(from), static_cast<int>(to)));
                }
                else
                {
                    std::uniform_int_distribution<T> distribution(from, to);
                    return distribution(m_engine);
                }
            }
            else
            {
                std::uniform_real_distribution<T> distribution(
                    from, std::nextafter(to, std::numeric_limits<T>::max()));

                return distribution(m_engine);
            }
        }

        template <typename T>
        T zeroTo(const T to) const
        {
            return fromTo(T(0), to);
        }

        template <typename T>
        T zeroToOneLessThan(const T to) const
        {
            return zeroTo(to - 1);
        }

        inline bool boolean() const { return (zeroTo(1) == 1); }

        template <typename T>
        T ofType() const
        {
            if constexpr (std::is_signed_v<T>)
            {
                if (boolean())
                {
                    return fromTo(std::numeric_limits<T>::lowest(), T(0));
                }
                else
                {
                    return fromTo(T(0), std::numeric_limits<T>::max());
                }
            }
            else
            {
                return fromTo(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::max());
            }
        }

        // returns [0, size-1]
        template <typename T>
        std::size_t index(const T & container) const
        {
            if (0 == container.size())
            {
                throw std::runtime_error("Random::index() but container.size()==0.");
            }

            return zeroToOneLessThan(container.size());
        }

        template <typename Iterator_t>
        auto & from(Iterator_t begin, const Iterator_t end) const
        {
            if (end == begin)
            {
                throw std::runtime_error("Random::from(begin, end) but begin==end.");
            }

            return fromIterAndSize(begin, std::distance(begin, end));
        }

        template <typename T>
        auto & from(T & container) const
        {
            if (0 == container.size())
            {
                throw std::runtime_error("Random::from(container) but container.size()==0.");
            }

            return fromIterAndSize(std::begin(container), container.size());
        }

        template <typename T>
        const T & from(const std::initializer_list<T> initList) const
        {
            return fromIterAndSize(std::begin(initList), initList.size());
        }

        template <typename Iterator_t>
        void shuffle(const Iterator_t begin, const Iterator_t end) const
        {
            std::shuffle(begin, end, m_engine);
        }

        template <typename T>
        void shuffle(T & container) const
        {
            shuffle(std::begin(container), std::end(container));
        }

    private:
        template <typename Iterator_t>
        auto & fromIterAndSize(Iterator_t iter, const std::size_t size) const
        {
            if (0 == size)
            {
                throw std::runtime_error("Random::fromIterAndSize(iter, size) but size==0.");
            }

            std::advance(iter, zeroToOneLessThan(size));
            return *iter;
        }

    private:
        // the first few numbers generated by mt19937 are easily predictable if the seed is bad, so
        // skip them warmup is also just a good practice when working with PRNGs anything from
        // thousands to millions will work fine
        unsigned long long m_warmupSkipCount;

        // this is kept so that the seed can be printed or logged for replay testing
        std::random_device::result_type m_seed;

        mutable std::mt19937 m_engine;
    };

} // namespace util

#endif // SFUTIL_RANDOM_HPP_INCLUDED
