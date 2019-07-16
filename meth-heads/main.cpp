#include "audio.hpp"
#include "cell.hpp"
#include "display-constants.hpp"
#include "meth-head-enum.hpp"
#include "meth-head.hpp"
#include "random.hpp"
#include "utils.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

#include <SFML/Graphics.hpp>

void printResults(const std::size_t lazyFinalScore, const std::size_t greedyFinalScore);

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

    BoardMap_t gameBoard(displayConstants.makeGameBoard());

    Lazy lazy(displayConstants, "image/head-1.png", sf::Vector2i(9, 0), gameBoard);
    Greedy greedy(displayConstants, "image/head-2.png", sf::Vector2i(11, 0), gameBoard);

    // loot creation
    for (int i(0); i < 5; ++i)
    {
        MethHeadBase::spawnLoot(gameBoard, random);
    }

    sf::Text lootText(displayConstants.default_text);
    lootText.setFillColor(sf::Color::Yellow);

    // Score Column Drawing Here
    sf::RectangleShape lazyScoreRectangle;
    lazyScoreRectangle.setFillColor(displayConstants.lazy_color);

    sf::RectangleShape greedyScoreRectangle;
    greedyScoreRectangle.setFillColor(displayConstants.greedy_color);

    sf::Clock frameClock;
    std::size_t frameCount(0);
    std::size_t turnsPerSecMax(0);

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

        if (!window.isOpen())
        {
            break;
        }

        // sf::sleep(sf::seconds(1.0f));

        ++frameCount;
        const float elapsedTimeSec(frameClock.getElapsedTime().asSeconds());

        if (elapsedTimeSec > 1.0f)
        {
            std::cout << "FPS: " << frameCount << std::endl;

            if (turnsPerSecMax < frameCount)
            {
                turnsPerSecMax = frameCount;
            }

            frameCount = 0;
            frameClock.restart();
        }

        lazy.act(displayConstants, gameBoard, audio, random);
        greedy.act(displayConstants, gameBoard, audio, random);

        scoreBarSetup(
            lazy.getScore(),
            greedy.getScore(),
            lazyScoreRectangle,
            greedyScoreRectangle,
            displayConstants);

        window.clear();
        for (const auto & cellPair : gameBoard)
        {
            window.draw(cellPair.second.rectangle);

            if (cellPair.second.loot > 0)
            {
                placeInBounds(lootSprite, cellPair.second.bounds());
                window.draw(lootSprite);

                lootText.setString(std::to_string(cellPair.second.loot));
                placeInBounds(lootText, cellPair.second.bounds());
                window.draw(lootText);
            }
        }

        window.draw(lazyScoreRectangle);
        window.draw(greedyScoreRectangle);

        window.draw(lazy);
        window.draw(greedy);

        window.display();
    }

    std::cout << "Maximum turns per second was: " << turnsPerSecMax << std::endl;

    printResults(lazy.getScore(), greedy.getScore());
}

void printResults(const std::size_t lazyFinalScore, const std::size_t greedyFinalScore)
{
    if (lazyFinalScore == greedyFinalScore)
    {
        std::cout << "Lazy and Greedy tied at " << lazyFinalScore << std::endl;
    }
    else
    {
        std::cout << "Lazy Score = " << lazyFinalScore << ", "
                  << "Greedy Score = " << greedyFinalScore << ", "
                  << ((greedyFinalScore > lazyFinalScore) ? "Greedy" : "Lazy");

        if ((lazyFinalScore == 0) || (greedyFinalScore == 0))
        {
            std::cout << " wins!" << std::endl;
        }
        else
        {
            const float loserScoreRatio(
                static_cast<float>(std::min(lazyFinalScore, greedyFinalScore))
                / static_cast<float>(std::max(lazyFinalScore, greedyFinalScore)));

            const float winnerScoreRatio(1.0f - loserScoreRatio);
            const float winnerScorePercent(winnerScoreRatio * 100.0f);

            std::cout << " wins by " << std::setprecision(3) << winnerScorePercent << "%"
                      << std::endl;
        }
    }
}
