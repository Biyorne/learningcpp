// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "simulator.hpp"

#include <cstddef>
#include <string>

int main()
{
    using namespace methhead;

    bool willReRunInTestMode{ false };
    {
        Simulator simulator(Mode::Normal);
        willReRunInTestMode = simulator.run();
    }

    if (willReRunInTestMode)
    {
        Simulator simulator(Mode::Test);
        simulator.run();
    }

    return EXIT_SUCCESS;
}