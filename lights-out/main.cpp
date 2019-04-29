// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "game-controller.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
    lightsout::GameController gameController;

    sf::Clock clock;

    while (gameController.isRunning())
    {
        const float FRAME_TIME_SEC(clock.getElapsedTime().asSeconds());
        clock.restart();

        gameController.handleEvents();
        gameController.draw(FRAME_TIME_SEC);
    }

    sf::sleep(sf::seconds(3.0f));

    return EXIT_SUCCESS;
}
