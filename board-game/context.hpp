#ifndef BOARDGAME_SIM_CONTEXT_HPP_INCLUDED
#define BOARDGAME_SIM_CONTEXT_HPP_INCLUDED
//
// context.hpp
//
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
    struct IBoard;
    struct IMedia;
    struct IMedia;
    struct ILayout;
    struct GameConfig;
    struct IGameInPlay;

    using Map_t = std::vector<std::string>;

    //

    struct Context
    {
        Context(
            IGameInPlay & gam,
            const Map_t & mp,
            const GameConfig & conf,
            const ILayout & lay,
            const IMedia & med,
            IBoard & bor,
            const util::Random & ran,
            util::SoundPlayer & aud,
            util::AnimationPlayer & ani)
            : game(gam)
            , map(mp)
            , config(conf)
            , layout(lay)
            , media(med)
            , board(bor)
            , random(ran)
            , audio(aud)
            , anim(ani)
        {}

        Context(const Context &) = delete;
        Context(Context &&) = delete;

        Context & operator=(const Context &) = delete;
        Context & operator=(Context &&) = delete;

        IGameInPlay & game;
        const Map_t & map;
        const GameConfig & config;
        const ILayout & layout;
        const IMedia & media;
        IBoard & board;
        const util::Random & random;
        util::SoundPlayer & audio;
        util::AnimationPlayer & anim;
    };
} // namespace boardgame

#endif // BOARDGAME_SIM_CONTEXT_HPP_INCLUDED