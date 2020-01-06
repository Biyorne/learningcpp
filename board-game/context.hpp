#ifndef BOARDGAME_SIM_CONTEXT_HPP_INCLUDED
#define BOARDGAME_SIM_CONTEXT_HPP_INCLUDED
//
// context.hpp
//
#include <memory>
#include <string>
#include <vector>

namespace util
{
    class Random;
    class SoundPlayer;
    class AnimationPlayer;
} // namespace util

namespace boardgame
{
    struct PieceBase;
    using PiceUPtr_t = std::unique_ptr<PieceBase>;

    struct Resources;
    struct DisplayConstants;

    //

    struct Context
    {
        Context(
            const std::string & mediaPathParam,
            const DisplayConstants & displayParam,
            const Resources & resourcesParam,
            util::Random & randonParam,
            util::SoundPlayer & audioParam,
            util::AnimationPlayer & animParam,
            std::vector<PiceUPtr_t> & piecesParam)
            : media_path(mediaPathParam)
            , display(displayParam)
            , resources(resourcesParam)
            , random(randonParam)
            , audio(audioParam)
            , anim(animParam)
            , pieces(piecesParam)
            , player_pos_change(0, 0)
        {}

        const std::string & media_path;
        const DisplayConstants & display;
        const Resources & resources;

        util::Random & random;
        util::SoundPlayer & audio;
        util::AnimationPlayer & anim;

        std::vector<PiceUPtr_t> & pieces;

        // non-zero means move in that direction next turn
        BoardPos_t player_pos_change;
    };
} // namespace boardgame

#endif // BOARDGAME_SIM_CONTEXT_HPP_INCLUDED