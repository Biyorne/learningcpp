#ifndef LIGHTS_OUT_WINDOW_UTIL_HPP_INCLUDED
#define LIGHTS_OUT_WINDOW_UTIL_HPP_INCLUDED

#include <SFML/Graphics.hpp>

namespace lightsout
{
    class Window
    {
    public:
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
