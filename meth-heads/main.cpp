#include "audio.hpp"
#include "cell-content.hpp"
#include "display-constants.hpp"
#include "meth-head-enum.hpp"
#include "meth-head.hpp"
#include "utils.hpp"

#include <cmath>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

#include <SFML/Graphics.hpp>

int main()
{
    methhead::Audio audio;

    sf::Texture lootTexture;
    lootTexture.loadFromFile("image/loot.png");

    sf::Sprite lootSprite(lootTexture);
    lootSprite.setColor(sf::Color(255, 255, 255, 127));

    sf::VideoMode videoMode(sf::VideoMode::getDesktopMode());
    sf::RenderWindow window(videoMode, "Meth Heads", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);
    // window.setFramerateLimit(60);

    const methhead::DisplayConstants displayConstants(window.getSize());

    std::map<sf::Vector2i, methhead::CellContent> gameBoard;

    for (const methhead::CellPositions & cellPositions : displayConstants.positions)
    {
        gameBoard.insert(std::make_pair(
            cellPositions.board,
            methhead::CellContent(cellPositions.screen, displayConstants.cell_size)));
    }

    methhead::MethHead lazy(
        displayConstants,
        methhead::Motivation::lazy,
        "image/head-1.png",
        sf::Vector2i(9, 0),
        gameBoard[sf::Vector2i(9, 0)].region);

    methhead::MethHead greedy(
        displayConstants,
        methhead::Motivation::greedy,
        "image/head-2.png",
        sf::Vector2i(11, 0),
        gameBoard[sf::Vector2i(11, 0)].region);

    gameBoard.find(sf::Vector2i(1, 1))->second.loot = 1;
    gameBoard.find(sf::Vector2i(5, 5))->second.loot = 10;
    gameBoard.find(sf::Vector2i(10, 10))->second.loot = 100;
    gameBoard.find(sf::Vector2i(17, 11))->second.loot = 5;
    gameBoard.find(sf::Vector2i(15, 2))->second.loot = 75;
    gameBoard.find(sf::Vector2i(12, 19))->second.loot = 50;

    // Score Column Drawing Here
    sf::RectangleShape lazyScoreRectangle;
    lazyScoreRectangle.setFillColor(displayConstants.lazy_color);

    sf::RectangleShape greedyScoreRectangle;
    greedyScoreRectangle.setFillColor(displayConstants.greedy_color);

    const float secondsPerTurn(0.5f);

    sf::Clock frameClock;
    std::size_t frameCount(0);

    sf::Text lootText(displayConstants.default_text);
    lootText.setFillColor(sf::Color::Yellow);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) || (event.type == sf::Event::MouseButtonPressed))
            {
                window.close();
            }

            if (sf::Event::KeyPressed == event.type)
            {
                if (sf::Keyboard::Up == event.key.code)
                {
                    audio.volumeUp();
                }
                else if (sf::Keyboard::Down == event.key.code)
                {
                    audio.volumeDown();
                }
                else
                {
                    window.close();
                }
            }
        }

        ++frameCount;
        const float elapsedTimeSec(frameClock.getElapsedTime().asSeconds());

        if (elapsedTimeSec > secondsPerTurn)
        {
            std::cout << "FPS: " << (static_cast<float>(frameCount) / elapsedTimeSec) << std::endl;

            frameCount = 0;
            frameClock.restart();

            lazy.act(displayConstants, gameBoard, audio);
            greedy.act(displayConstants, gameBoard, audio);
        }

        scoreBarSetup(
            lazy.getScore(),
            greedy.getScore(),
            lazyScoreRectangle,
            greedyScoreRectangle,
            displayConstants);

        window.clear();
        for (const auto & rectangle : displayConstants.rectangles)
        {
            window.draw(rectangle);
        }

        window.draw(lazyScoreRectangle);
        window.draw(greedyScoreRectangle);

        for (const auto & posContentPair : gameBoard)
        {
            if (posContentPair.second.loot > 0)
            {
                methhead::setSpriteRegion(lootSprite, posContentPair.second.region);
                window.draw(lootSprite);

                lootText.setString(std::to_string(posContentPair.second.loot));
                methhead::setTextToRegion(lootText, posContentPair.second.region);
                window.draw(lootText);
            }
        }

        window.draw(lazy);
        window.draw(greedy);

        window.display();
    }
}
