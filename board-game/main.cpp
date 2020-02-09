// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "game-coordinator.hpp"
#include "util.hpp"

#include <cstddef>

int main(const int argc, const char * const argv[])
{
    std::filesystem::path mediaPath;
    if (argc > 1)
    {
        mediaPath = argv[1];
    }
    else
    {
        mediaPath = (std::filesystem::current_path() / "media");
    }

    if (!std::filesystem::exists(mediaPath) || !std::filesystem::is_directory(mediaPath))
    {
        std::cout << "Error:  media directory path provided was either empty, not a directory, or "
                     "was simply not there: \""
                  << mediaPath << "\"." << std::endl;

        return EXIT_FAILURE;
    }

    boardgame::GameCoordinator game(mediaPath);
    game.run();

    return EXIT_SUCCESS;
}