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
    class Map;
    class Media;
    class Layout;
    class GameInPlay;
    struct Board;
    struct GameConfig;

    //

    struct Context
    {
        Context(
            GameInPlay & gam,
            Map & mp,
            Board & bor,
            const GameConfig & con,
            const Layout & lay,
            const Media & med,
            const util::Random & ran,
            util::SoundPlayer & aud,
            util::AnimationPlayer & ani)
            : game(gam)
            , map(mp)
            , board(bor)
            , config(con)
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
        Map & map;
        Board & board;
        const GameConfig & config;
        const Layout & layout;
        const Media & media;
        const util::Random & random;
        util::SoundPlayer & audio;
        util::AnimationPlayer & anim;
    };
} // namespace castlecrawl

#endif // CASTLECRAWL_CONTEXT_HPP_INCLUDED
