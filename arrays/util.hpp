#ifndef ARRAYS_UTIL_HPP_INCLUDED
#define ARRAYS_UTIL_HPP_INCLUDED
#include "test.hpp"
#include <iostream>

namespace util
{

    template <typename T>
    T sum(const T ARRAY[], const std::size_t SIZE)
    {
        return test::sum<T>(ARRAY, SIZE);
    }

    template <typename T>
    void print(const T ARRAY[], const std::size_t SIZE)
    {
        std::cout << "[";
        for (std::size_t i(0); i < SIZE; ++i)
        {
            std::cout << ARRAY[i];
            if (i < (SIZE - 1))
            {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }

    template <typename T>
    void fillCounting(T myArray[], const std::size_t SIZE)
    {
        for (std::size_t i(0); i < SIZE; ++i)
        {
            myArray[i] = i;
        }
    }

    void runTests()
    {
        // fillCounting

        const std::size_t ARRAY_SIZE(4);
        int arrayCounting[ARRAY_SIZE];
        int arrayExpected[ARRAY_SIZE] = { 0, 1, 2, 3 };
        fillCounting(arrayCounting, ARRAY_SIZE);
        test::printResult(test::isEqual(arrayCounting, ARRAY_SIZE, arrayExpected, ARRAY_SIZE));
    }

} // namespace util
#endif // ARRAYS_UTIL_HPP_INCLUDED
