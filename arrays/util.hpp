#ifndef ARRAYS_UTIL_HPP_INCLUDED
#define ARRAYS_UTIL_HPP_INCLUDED
#include <iostream>

namespace util
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

} // namespace util
#endif // ARRAYS_UTIL_HPP_INCLUDED
