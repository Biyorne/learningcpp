#ifndef CASTLECRAWL_CONTEXT_HPP_INCLUDED
#define CASTLECRAWL_CONTEXT_HPP_INCLUDED
//
// context.hpp
//

namespace util
{
    class Random;
    class SoundPlayer;
    class AnimationPlayer;
} // namespace util

namespace castlecrawl
{
    class Media;
    class Layout;
    struct GameConfig;
    class GameInPlay;
    class Map;

    //

    struct Context
    {
        Context(
            GameInPlay & gam,
            const Map & mp,
            const GameConfig & conf,
            const Layout & lay,
            const Media & med,
            const util::Random & ran,
            util::SoundPlayer & aud,
            util::AnimationPlayer & ani)
            : game(gam)
            , map(mp)
            , config(conf)
            , layout(lay)
            , media(med)
            , random(ran)
            , audio(aud)
            , anim(ani)
        {}

        Context(const Context &) = delete;
        Context(Context &&) = delete;

        Context & operator=(const Context &) = delete;
        Context & operator=(Context &&) = delete;

        GameInPlay & game;
        const Map & map;
        const GameConfig & config;
        const Layout & layout;
        const Media & media;
        const util::Random & random;
        util::SoundPlayer & audio;
        util::AnimationPlayer & anim;
    };
} // namespace castlecrawl

#endif // CASTLECRAWL_CONTEXT_HPP_INCLUDED
