// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-play.cpp
//
#include "state-play.hpp"

#include "animation-player.hpp"
#include "board.hpp"
#include "context.hpp"
#include "map.hpp"
#include "settings.hpp"
#include "state-machine.hpp"

#include <iostream>

#include <SFML/Graphics.hpp>

namespace castlecrawl
{

    StatePlay::StatePlay(Context & context)
        : StateBase(State::Play)
        , m_fps()
        , m_windowOutline()
    {
        m_windowOutline.setPosition(1.0f, 1.0f);
        m_windowOutline.setSize(context.config.windowSize() - sf::Vector2f{ 2.0f, 2.0f });
        m_windowOutline.setFillColor(sf::Color::Transparent);
        m_windowOutline.setOutlineThickness(1.0f);
        m_windowOutline.setOutlineColor(sf::Color(80, 80, 80));

        m_fps.reset(context);
    }

    void StatePlay::update(Context & context, const float frameTimeSec)
    {
        context.anim.update(frameTimeSec);
        m_fps.update();
    }

    bool StatePlay::handleEvent(Context & context, const sf::Event & event)
    {
        // all that remain are keystrokes
        if (sf::Event::KeyPressed != event.type)
        {
            return false;
        }

        if ((sf::Keyboard::Escape == event.key.code) || (sf::Keyboard::Q == event.key.code))
        {
            std::cout << "Player pressed 'Q' or 'Escape'.  Quitting." << std::endl;
            context.state.setChangePending(State::Quit);
            return true;
        }

        context.board.player.handleEvent(context, event);

        return context.state.isChangePending();
    }

    void StatePlay::draw(
        const Context & context, sf::RenderTarget & target, const sf::RenderStates & states) const
    {
        target.clear(context.config.background_color);
        context.map().draw(context, target, states);
        target.draw(context.board);
        target.draw(m_windowOutline);
        target.draw(context.anim);
        target.draw(m_fps);
    }

} // namespace castlecrawl
