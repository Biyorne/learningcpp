// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-popup.cpp
//
#include "state-popup.hpp"

#include "context.hpp"
#include "popup-manager.hpp"
#include "state-machine.hpp"

#include <SFML/Graphics.hpp>

namespace castlecrawl
{

    StatePopup::StatePopup(Context & context)
        : StatePlay(context)
    {}

    void StatePopup::handleEvent(Context & context, const sf::Event & event)
    {
        if (sf::Event::KeyPressed == event.type)
        {
            context.popup.key = event.key.code;
            context.state.setChangePending(State::Play);
            return;
        }
    }

    void StatePopup::draw(
        const Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        StatePlay::draw(context, target, states);
        target.draw(context.popup);
    }

} // namespace castlecrawl
