#ifndef WORKING_WITH_CONTAINERS_HPP_INCLUDED
#define WORKING_WITH_CONTAINERS_HPP_INCLUDED

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <vector>

namespace school
{

    class OverFifty
    {
    public:
        bool operator()(const int NUMBER)
        {
            auto foundIter = m_cacheMap.find(NUMBER);

            if (foundIter == std::end(m_cacheMap))
            {
                const bool RESULT(NUMBER > 50);
                m_cacheMap.insert(std::make_pair(NUMBER, RESULT));
                return RESULT;
            }
            else
            {
                return foundIter->second;
            }
        }

    private:
        std::map<int, bool> m_cacheMap;
    };

    template <typename T>
    void print(const T & CONTAINER)
    {
        for (const auto & ELEMENT : CONTAINER)
        {
            std::cout << ELEMENT << ",";
        }
        std::cout << std::endl;
    }

    void runContainerTests()
    {
        // Remember:  A class can implement the "funtion call operator" operator(), and become
        // callable, and become a "functor".
        // You want to do this when a function needs to remember or store something.

        // To change the size of a container (insert/erase), you HAVE to call a function on that
        // container. Because insert/erase is the beating heart of what a container is.

        // how to do you add things to containers?  (we say INSERT)
        std::vector<int> numbers = { 0, 1, 2, 3 };
        std::vector<int> sameNumbers;
        sameNumbers.insert(std::begin(sameNumbers), std::begin(numbers), std::end(numbers));
        print(sameNumbers);

        std::list<int> listOfNumbers;
        listOfNumbers.insert(std::begin(listOfNumbers), std::begin(numbers), std::end(numbers));
        print(listOfNumbers);

        // how do you (not remove) erase elements from a container?
        sameNumbers.erase(std::begin(sameNumbers) + (sameNumbers.size() - 1));
        print(sameNumbers);

        // remove everything
        // sameNumbers.clear(); // makes the size zero
        sameNumbers.erase(std::begin(sameNumbers), std::end(sameNumbers));
        print(sameNumbers);

        // How to just resize?
        sameNumbers.resize(99, 5);
        print(sameNumbers);

        // What if you want to erase only CERTAIN elements?
        // auto iterToStartErasing(std::remove(std::begin(sameNumbers), std::end(sameNumbers), 5));
        // sameNumbers.erase(iterToStartErasing, std::end(sameNumbers));

        // erase-remove idiom (with a lambda)
        sameNumbers.erase(
            std::remove_if(
                std::begin(sameNumbers),
                std::end(sameNumbers),
                [](const auto NUMBER) { return (NUMBER == 5); }),
            std::end(sameNumbers));

        std::cout << "---" << std::endl;
        print(sameNumbers);
        std::cout << "---" << std::endl;

        for (int index(0); index < 100; ++index)
        {
            sameNumbers.push_back(index);
        }
        print(sameNumbers);

        // erase-remove_if call that removes all even numbers
        // if ((NUMBER % 2) == 0) then is even
        sameNumbers.erase(
            std::remove_if(
                std::begin(sameNumbers),
                std::end(sameNumbers),
                [](const auto NUMBER) { return ((NUMBER % 2) == 0); }),
            std::end(sameNumbers));
        print(sameNumbers);

        // sort
        // std::sort(std::rbegin(sameNumbers), std::rend(sameNumbers));
        std::sort(std::begin(sameNumbers), std::end(sameNumbers), [](const auto A, const auto B) {
            return (A >= B);
        });
        print(sameNumbers);

        // IF you are not using a std library algorithm, use the range for:
        for (auto & number : sameNumbers)
        {
            number *= 2;
        }
    }

} // namespace school

#endif // WORKING_WITH_CONTAINERS_HPP_INCLUDED
