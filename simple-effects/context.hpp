#ifndef SIMPLE_EFFECTS_CONTEXT_HPP_INCLUDED
#define SIMPLE_EFFECTS_CONTEXT_HPP_INCLUDED

#include "random.hpp"
#include "sound-player.hpp"
#include "util.hpp"

#include <SFML/Graphics.hpp>

struct Context
{
    Context(const sf::RenderWindow & window, util::Random & rdm, util::SoundPlayer & aud)
        : random(rdm)
        , audio(aud)
        , window_size(window.getSize())
        , window_rect({}, window_size)
        , mouse_pos(sf::Mouse::getPosition(window))
        , sprite_size_ratio_default(1.0f / 5.0f)
    {}

    util::Random & random;
    util::SoundPlayer & audio;
    sf::Vector2f window_size;
    sf::FloatRect window_rect;
    sf::Vector2f mouse_pos;
    float sprite_size_ratio_default;
};

#endif // SIMPLE_EFFECTS_CONTEXT_HPP_INCLUDED
