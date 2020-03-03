#ifndef BOARDGAME_SIM_CONTEXT_HPP_INCLUDED
#define BOARDGAME_SIM_CONTEXT_HPP_INCLUDED
//
// context.hpp
//
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

namespace util
{
    class Random;
    class SoundPlayer;
    class AnimationPlayer;
} // namespace util

namespace boardgame
{
    struct IBoard;
    struct IResources;
    struct IGameSettings;

    //

    struct Context
    {
        Context(
            IGameSettings & set,
            const IResources & res,
            IBoard & bor,
            util::Random & ran,
            util::SoundPlayer & aud,
            util::AnimationPlayer & ani)
            : settings(set)
            , resources(res)
            , board(bor)
            , random(ran)
            , audio(aud)
            , anim(ani)
        {}

        IGameSettings & settings;
        const IResources & resources;
        IBoard & board;
        const util::Random & random;
        util::SoundPlayer & audio;
        util::AnimationPlayer & anim;
    };
} // namespace boardgame

#endif // BOARDGAME_SIM_CONTEXT_HPP_INCLUDED