#ifndef STD_CONTAINER_EXAMPLES_HPP_INCLUDED
#define STD_CONTAINER_EXAMPLES_HPP_INCLUDED

#include "test.hpp"

#include <algorithm>
#include <list>
#include <set>
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
    // O(n)
    bool isValueDuplicated(const int VALUE, const std::vector<int> & V)
    {
        std::size_t howManyFound(0);
        for (std::size_t u(0); u < V.size(); ++u)
        {
            if (VALUE == V[u])
            {
                ++howManyFound;
                if (howManyFound > 1)
                {
                    return true;
                }
            }
        }

        const bool IS_DUPLICATED(howManyFound > 1);
        return IS_DUPLICATED;
    }

    // O(n^2)
    bool isEveryValueDuplicated(const std::vector<int> & V)
    {
        std::size_t duplicateCounter(0);
        for (std::size_t i(0); i < V.size(); ++i)
        {
            const int VALUE(V[i]);
            const bool IS_DUPLICATED(isValueDuplicated(VALUE, V));

            if (IS_DUPLICATED)
            {
                ++duplicateCounter;
            }
        }

        return (V.size() == duplicateCounter);
    }

    void runTests()
    {
        test::isTrue(isMultipleOf(20, 10));
        test::isFalse(isMultipleOf(10, 20));
        test::isTrue(isMultipleOf(0, 8765, true));
        test::isFalse(isMultipleOf(0, 315, false));
        test::isFalse(isMultipleOf(0, 0));
        runStdVectorCopyingTests();

        std::cout << "-" << std::endl;
        // isEveryValueDuplicated(V(n)) = O(n^2)
        test::isFalse(isEveryValueDuplicated({ 0, 1, 2, 3 }));
        test::isTrue(isEveryValueDuplicated({ 1, 1, 2, 2 }));

        /*{
    std::vector<int> v;
    v.reserve(100);
    // std::cout << v.capacity() << std::endl;
    for (std::size_t i(0); i < 100; ++i)
    {
        const std::size_t CAPACITY_ORIG(v.capacity());
        v.push_back(0);
        // std::cout << v.capacity() - CAPACITY_ORIG << std::endl;
    }
}*/

        //	Big O notation: O(#)
        //	The O stands for order
        //	The # stands for the number of operations that will be performed in the worst case.
        //	Linear naive searching of arrays of size n is O(n)
        //	O(1) means that the worst case takes just one operation.
        //	If the operations don't depend on the size of something, then it's considered O(1).
        //

        // std::array
        //	Elements are in an array on the stack
        //	- Size of the container can't change
        //	- You have to pick the size ahead of time and you can never pick the right size.
        //	- Pass and return from functions is a hassle because you have to pick the size.
        //	+ It is the fastest container there is.
        //  + Implements the indice operator[] to access elements
        //	- Search operation. O(n) "Linear"
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
        //	+ Implements the indice operator[] to access elements
        //	- Search operation. O(n) "Linear"
        //	- Insert/Remove O(n)
        //	+ Push/Pop back amortized is O(1)
        //	+-

        // std::list
        // Chain of pointers to nodes scattered in memory
        // Single or double linked
        // - Search O(n)
        // + Insert/Remove O(1)
        // Use when inserting and removing needs to be as fast as possible.

        // std::set
        // Elements in a balanced binary tree.
        // + Search O(log2(n))
        // + Insert/Remove O(log2(n))
        // + Always sorted

        // std::multiset
        // O(same as std::set)

        // std::string
        //

        // std::map<Key_t, Value_t>
        //  (same as set, only direction nodes are a different type than the value nodes)
        /*
                std::map<std::string, std::string> dict;
                dict.insert("one", "lowest cardinal");
                ..
                const std::string KEY("one");
                std::cout << "The definition of " << KEY << " is " << dict.find(KEY);
        */
        // std::multimap

        // Array, List, Balanced Binary Tree
        // myInventoryMap.Find("Bob").DropItem("Torch");

        // Three Fundamental Container Memory Layouts

        // Array
        //	Why a big three?  100 % efficient
        // std::array
        // std::vector
        // Search O(n)
        // Insert/Remove O(n)

        // List
        //	Why a big three?  Insert/Remove is Constant Time
        // std::list
        // Search O(n)
        // Insert/Remove O(1)

        // BBT
        //	Logrithmic operations, Always sorted
        // std::set
        // std::multiset
        // std::map
        // std::multimap
        // Search O(log(n))
        // Insert/Remove O(log(n))

        // STL Containers
        //.size(), .empty(), .insert(),

        // STL Iterators
        // begin/end/advance/++/--/

        std::vector<int> myVec = { 5, 4, 3, 2, 1 };
        // util::print(myVec.data(), myVec.size());
        std::sort(myVec.begin(), myVec.end(), [](const int L, const int R) { return (R < L); });
        util::print(myVec.data(), myVec.size());

        /*std::list<int> myList = { 6, 7, 9, 8, 10 };
        myList.sort();
        for (std::list<int>::const_reverse_iterator i(myList.crbegin()); i != myList.crend(); ++i)
        {
            std::cout << *i;
        }
        std::cout << std::endl;

        std::list<int> myList2 = { 6, 7, 9, 8, 10 };

                const int MAX(7);

        std::copy_if(myList.begin(), myList.end(), std::back_inserter(myVec), [&MAX](const int
        VALUE) { return (VALUE <= MAX);
        });
                */
        std::sort(myVec.rbegin(), myVec.rend());
        util::print(myVec.data(), myVec.size());

        /*std::set<int> mySet = { 0, 3, 6, 2, 7 };
        for (std::set<int>::iterator i(mySet.begin()); i != mySet.end(); ++i)
        {
            std::cout << *i;
        }
        std::cout << std::endl;*/
    }

    /*
// O(1) - "Constant Time"
// O(9999999) - "Constant Time"
void Foo1(const std::vector<int> & V)
{
    for (int i(0); i < 9999999; ++i)
    {
        std::cout << "The vector has " << V.size() << " ints." << std::endl;
    }
}

// O(n)	- "Linear Time"
// O(3n) - "Linear Time"
void Foo2(const std::vector<int> & V)
{
    for (int i(0); i < V.size(); ++i)
    {
        std::cout << "Value=" << V[i] << "." << std::endl;
        std::cout << "Value=" << V[i] << "." << std::endl;
        std::cout << "Value=" << V[i] << "." << std::endl;
    }
}

// O(n * log(n)) - "some strange name"
void Foo3(const std::vector<int> & V)
{
    std::set<int> mySet;
    for (int i(0); i < V.size(); ++i)
    {
        mySet.insert(V[i]);
    }
}

// O(n^3) - "Exponential Time"
void Foo4(const std::vector<int> & V)
{
    for (int i(0); i < V.size(); ++i)
    {
        for (int u(0); u < V.size(); ++u)
        {
            for (int x(0); x < V.size(); ++x)
            {
                std::cout << "Value=" << V[i] << "." << std::endl;
            }
        }
    }
}
    */
} // namespace examples

#endif // STD_CONTAINER_EXAMPLES_HPP_INCLUDED
