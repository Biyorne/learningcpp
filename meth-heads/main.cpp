// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "simulator.hpp"

#include <cstddef>

using namespace methhead;

int main()
{
    //{
    //    Simulator simulator(Mode::Normal);
    //    simulator.run();
    //}

    {
        Simulator simulator(Mode::SpeedTest);
        simulator.run();
    }

    return EXIT_SUCCESS;
}