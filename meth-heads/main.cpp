// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "simulator.hpp"

#include <cstddef>

// TODO Nel:  Allow command line parameters to set the mode
int main()
{
    using namespace methhead;

    Simulator simulator(Mode::Normal);
    simulator.run();

    return EXIT_SUCCESS;
}