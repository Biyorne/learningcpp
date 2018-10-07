#include "cell.hpp"
#include "game-board.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

void eventHandler(const sf::Event & EVENT, sf::RenderWindow & window, GameBoard & gameBoard);

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Lights Out");
    const sf::Vector2f WHOLE_SCREEN_SIZE_V(window.getSize());
    const sf::FloatRect WHOLE_SCREEN_REGION(
        sf::Vector2f(100.0f, 100.0f),
        sf::Vector2f((WHOLE_SCREEN_SIZE_V.x - 200.0f), (WHOLE_SCREEN_SIZE_V.y - 200.0f)));

    GameBoard gameBoard(WHOLE_SCREEN_REGION, sf::Color(121, 50, 105));

    const sf::Color CELL_OUTLINE_COLOR(sf::Color::Black);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            eventHandler(event, window, gameBoard);
        }

        window.clear(CELL_OUTLINE_COLOR);

        for (const Cell & CELL : gameBoard.cells())
        {
            window.draw(CELL.rectangle());
        }

        window.display();
    }

    return EXIT_SUCCESS;
}

void eventHandler(const sf::Event & EVENT, sf::RenderWindow & window, GameBoard & gameBoard)
{
    if (EVENT.type == sf::Event::Closed)
    {
        window.close();
    }

    if (EVENT.type == sf::Event::KeyPressed)
    {
        if (EVENT.key.code == sf::Keyboard::U)
        {
            gameBoard.undo();
        }
        else if (EVENT.key.code == sf::Keyboard::F)
        {
            gameBoard.flip();
        }
        else if (EVENT.key.code == sf::Keyboard::R)
        {
            gameBoard.reset();
        }
        else
        {
            window.close();
        }
    }

    if (EVENT.type == sf::Event::MouseButtonPressed)
    {
        const sf::Vector2f MOUSE_POSITION_V(sf::Vector2i(EVENT.mouseButton.x, EVENT.mouseButton.y));
        gameBoard.handleMouseClick(MOUSE_POSITION_V);
    }

    if (gameBoard.isGameBoardOver())
    {
        window.close();
    }
}
