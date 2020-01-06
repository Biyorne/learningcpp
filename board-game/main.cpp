// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "board-game.hpp"
#include "util.hpp"

#include <cstddef>

int main(const int argc, const char * const argv[])
{
    boardgame::BoardGame game(util::mediaPathFind(argc, argv));
    game.run();

    return EXIT_SUCCESS;
}