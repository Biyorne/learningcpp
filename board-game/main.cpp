// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "game-coordinator.hpp"
#include "util.hpp"

#include <cstddef>

int main(const int argc, const char * const argv[])
{
    // setup misc game details
    boardgame::SnakeGameSettings settings;
    settings.game_name = "Snake";
    settings.sf_window_style = sf::Style::Default;
    settings.video_mode.width = 2880;
    settings.video_mode.height = 1800;

    // find the media directory
    if (argc > 1)
    {
        settings.media_path = argv[1];
    }
    else
    {
        settings.media_path = (std::filesystem::current_path() / "media");
    }

    if (!std::filesystem::exists(settings.media_path) ||
        !std::filesystem::is_directory(settings.media_path))
    {
        std::cout << "Error:  media directory path provided was either empty, not a directory, or "
                     "was simply not there: \""
                  << settings.media_path << "\"." << std::endl;

        return EXIT_FAILURE;
    }

    //
    boardgame::GameCoordinator game(settings);
    game.run();

    return EXIT_SUCCESS;
}