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
    std::size_t frameCounter(0);

    while (window.isOpen() && (gameBoard.isGameOver() == false))
    {
        const float ELAPSED_TIME_SEC(clock.getElapsedTime().asSeconds());
        const float FPS((frameCounter / ELAPSED_TIME_SEC));

        if (ELAPSED_TIME_SEC > 1.0f)
        {
            std::cout << FPS << "\n";
            frameCounter = 0;
            clock.restart();
        }

        window.handleEvents(gameBoard);
        gameBoard.update(ELAPSED_TIME_SEC);
        window.draw(gameBoard);

        ++frameCounter;
    }

    return EXIT_SUCCESS;
}
