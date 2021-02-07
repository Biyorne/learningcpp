#ifndef CASTLECRAWL_CONTEXT_HPP_INCLUDED
#define CASTLECRAWL_CONTEXT_HPP_INCLUDED
//
// context.hpp
//
#include <string>

namespace util
{
    class Random;
    class SoundPlayer;
    class AnimationPlayer;
} // namespace util

namespace castlecrawl
{
    class Map;
    class Maps;
    class Media;
    class Layout;
    class GameInPlay;
    class StateMachine;

    struct Board;
    struct MapLink;
    struct GameConfig;
    struct PopupManager;
    //

    struct Context
    {
        Context(
            GameInPlay & gam,
            Maps & mps,
            Board & bor,
            const GameConfig & con,
            Layout & lay,
            const Media & med,
            StateMachine & sta,
            PopupManager & pop,
            const util::Random & ran,
            util::SoundPlayer & aud,
            util::AnimationPlayer & ani)
            : game(gam)
            , maps(mps)
            , board(bor)
            , config(con)
            , layout(lay)
            , media(med)
            , state(sta)
            , popup(pop)
            , random(ran)
            , audio(aud)
            , anim(ani)
            , map_name()
        {}

        Context(const Context &) = delete;
        Context(Context &&) = delete;

        Context & operator=(const Context &) = delete;
        Context & operator=(Context &&) = delete;

        // subsystem references
        GameInPlay & game;
        Maps & maps;
        Board & board;
        const GameConfig & config;
        Layout & layout;
        const Media & media;
        StateMachine & state;
        PopupManager & popup;
        const util::Random & random;
        util::SoundPlayer & audio;
        util::AnimationPlayer & anim;

        // map handlers
        Map & map();
        const Map & map() const;
        void switchToMap(const MapLink & link);

        std::string map_name;
    };
} // namespace castlecrawl

#endif // CASTLECRAWL_CONTEXT_HPP_INCLUDED
