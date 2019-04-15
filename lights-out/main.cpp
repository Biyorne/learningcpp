#include "game-controller.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

//    if (lightsout::GameState::Lose == m_state)
//    {
//
//        sf::Texture loseTexture;
//        if (!loseTexture.loadFromFile("image\\lose.png"))
//        {
//            std::cerr << "Failed to load image: image\\lose.png" << std::endl;
//            return EXIT_FAILURE;
//        }
//
//        sf::Sprite loseSprite;
//        loseSprite.setTexture(loseTexture);
//        window.spriteFitHorizontalAndCenter(loseSprite);
//        const float DISPLAY_TIME_SEC(displayClock.getElapsedTime().asSeconds());
//        window.drawRectangle(
//            sf::FloatRect(sf::Vector2f(), window.size()), sf::Color(0, 0, 0, 127));
//        window.draw(loseSprite);
//
//        if (DISPLAY_TIME_SEC > 2.0f)
//        {
//            break;
//        }
//    }

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
