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
    struct Board;
    struct Resources;

    //

    struct Context
    {
        Context(
            const Resources & resourcesParam,
            Board & boardParam,
            util::Random & randonParam,
            util::SoundPlayer & audioParam,
            util::AnimationPlayer & animParam)
            : resources(resourcesParam)
            , board(boardParam)
            , random(randonParam)
            , audio(audioParam)
            , anim(animParam)
        {}

        const Resources & resources;
        Board & board;
        util::Random & random;
        util::SoundPlayer & audio;
        util::AnimationPlayer & anim;
    };
} // namespace boardgame

#endif // BOARDGAME_SIM_CONTEXT_HPP_INCLUDED