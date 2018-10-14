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
    {}

    void Window::handleEvents(GameBoard & gameBoard)
    {
        sf::Event event;
        while (m_renderWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                m_renderWindow.close();
            }

            gameBoard.eventHandler(event);
        }
    }

    void Window::draw(const GameBoard & GAME_BOARD)
    {
        m_renderWindow.clear(m_backgroundColor);

        for (const lightsout::Cell & CELL : GAME_BOARD.cells())
        {
            m_renderWindow.draw(CELL.rectangle());
        }

        m_renderWindow.display();
    }

} // namespace lightsout
