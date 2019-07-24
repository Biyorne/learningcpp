#ifndef EXAMPLES_VECTOR_CAPACITY_HPP_INCLUDED
#define EXAMPLES_VECTOR_CAPACITY_HPP_INCLUDED
//
// vector-capacity.hpp
//
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>

namespace vector_capacity
{

    inline void Ex1()
    {
        //
        std::vector<int> v;

        std::size_t reallocCount(0);
        for (std::size_t i(0); i < 1000; ++i)
        {
            const auto capacityBefore(v.capacity());
            v.push_back(123);
            const auto capacityAfter(v.capacity());

            if (capacityBefore != capacityAfter)
            {
                ++reallocCount;

                std::cout << "\t" << std::setw(3) << reallocCount << "\tsize: " << std::setw(3)
                          << std::right << (v.size() - 1) << " -> " << std::setw(3) << std::left
                          << v.size() << ", capacity: " << std::setw(5) << std::right
                          << capacityBefore << " -> " << std::setw(5) << std::left << capacityAfter
                          << "  +" << (capacityAfter - capacityBefore) << std::endl;
            }
        }

        std::cout << "final_size=" << v.size() << ", realloc_count=" << reallocCount
                  << ", waste_count=" << (v.capacity() - v.size()) << std::endl;
    }

    //

    inline void runAll()
    {
        Ex1();
        std::cout << std::endl << std::endl;
    }

} // namespace vector_capacity

#endif // EXAMPLES_VECTOR_CAPACITY_HPP_INCLUDED
