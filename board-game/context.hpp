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
    struct IMap;
    struct IBoard;
    struct IResources;

    //

    struct Context
    {
        Context(
            const IResources & res,
            const IMap & mp,
            IBoard & bor,
            util::Random & ran,
            util::SoundPlayer & aud,
            util::AnimationPlayer & ani)
            : resources(res)
            , map(mp)
            , board(bor)
            , random(ran)
            , audio(aud)
            , anim(ani)
            , is_game_over(false)
            , is_testing_enabled(true)
        {}

        const IResources & resources;
        const IMap & map;
        IBoard & board;
        util::Random & random;
        util::SoundPlayer & audio;
        util::AnimationPlayer & anim;
        bool is_game_over;
        bool is_testing_enabled;
    };
} // namespace boardgame

#endif // BOARDGAME_SIM_CONTEXT_HPP_INCLUDED