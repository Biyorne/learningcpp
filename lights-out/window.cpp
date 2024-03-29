// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "window.hpp"

namespace lightsout
{

    Window::Window(
        const std::string & TITLE,
        const unsigned int WIDTH,
        const unsigned int HEIGHT,
        const sf::Color & COLOR)
        : m_renderWindow(
              sf::VideoMode(WIDTH, HEIGHT, sf::VideoMode::getDesktopMode().bitsPerPixel),
              TITLE,
              (sf::Style::Close | sf::Style::Titlebar))
        , m_backgroundColor(COLOR)
    {
        m_renderWindow.setFramerateLimit(60);
    }

    std::vector<sf::Event> Window::gatherEvents()
    {
        std::vector<sf::Event> events;
        sf::Event event;

        while (m_renderWindow.pollEvent(event))
        {
            events.push_back(event);
        }

        return events;
    }

    void Window::drawRectangle(const sf::FloatRect & REGION, const sf::Color & COLOR)
    {
        sf::RectangleShape rectangle;
        rectangle.setFillColor(COLOR);
        rectangle.setPosition(REGION.left, REGION.top);
        rectangle.setSize(sf::Vector2f(REGION.width, REGION.height));
        m_renderWindow.draw(rectangle);
    }

    void Window::spriteFitHorizontalAndCenter(sf::Sprite & sprite) const
    {
        if ((0.0f < sprite.getLocalBounds().height) == false)
        {
            return;
        }

        const float SCALE(size().y / sprite.getLocalBounds().height);
        sprite.setScale(SCALE, SCALE);
        sprite.setPosition(centerPositionOf(sprite.getGlobalBounds()));
    }

} // namespace lightsout
