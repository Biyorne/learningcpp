// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "game-coordinator.hpp"

#include <cstddef>

//

int main(const int argc, const char * const argv[])
{
    using namespace boardgame;

    // find the media directory
    std::filesystem::path mediaDirPath{ std::filesystem::current_path() / "media" };
    if (argc > 1)
    {
        mediaDirPath = argv[1];
    }

    //
    const GameSettingsBase settings("Snaker", { 40, 30 }, mediaDirPath);

    //
    try
    {
        boardgame::GameCoordinator game(settings);
        game.run();
    }
    catch (const std::exception & ex)
    {
        std::cout << "EXCEPTION ERROR:  \"" << ex.what() << "\"" << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cout << "EXCEPTION ERROR:  \"UNKOWNN\"" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}