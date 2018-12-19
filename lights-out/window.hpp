#ifndef LIGHTS_OUT_WINDOW_UTIL_HPP_INCLUDED
#define LIGHTS_OUT_WINDOW_UTIL_HPP_INCLUDED

#include "game-board-model.hpp"

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

        void handleEvents(GameBoardModel & gameBoardModel);

        void drawRectangle(const sf::FloatRect & REGION, const sf::Color & COLOR);

        void draw(const sf::Drawable & DRAWABLE) { m_renderWindow.draw(DRAWABLE); }

        void clear();

        void display();

        sf::Vector2f centerPositionOf(const sf::Vector2f & SIZE_V)
        {
            const sf::Vector2f WINDOW_CENTER_V(size() * 0.5f);
            const sf::Vector2f SIZE_HALF_V(SIZE_V * 0.5f);
            return (WINDOW_CENTER_V - SIZE_HALF_V);
        }

        sf::Vector2f centerPositionOf(const sf::FloatRect & RECT)
        {
            return centerPositionOf(sf::Vector2f(RECT.width, RECT.height));
        }

        void spriteFitHorizontalAndCenter(sf::Sprite & sprite)
        {
            const float SCALE(size().y / sprite.getLocalBounds().height);
            sprite.setScale(SCALE, SCALE);
            sprite.setPosition(centerPositionOf(sprite.getGlobalBounds()));
        }

    private:
        sf::RenderWindow m_renderWindow;
        sf::Color m_backgroundColor;
    };
} // namespace lightsout

#endif // LIGHTS_OUT_WINDOW_UTIL_HPP_INCLUDED
