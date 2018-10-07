#include "cell.hpp"
#include "game.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

void eventHandler(const sf::Event & EVENT, sf::RenderWindow & window, GameBoard & gameBoard);

int main()
{
    const unsigned int SCREEN_WIDTH(800);
    const unsigned int SCREEN_HEIGHT(600);

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Lights Out");

    GameBoard gameBoard(SCREEN_WIDTH, SCREEN_HEIGHT);

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
