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
    struct ImageHandler;

    //

    struct Context
    {
        Context(
            const ImageHandler & imagesParam,
            Board & boardParam,
            util::Random & randonParam,
            util::SoundPlayer & audioParam,
            util::AnimationPlayer & animParam)
            : images(imagesParam)
            , board(boardParam)
            , random(randonParam)
            , audio(audioParam)
            , anim(animParam)
        {}

        const ImageHandler & images;
        Board & board;
        util::Random & random;
        util::SoundPlayer & audio;
        util::AnimationPlayer & anim;
    };
} // namespace boardgame

#endif // BOARDGAME_SIM_CONTEXT_HPP_INCLUDED