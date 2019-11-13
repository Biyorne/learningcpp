#ifndef SFUTIL_RANDOM_HPP_INCLUDED
#define SFUTIL_RANDOM_HPP_INCLUDED
//
// random.hpp
//
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <random>
#include <stdexcept>

namespace math
{

    // These functions are written here by hand because:
    //  * <cmath>'s abs() does not use templates
    //  * some of the std functions are not constexpr when they could be
    //  * some of the std functions are not noexcept when they could be
    //  * comparing reals is only needs to be simple/fast/less-accurate for games

    template <typename T>
    [[nodiscard]] T constexpr abs(const T number) noexcept
    {
        static_assert(std::is_arithmetic_v<T>);

        if constexpr (std::is_unsigned_v<T>)
        {
            return number;
        }
        else
        {
            if (number < T(0))
            {
                return -number;
            }
            else
            {
                return number;
            }
        }
    }

    //

    template <typename T>
    [[nodiscard]] constexpr T max(const T left, const T right) noexcept
    {
        static_assert(std::is_arithmetic_v<T>);

        if (left < right)
        {
            return right;
        }
        else
        {
            return left;
        }
    }

    template <typename T, typename... Ts>
    [[nodiscard]] constexpr T max(const T first, const Ts... allOthers) noexcept
    {
        return max(first, max(allOthers...));
    }

    template <typename T>
    [[nodiscard]] constexpr T min(const T left, const T right) noexcept
    {
        static_assert(std::is_arithmetic_v<T>);

        if (left < right)
        {
            return left;
        }
        else
        {
            return right;
        }
    }

    template <typename T, typename... Ts>
    [[nodiscard]] constexpr T min(const T first, const Ts... allOthers) noexcept
    {
        return min(first, min(allOthers...));
    }

    //

    // this lib is for simple/innaccurate/game/etc apps, so a simple multiple of epsilon works
    // fine
    template <typename T>
    constexpr T float_compare_epsilon = (std::numeric_limits<T>::epsilon() * T(10));

    //

    template <typename T>
    [[nodiscard]] constexpr bool isRealClose(const T left, const T right) noexcept
    {
        static_assert(std::is_arithmetic_v<T>);

        if constexpr (std::is_integral_v<T>)
        {
            return (left == right);
        }
        else
        {
            const T diffAbs { abs(right - left) };

            if (diffAbs < T(1))
            {
                return !(diffAbs > float_compare_epsilon<T>);
            }
            else
            {
                return !(diffAbs > (max(abs(left), abs(right), T(1)) * float_compare_epsilon<T>));
            }
        }
    }

    template <typename T>
    [[nodiscard]] constexpr bool isRealCloseOrLess(const T number, const T comparedTo) noexcept
    {
        return ((number < comparedTo) || isRealClose(number, comparedTo));
    }

    template <typename T>
    [[nodiscard]] constexpr bool isRealCloseOrGreater(const T number, const T comparedTo) noexcept
    {
        return ((number > comparedTo) || isRealClose(number, comparedTo));
    }

    //

    template <typename T>
    [[nodiscard]] constexpr bool isRealZero(const T number)
    {
        return isRealClose(number, T(0));
    }

    template <typename T>
    [[nodiscard]] constexpr bool isRealZeroOrLess(const T number) noexcept
    {
        return ((number < T(0)) || isRealClose(number, T(0)));
    }

    template <typename T>
    [[nodiscard]] constexpr bool isRealZeroOrGreater(const T number) noexcept
    {
        return ((number > T(0)) || isRealClose(number, T(0)));
    }

    //

    template <typename T>
    [[nodiscard]] constexpr bool isRealOne(const T number)
    {
        return isRealClose(number, T(1));
    }

    template <typename T>
    [[nodiscard]] constexpr bool isRealOneOrLess(const T number) noexcept
    {
        return ((number < T(1)) || isRealClose(number, T(1)));
    }

    template <typename T>
    [[nodiscard]] constexpr bool isRealOneOrGreater(const T number) noexcept
    {
        return ((number > T(1)) || isRealClose(number, T(1)));
    }

    template <typename T, typename U = T>
    [[nodiscard]] constexpr U
        map(const T number, const T inMin, const T inMax, const U outMin, const U outMax) noexcept
    {
        if (isRealClose(inMin, inMax))
        {
            return outMax;
        }

        return (outMin + static_cast<U>(((number - inMin) * (outMax - outMin)) / (inMax - inMin)));
    }

    // assumes ratio is [0,1]
    template <typename Ratio_t, typename Number_t>
    [[nodiscard]] constexpr Number_t
        mapRatioTo(const Ratio_t ratio, const Number_t outMin, const Number_t outMax) noexcept
    {
        static_assert(std::is_arithmetic_v<Number_t>);
        static_assert(std::is_floating_point_v<Ratio_t>);

        return (
            outMin
            + static_cast<Number_t>(
                ratio * (static_cast<Ratio_t>(outMax) - static_cast<Ratio_t>(outMin))));
    }

    template <typename Number_t, typename Ratio_t = float>
    [[nodiscard]] constexpr Ratio_t
        mapToRatio(const Number_t number, const Number_t inMin, const Number_t inMax) noexcept
    {
        static_assert(std::is_floating_point_v<Ratio_t>);

        if (isRealClose(inMin, inMax))
        {
            return Ratio_t(1);
        }

        return static_cast<Ratio_t>((number - inMin) / (inMax - inMin));
    }

} // namespace math

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

        if (math::isRealClose(from, to))
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

#endif // SFUTIL_RANDOM_HPP_INCLUDED
