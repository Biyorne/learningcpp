// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// main.cpp
//  Examples and practice problems
//
#include "nested-for.hpp"
#include "vector-capacity.hpp"

#include <cstdlib>
#include <limits>

template <typename T>
void printNumberRollover()
{
    std::cout << std::numeric_limits<T>::lowest() << ", " << std::numeric_limits<T>::max()
              << std::endl;

    T r = ((std::numeric_limits<T>::max() - 10) * (std::numeric_limits<T>::max() - 10));

    std::cout << r << std::endl;
}

int main(void)
{

    printNumberRollover<short>();
    printNumberRollover<int>();
    // nested_for::runAll();
    // vector_capacity::runAll();
    return EXIT_SUCCESS;
}
