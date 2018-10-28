#include "window.hpp"

namespace lightsout
{

    Window::Window(
        const std::string & TITLE,
        const unsigned int WIDTH,
        const unsigned int HEIGHT,
        const sf::Color & COLOR)
        : m_renderWindow(sf::VideoMode(WIDTH, HEIGHT), TITLE)
        , m_backgroundColor(COLOR)
    {
        m_renderWindow.setFramerateLimit(60);
    }

    void Window::handleEvents(GameBoardModel & gameBoardModel)
    {
        sf::Event event;
        while (m_renderWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                m_renderWindow.close();
            }

            gameBoardModel.eventHandler(event);
        }
    }

    void Window::clear() { m_renderWindow.clear(m_backgroundColor); }

    void Window::display() { m_renderWindow.display(); }

    void Window::drawRectangle(const sf::FloatRect & REGION, const sf::Color & COLOR)
    {
        sf::RectangleShape rectangle;
        rectangle.setFillColor(COLOR);
        rectangle.setPosition(REGION.left, REGION.top);
        rectangle.setSize(sf::Vector2f(REGION.width, REGION.height));
        m_renderWindow.draw(rectangle);
    }

} // namespace lightsout
