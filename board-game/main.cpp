// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// main.cpp
//
#include "game-coordinator.hpp"

#include <cstddef>

//
// map needs to be it's own feature packed class
// WindowRegions
// IEntity
// States
//

//
// Rules:
//  * NEVER allow multiple pieces on one BoardPos_t at a time.
//      This would be worse than a crash: Occasionally strange behavior. <cue scary music>
//      Solution:
//          - Use the Board class EVERY TIME you ADD or MOVE a piece, and then you don't hae to
//          worry.
//            (This is literally the sole purpose of the Board class: To avoid breaking this rule.)
//          - However, when you or some non-player Piece Simply call Board::removePiece() before
//          setting ANY/ALL Piece positions by hand.
//            That function is meant to be overused, and will NOT error if theres nothing's there.
//          - All Board's functions already do this auto remove() for you, so don't worry there.
//          - WORRY when:
//              * you don't use Board's add/move functions
//              * you pick the BoardPos_t yourself, a, or set Piece::m_position
//          yourself.
//
//  * you can erase pieces from the Board while traversing, BUT NOT YOURSELF
//  * The Piece enum MUST have a Count at the end
//
//

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
        config.is_fullscreen = false;
        config.video_mode.width = 2880;
        config.video_mode.height = 1800;
        config.between_cells_pad_ratio = 0.9f;
        config.frame_rate_limit = 0;

        //
        Map_t map;

        //
        TestingFrenzyGame game;
        game.reset(config, map);
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