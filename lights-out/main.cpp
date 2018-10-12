#include "cell.hpp"
#include "game-board.hpp"
#include "window.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
    // sf::RenderWindow window(sf::VideoMode(800, 600), "Lights Out");
    lightsout::Window window("Lights Out", 800, 600, sf::Color::Black);

    lightsout::GameBoard gameBoard(
        sf::FloatRect(sf::Vector2f(), window.size()), sf::Color(121, 50, 105));

    while (window.isOpen() && (gameBoard.isGameOver() == false))
    {
        window.handleEvents(gameBoard);
        window.draw(gameBoard);
    }

    return EXIT_SUCCESS;
}
