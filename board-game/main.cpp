// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// main.cpp
//
#include "game-coordinator.hpp"

#include <cstddef>

int main(const int argc, const char * const argv[])
{
    using namespace boardgame;

    try
    {
        //
        GameConfig config;
        if (argc > 1)
        {
            config.media_dir_path = std::filesystem::path{ argv[1] };
        }

        // clang-format off
        MapChars_t mapChars = {  
            "```|pC----------T````````````````",
            "```| |bbb       |````````````````",
            "```| |bb        B-T----T`````````",
            "```| |b           |  bb|`````````",
            "```| |            |   b|`````````",
            "```| B--T         |    |`````````",
            "```|    |         |    |`````````",
            "```B--T C------D--B    |`````````",
            "``````| |              |`````````",
            "``````| |              |`````````",
            "C-----BDB              B--------B",
            "|                               D",
            "|              C----------T-----B",
            "|              |         b|``````",
            "|              D          |``````",
            "|              |         b|``````",
            "B----T----B B--C----------B``````",
            "`````|   b     |`````````````````",
            "`````|bb      b|`````````````````",
            "`````|bbbb  bbb|`````````````````",
            "`````B---------B`````````````````" };
        // clang-format on

        //
        GameCoordinator game;
        game.reset(config, mapChars);
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
