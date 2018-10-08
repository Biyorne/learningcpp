#include "cell.hpp"
#include "game-board.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

namespace lightsout
{
    class Window
    {
    public:
        Window(
            const std::string & TITLE,
            const unsigned int WIDTH,
            const unsigned int HEIGHT,
            const sf::Color & COLOR)
            : m_renderWindow(sf::VideoMode(WIDTH, HEIGHT), TITLE)
            , m_backgroundColor(COLOR)
        {}

        sf::Vector2f size() const { return sf::Vector2f(m_renderWindow.getSize()); }

        bool isOpen() const { return m_renderWindow.isOpen(); }

        void handleEvents(GameBoard & gameBoard)
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

        void draw(const GameBoard & GAME_BOARD)
        {
            m_renderWindow.clear(m_backgroundColor);

            for (const lightsout::Cell & CELL : GAME_BOARD.cells())
            {
                m_renderWindow.draw(CELL.rectangle());
            }

            m_renderWindow.display();
        }

    private:
        sf::RenderWindow m_renderWindow;
        sf::Color m_backgroundColor;
    };
} // namespace lightsout

int main()
{
    // sf::RenderWindow window(sf::VideoMode(800, 600), "Lights Out");
    lightsout::Window window("Lights Out", 800, 600, sf::Color::Black);

    const sf::FloatRect WHOLE_SCREEN_REGION(
        sf::Vector2f(100.0f, 100.0f), (window.size() - sf::Vector2f(200.0f, 200.0f)));

    lightsout::GameBoard gameBoard(WHOLE_SCREEN_REGION, sf::Color(121, 50, 105));

    while (window.isOpen() && (gameBoard.isGameOver() == false))
    {
        window.handleEvents(gameBoard);
        window.draw(gameBoard);
    }

    return EXIT_SUCCESS;
}
