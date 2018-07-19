#ifndef ARRAYS_TEST_HPP_INCLUDED
#define ARRAYS_TEST_HPP_INCLUDED

#include <iostream>

namespace test
{

    template <typename T>
    void isSum(
        const std::string & NAME, const T ARRAY[], const std::size_t SIZE, const T SUM_EXPECTED)
    {
        const T SUM_ACTUAL(sumArray(ARRAY, SIZE));
        std::cout << NAME << " sum = " << SUM_ACTUAL;

        if (SUM_ACTUAL == SUM_EXPECTED)
        {
            std::cout << " PASS" << std::endl;
        }
        else
        {
            std::cout << " FAIL ("
                      << "expected " << SUM_EXPECTED << ")" << std::endl;
        }
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
} // namespace test

#endif // ARRAYS_TEST_HPP_INCLUDED
