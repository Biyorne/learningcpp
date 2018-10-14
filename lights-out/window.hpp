#ifndef LIGHTS_OUT_WINDOW_UTIL_HPP_INCLUDED
#define LIGHTS_OUT_WINDOW_UTIL_HPP_INCLUDED

#include "game-board.hpp"

#include <SFML/Graphics.hpp>

namespace lightsout
{
    class Window
    {
    public:
        Window(
            const std::string & TITLE,
            const unsigned int WIDTH,
            const unsigned int HEIGHT,
            const sf::Color & COLOR);

        sf::Vector2f size() const { return sf::Vector2f(m_renderWindow.getSize()); }

        bool isOpen() const { return m_renderWindow.isOpen(); }

        void handleEvents(GameBoard & gameBoard);

        void draw(const GameBoard & GAME_BOARD);

    private:
        sf::RenderWindow m_renderWindow;
        sf::Color m_backgroundColor;
    };
} // namespace lightsout

#endif // LIGHTS_OUT_WINDOW_UTIL_HPP_INCLUDED
