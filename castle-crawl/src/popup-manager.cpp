// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// popup-manager.cpp
//
#include "popup-manager.hpp"

#include "resources.hpp"
#include "settings.hpp"
#include "util.hpp"

#include <SFML/Graphics.hpp>

namespace castlecrawl
{

    void PopupManager::setup(Context & context, const std::string & message)
    {
        text.setString(message);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(70);
        text.setFont(context.media.font());

        fade_rectangle.setSize(context.layout.windowSize());
        fade_rectangle.setFillColor(sf::Color(0, 0, 0, 150));

        background_rectangle.setFillColor(sf::Color(25, 100, 115));
        background_rectangle.setOutlineColor(sf::Color(255, 255, 255, 200));
        background_rectangle.setOutlineThickness(2.0f);
        background_rectangle.setPosition(0.0f, context.layout.windowSize().y * 0.2f);

        background_rectangle.setSize(
            sf::Vector2f{ context.layout.windowSize().x, context.layout.windowSize().y * 0.2f });

        util::centerInside(context.popup.text, background_rectangle.getGlobalBounds());

        key = sf::Keyboard::Unknown;
    }

    void PopupManager::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(fade_rectangle, states);
        target.draw(background_rectangle, states);
        target.draw(text, states);
    }

} // namespace castlecrawl
