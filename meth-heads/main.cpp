#include "audio.hpp"
#include "cell-content.hpp"
#include "display-constants.hpp"
#include "meth-head-enum.hpp"
#include "meth-head.hpp"
#include "random.hpp"
#include "utils.hpp"

#include <cmath>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

#include <SFML/Graphics.hpp>

int main()
{
    using namespace methhead;

    Random random;
    Audio audio;

    sf::Texture lootTexture;
    lootTexture.loadFromFile("image/loot.png");

    sf::Sprite lootSprite(lootTexture);
    lootSprite.setColor(sf::Color(255, 255, 255, 127));

    sf::VideoMode videoMode(sf::VideoMode::getDesktopMode());
    sf::RenderWindow window(videoMode, "Meth Heads", sf::Style::Fullscreen);
    // window.setVerticalSyncEnabled(true);
    // window.setFramerateLimit(60);

    const DisplayConstants displayConstants(window.getSize());

    std::map<sf::Vector2i, CellContent> gameBoard;

    for (const CellPositions & cellPositions : displayConstants.positions)
    {
        gameBoard.insert(std::make_pair(
            cellPositions.board, CellContent(cellPositions.screen, displayConstants.cell_size)));
    }

    Lazy lazy(displayConstants, "image/head-1.png", sf::Vector2i(9, 0), gameBoard);
    Greedy greedy(displayConstants, "image/head-2.png", sf::Vector2i(11, 0), gameBoard);

    for (int i(0); i < 5; ++i)
    {
        MethHeadBase::spawnLoot(gameBoard, random);
    }

    // Score Column Drawing Here
    sf::RectangleShape lazyScoreRectangle;
    lazyScoreRectangle.setFillColor(displayConstants.lazy_color);

    sf::RectangleShape greedyScoreRectangle;
    greedyScoreRectangle.setFillColor(displayConstants.greedy_color);

    float secondsPerTurn(0.5f);

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
                else if (sf::Keyboard::Left == event.key.code)
                {
                    secondsPerTurn *= 1.1f;

                    if (secondsPerTurn > 2.0f)
                    {
                        secondsPerTurn = 2.0f;
                    }
                }
                else if (sf::Keyboard::Right == event.key.code)
                {
                    secondsPerTurn *= 0.9f;
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

            lazy.act(displayConstants, gameBoard, audio, random);
            greedy.act(displayConstants, gameBoard, audio, random);
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
                placeInRegion(lootSprite, posContentPair.second.region);
                window.draw(lootSprite);

                lootText.setString(std::to_string(posContentPair.second.loot));
                placeInRegion(lootText, posContentPair.second.region);
                window.draw(lootText);
            }
        }

        window.draw(lazy);
        window.draw(greedy);

        window.display();
    }
}
