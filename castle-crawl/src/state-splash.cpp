// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// splash-coordinator.cpp
//
#include "state-splash.hpp"

#include "check-macros.hpp"
#include "settings.hpp"
#include "state-machine.hpp"
#include "util.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>

#include <SFML/Graphics.hpp>

namespace castlecrawl
{
    StateSplash::StateSplash()
        : StateBase(State::Splash)
        , m_texture()
        , m_sprite()
    {}

    void StateSplash::onEnter(Context & context)
    {
        const bool loadSuccess = m_texture.loadFromFile(
            (context.config.media_dir_path / "image" / "splash.png").string());

        M_CHECK_SS(loadSuccess, "failed to load splash.png");

        m_sprite.setTexture(m_texture);

        util::fit(m_sprite, (context.layout.windowSize() * 0.5f));
        util::centerInside(m_sprite, sf::FloatRect({ 0.0f, 0.0f }, context.layout.windowSize()));
    }

    void StateSplash::handleEvent(Context & context, const sf::Event & event)
    {
        if ((sf::Event::KeyPressed == event.type) || (sf::Event::MouseButtonPressed == event.type))
        {
            context.state.setChangePending(State::Play);
        }
    }

    void StateSplash::draw(
        const Context &, sf::RenderTarget & target, const sf::RenderStates & states) const
    {
        target.draw(m_sprite);
    }

} // namespace castlecrawl
