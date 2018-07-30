#ifndef ARRAYS_TEST_HPP_INCLUDED
#define ARRAYS_TEST_HPP_INCLUDED

#include <iostream>

namespace test
{
    template <typename T>
    T sum(const T ARRAY[], const std::size_t SIZE)
    {
        T sum(0);

        for (std::size_t i(0); i < SIZE; ++i)
        {
            sum += ARRAY[i];
        }

        return sum;
    }

    template <typename T>
    bool isSum(const T ARRAY[], const std::size_t SIZE, const T SUM_EXPECTED)
    {
        return (sum(ARRAY, SIZE) == SUM_EXPECTED);
    }

    inline void printResult(const bool B)
    {
        if (B)
        {
            std::cout << "PASS" << std::endl;
        }
        else
        {
            std::cout << "FAIL" << std::endl;
        }
    }

    template <typename T>
    bool isEqual(const T ARRAY1, const std::size_t SIZE1, const T ARRAY2, const std::size_t SIZE2)
    {
        if (SIZE1 != SIZE2)
        {
            return false;
        }

        for (std::size_t i(0); i < SIZE1; ++i)
        {
            if (ARRAY1[i] != ARRAY2[i])
            {
                return false;
            }
        }

        return true;
    }

    void isTrue(const bool CONDITION) { std::cout << ((CONDITION) ? "PASS" : "FAIL") << std::endl; }

    void isFalse(const bool CONDITION) { return isTrue(!CONDITION); }

    void runTests()
    {
        const std::size_t ARRAY_SIZE(4);
        const int ARRAY_COUNTING[ARRAY_SIZE] = { 0, 1, 2, 3 };
        const int ARRAY_COUNTING_COPY[ARRAY_SIZE] = { 0, 1, 2, 3 };
        const int ARRAY_ZERO[ARRAY_SIZE] = { 0, 0, 0, 0 };

        // isEqual() tests
        {
            // two equal arrays
            printResult(isEqual(ARRAY_COUNTING, ARRAY_SIZE, ARRAY_COUNTING_COPY, ARRAY_SIZE));

            // two !equal arrays
            printResult(isEqual(ARRAY_COUNTING, ARRAY_SIZE, ARRAY_ZERO, ARRAY_SIZE) == false);

            // if one array is empty
            // different sized array
            printResult(isEqual(ARRAY_COUNTING, ARRAY_SIZE, ARRAY_COUNTING_COPY, 0) == false);

            // both are empty
            printResult(isEqual(ARRAY_COUNTING, 0, ARRAY_COUNTING_COPY, 0));

            // same array
            printResult(isEqual(ARRAY_COUNTING, ARRAY_SIZE, ARRAY_COUNTING, ARRAY_SIZE));
        }

        // isSum() tests
        {
            // non-empty array
            printResult(isSum(ARRAY_COUNTING, ARRAY_SIZE, 6));

            // sum is zero
            printResult(isSum(ARRAY_ZERO, ARRAY_SIZE, 0));

            // empty array
            printResult(isSum(ARRAY_COUNTING, 0, 0));
        }
    }

} // namespace test

#endif // ARRAYS_TEST_HPP_INCLUDED
