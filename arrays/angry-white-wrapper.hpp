#ifndef ANGRY_WHITE_WRAPPER_HPP_INCLUDED
#define ANGRY_WHITE_WRAPPER_HPP_INCLUDED

#include "test.hpp"
#include "util.hpp"

#include <array>
#include <chrono>
#include <cstdlib>

namespace wrappers
{
    template <typename T>
    class AngryWhiteWrapper
    {
    public:
        // default constructor
        AngryWhiteWrapper() = delete;

        // custom constructor
        explicit AngryWhiteWrapper(const std::size_t SIZE)
            : m_arrayPtr(new T[SIZE])
            , m_size(SIZE)
        {}

        // copy constructor
        explicit AngryWhiteWrapper(const AngryWhiteWrapper & AW)
            : m_arrayPtr(new T[AW.size()])
            , m_size(AW.size())
        {
            for (std::size_t i(0); i < m_size; ++i)
            {
                m_arrayPtr[i] = AW[i];
            }
        }

        // destructor
        ~AngryWhiteWrapper() { delete[] m_arrayPtr; }

        T & operator[](const std::size_t INDEX) { return m_arrayPtr[INDEX]; }

        const T & operator[](const std::size_t INDEX) const { return m_arrayPtr[INDEX]; }

        std::size_t size() const { return m_size; }

        void fill() { util::fillCounting(m_arrayPtr, m_size); }

        T sum() { return util::sum(m_arrayPtr, m_size); }

        friend bool operator==(const AngryWhiteWrapper<T> & L, const AngryWhiteWrapper<T> & R)
        {
            return test::isEqual(L.m_arrayPtr, L.m_size, R.m_arrayPtr, R.m_size);
        }

        friend bool operator!=(const AngryWhiteWrapper<T> & L, const AngryWhiteWrapper<T> & R)
        {
            return !(L == R);
        }

        std::size_t max_size() { return std::size_t(-1); }

    private:
        T * m_arrayPtr;
        std::size_t m_size;
    };

    inline void testAngryWhiteWrapper()
    {
        const std::size_t ARRAY_SIZE(4);
        AngryWhiteWrapper<int> wArray1(ARRAY_SIZE);
        for (std::size_t i(0); i < ARRAY_SIZE; ++i)
        {
            wArray1[i] = static_cast<int>(i + 1); //{ 1, 2, 3, 4 }
        }
        test::printResult(wArray1.size() == ARRAY_SIZE);

        AngryWhiteWrapper<int> wArray2(ARRAY_SIZE);
        for (std::size_t i(0); i < ARRAY_SIZE; ++i)
        {
            wArray2[i] = static_cast<int>((i + 1) * 10); //{ 10, 20, 30, 40}
        }

        // test(wArray1.size() == ARRAY_SIZE);

        AngryWhiteWrapper<int> wArray2Copy(wArray2);
        test::printResult(wArray2 == wArray2Copy);

        // test(wArray1 != wArray2);
        // wArray1 = wArray2;
        // test(wArray1 == wArray2);

        AngryWhiteWrapper<int> wArrayCounting1(ARRAY_SIZE);
        wArrayCounting1.fill();

        AngryWhiteWrapper<int> wArrayCounting2(ARRAY_SIZE);
        wArrayCounting2.fill();

        test::printResult(wArrayCounting1 == wArrayCounting2);
        test::printResult(wArrayCounting1 != wArray1);
    }

    inline void timingTests()
    {
        const std::size_t TEST_LOOP_COUNT(10000);
        const std::size_t TEST_ARRAY_SIZE(100000);

        using Clock_t = std::chrono::steady_clock;

        {
            const std::chrono::time_point<Clock_t> ARRAY_TEST_START(Clock_t::now());
            {
                for (std::size_t i(0); i < TEST_LOOP_COUNT; ++i)
                {
                    int intArray[TEST_ARRAY_SIZE];
                    util::fillCounting(intArray, TEST_ARRAY_SIZE);
                    util::sum(intArray, TEST_ARRAY_SIZE);
                }
            }
            const std::chrono::time_point<Clock_t> ARRAY_TEST_STOP(Clock_t::now());

            const auto ARRAY_TEST_DURATION_MS(std::chrono::duration_cast<std::chrono::milliseconds>(
                                                  ARRAY_TEST_STOP - ARRAY_TEST_START)
                                                  .count());

            std::cout << "Array Test took " << ARRAY_TEST_DURATION_MS << "ms." << std::endl;

            const std::chrono::time_point<Clock_t> ARRAYWRAPPER_TEST_START(Clock_t::now());
            {
                for (std::size_t i(0); i < TEST_LOOP_COUNT; ++i)
                {
                    std::array<int, TEST_ARRAY_SIZE> ar;
                    util::fillCounting(ar.data(), ar.size());
                    util::sum(ar.data(), ar.size());
                }
            }
            const std::chrono::time_point<Clock_t> ARRAYWRAPPER_TEST_STOP(Clock_t::now());

            const auto ARRAYWRAPPER_TEST_DURATION_MS(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    ARRAYWRAPPER_TEST_STOP - ARRAYWRAPPER_TEST_START)
                    .count());

            std::cout << "AngryWhiteWrapper Test took " << ARRAYWRAPPER_TEST_DURATION_MS << "ms."
                      << std::endl;

            std::cout << "AngryWhiteWrapper took "
                      << (ARRAYWRAPPER_TEST_DURATION_MS - ARRAY_TEST_DURATION_MS)
                      << "ms longer which is "
                      << (static_cast<float>(ARRAYWRAPPER_TEST_DURATION_MS)
                          / static_cast<float>(ARRAY_TEST_DURATION_MS))
                      << "x slower." << std::endl;
        }
    }

    void runTests()
    {
        testAngryWhiteWrapper();
        // timingTests();
    }
} // namespace wrappers
#endif // ANGRY_WHITE_WRAPPER_HPP_INCLUDED
