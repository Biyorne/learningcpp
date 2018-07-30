#ifndef STD_CONTAINER_EXAMPLES_HPP_INCLUDED
#define STD_CONTAINER_EXAMPLES_HPP_INCLUDED

#include "test.hpp"

#include <vector>

namespace examples
{
    template <typename T>
    bool isMultipleOf(const T NUMBER, const T MULTIPLE, const bool WILL_RETURN_TRUE_IF_ZERO = false)
    {
        if (MULTIPLE == 0)
        {
            std::cout << "MULTIPLE is zero in examples::isMultipleOf()" << std::endl;
            return false;
        }
        else if (NUMBER == 0)
        {
            return WILL_RETURN_TRUE_IF_ZERO;
        }
        else
        {
            return (0 == (NUMBER % MULTIPLE));
        }
    }

    // We still have to pass the size with the container, and that sucks.
    // std::array is the fastest container, but that's because it's an array.
    // That is why we can't change the size.
    // It's annoying that the size of the source array has to be guessed by the programmer.
    // The same is true with the destination array.
    // When you have to pick the size at compile time (or when you write code), you can never pick
    // the right value. If you guess too low, you save memory but your program will be limited to
    // that number. If you guess too high, you will waste memory and because it's on the stack, you
    // will blow the stack and crash the app.
    template <typename T, std::size_t SOURCE_SIZE, std::size_t DEST_SIZE>
    std::size_t copyMultiplesAndReturnCount(
        const std::array<T, SOURCE_SIZE> & SOURCE,
        std::array<T, DEST_SIZE> & dest,
        const T MULTIPLE)
    {
        std::size_t destIndex(0);
        for (std::size_t i(0); ((i < SOURCE_SIZE) && (destIndex < DEST_SIZE)); ++i)
        {
            if (isMultipleOf(SOURCE[i], MULTIPLE))
            {
                dest[destIndex++] = SOURCE[i];
            }
        }

        return destIndex;
    }

    template <typename T>
    std::size_t copyMultiplesAndReturnCount(
        const std::vector<T> & SOURCE, std::vector<T> & dest, const T MULTIPLE)
    {
        for (std::size_t i(0); i < SOURCE.size(); ++i)
        {
            if (isMultipleOf(SOURCE[i], MULTIPLE))
            {
                dest.push_back(SOURCE[i]);
            }
        }

        return dest.size();
    }

    void runStdArrayCopyingTests()
    {
        const std::size_t SIZE(100);
        std::array<int, SIZE> countingArray;
        util::fillCounting(countingArray.data(), countingArray.size(), 1);
        std::array<int, SIZE> multsOfTenArray;
        const std::size_t MULTIPLE_COUNT(
            copyMultiplesAndReturnCount(countingArray, multsOfTenArray, 10));
        copyMultiplesAndReturnCount(countingArray, multsOfTenArray, 7);
        util::print(multsOfTenArray.data(), multsOfTenArray.size());
    }

    void runStdVectorCopyingTests()
    {
        const std::size_t SIZE(100);
        std::vector<int> countingVector;
        countingVector.resize(SIZE);
        util::fillCounting(countingVector.data(), countingVector.size(), 1);
        std::vector<int> multsVector;
        const std::size_t MULTIPLE_COUNT(
            copyMultiplesAndReturnCount(countingVector, multsVector, 10));
        util::print(multsVector.data(), multsVector.size());
    }

    void runTests()
    {
        test::isTrue(isMultipleOf(20, 10));
        test::isFalse(isMultipleOf(10, 20));
        test::isTrue(isMultipleOf(0, 8765, true));
        test::isFalse(isMultipleOf(0, 315, false));
        test::isFalse(isMultipleOf(0, 0));

        runStdVectorCopyingTests();
        {
            std::vector<int> v;
            v.reserve(100);
            std::cout << v.capacity() << std::endl;
            for (std::size_t i(0); i < 100; ++i)
            {
                const std::size_t CAPACITY_ORIG(v.capacity());
                v.push_back(0);
                std::cout << v.capacity() - CAPACITY_ORIG << std::endl;
            }
        }

        // std::array
        //	Elements are in an array on the stack
        //	- Size of the container can't change
        //	- You have to pick the size ahead of time and you can never pick the right size.
        //	- Pass and return from functions is a hassle because you have to pick the size.
        //	+ It is the fastest container there is.
        // When to use std::array
        //	If you know the size ahead of time
        //	If you don't change the size
        //	If it isn't often passed to functions

        // std::vector
        //	Elements are in an array on the heap
        //	- Slower than a std::array, but everything is.
        //	+ Second fastest container
        //	+ Resizable
        //	+ Smart automatic resizing
        //  + control the actual and under-the-hood size
    }
} // namespace examples

#endif // STD_CONTAINER_EXAMPLES_HPP_INCLUDED
