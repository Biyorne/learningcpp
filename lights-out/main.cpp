#include "cell.hpp"
#include "game-board.hpp"
#include "window.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
    lightsout::Window window("Lights Out", 800, 600, sf::Color::Black);

    lightsout::GameBoard gameBoard(
        sf::FloatRect(sf::Vector2f(), window.size()), sf::Color::Red); // sf::Color(121, 50, 105));

    sf::Clock clock;
    sf::Clock frameClock;
    std::size_t frameCounter(0);

    while (window.isOpen() && (gameBoard.isGameOver() == false))
    {
        const float FPS_TIME_SEC(clock.getElapsedTime().asSeconds());
        const float FRAME_TIME_SEC(frameClock.getElapsedTime().asSeconds());
        frameClock.restart();
        const float FPS((frameCounter / FPS_TIME_SEC));

        if (FPS_TIME_SEC > 1.0f)
        {
            std::cout << FPS << "\n";
            frameCounter = 0;
            clock.restart();
        }

        window.handleEvents(gameBoard);
        gameBoard.update(FRAME_TIME_SEC);
        window.draw(gameBoard);
        std::cout << FRAME_TIME_SEC << "\n";

        ++frameCounter;
    }

    return EXIT_SUCCESS;
}
