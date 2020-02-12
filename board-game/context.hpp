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

    //

    struct Context
    {
        Context(
            const std::filesystem::path & mediaPath,
            const sf::RenderWindow & window,
            const IResources & res,
            IBoard & bor,
            util::Random & ran,
            util::SoundPlayer & aud,
            util::AnimationPlayer & ani)
            : media_path(mediaPath)
            , window_size(window.getSize())
            , window_bounds({ 0.0f, 0.0f }, window_size)
            , resources(res)
            , board(bor)
            , random(ran)
            , audio(aud)
            , anim(ani)
            , is_self_testing(false)
        {}

        const std::filesystem::path media_path;
        const sf::Vector2f window_size;
        const sf::FloatRect window_bounds;
        const IResources & resources;
        IBoard & board;
        util::Random & random;
        util::SoundPlayer & audio;
        util::AnimationPlayer & anim;
        bool is_self_testing;
    };
} // namespace boardgame

#endif // BOARDGAME_SIM_CONTEXT_HPP_INCLUDED