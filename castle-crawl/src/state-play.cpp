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
#include "popup-manager.hpp"
#include "resources.hpp"
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
        m_windowOutline.setSize(context.layout.windowSize() - sf::Vector2f{ 2.0f, 2.0f });
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

    void StatePlay::handleEvent(Context & context, const sf::Event & event)
    {
        // all that remain are keystrokes
        if (sf::Event::KeyPressed != event.type)
        {
            return;
        }

        if ((sf::Keyboard::Escape == event.key.code) || (sf::Keyboard::Q == event.key.code))
        {
            std::cout << "Player pressed 'Q' or 'Escape'.  Quitting." << std::endl;
            context.state.setChangePending(State::Quit);
            return;
        }

        if (sf::Keyboard::P == event.key.code)
        {
            context.popup.setup(context, "All your bases are belong to us.");
            context.state.setChangePending(State::Popup);
            return;
        }

        context.board.player.handleEvent(context, event);
    }

    void StatePlay::draw(
        const Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.clear(context.config.background_color);
        context.map().draw(context, target, states);
        target.draw(context.board, states);
        target.draw(m_windowOutline, states);
        target.draw(context.anim, states);
        target.draw(m_fps, states);
    }

    //
    //
    //

    // StatePopup::StatePopup(Context & context)
    //    : StatePlay(context)
    //    , m_text(context.popup_message, context.media.font(), 40)
    //    , m_paperTexture()
    //    , m_paperSprite()
    //    , m_backgroundFade()
    //{
    //    m_text.setFillColor(sf::Color::Black);
    //
    //    m_backgroundFade.setSize(context.config.windowSize());
    //    m_backgroundFade.setFillColor(sf::Color(0, 0, 0, 150));
    //
    //    m_paperTexture.loadFromFile(
    //        (context.config.media_dir_path / "image" / "paper-1.png").string());
    //
    //    m_paperSprite.setTexture(m_paperTexture);
    //    util::centerInside(m_paperSprite, context.config.windowRect());
    //
    //    util::centerInside(m_text, context.config.windowRect());
    //}
    //
    // bool StatePopup::handleEvent(Context & context, const sf::Event & event)
    //{
    //    if ((sf::Event::KeyPressed == event.type) || (sf::Event::MouseButtonReleased ==
    //    event.type))
    //    {
    //        context.state.setChangePending(State::Play);
    //        return true;
    //    }
    //    else
    //    {
    //        return false;
    //    }
    //}
    //
    // void StatePopup::draw(
    //    const Context & context, sf::RenderTarget & target, const sf::RenderStates & states) const
    //{
    //    StatePlay::draw(context, target, states);
    //
    //    target.draw(m_backgroundFade, states);
    //    target.draw(m_paperSprite, states);
    //    target.draw(m_text, states);
    //}

} // namespace castlecrawl
