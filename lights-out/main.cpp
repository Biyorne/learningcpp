#include "cell-model.hpp"
#include "cell-view.hpp"
#include "game-board-model.hpp"
#include "game-board-view-fade.hpp"
#include "window.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

int main()
{

    sf::Font font;
    font.loadFromFile("bpdots.otf");

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(60);
    text.setFillColor(sf::Color::White);
    text.setString("LIGHTS OUT");
    text.setOrigin(text.getLocalBounds().left, text.getLocalBounds().top);

    lightsout::Window window("Lights Out", 800, 600, sf::Color::Black);

    text.setPosition(window.centerPositionOf(text.getGlobalBounds()));

    lightsout::GameBoardModel gameBoardModel(sf::FloatRect(sf::Vector2f(), window.size()));

    lightsout::GameBoardViewFade gameBoardView(sf::Color(121, 50, 105), gameBoardModel);

    sf::Clock frameClock;

    while (window.isOpen() && (gameBoardModel.isGameOver() == false))
    {
        window.clear();
        const float FRAME_TIME_SEC(frameClock.getElapsedTime().asSeconds());
        frameClock.restart();

        window.handleEvents(gameBoardModel);
        gameBoardView.update(FRAME_TIME_SEC, gameBoardModel);
        gameBoardView.draw(gameBoardModel, window);

        window.draw(text);

        window.display();
    }

    return EXIT_SUCCESS;
}
