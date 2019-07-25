#include "audio.hpp"
#include "cell.hpp"
#include "display-constants.hpp"
#include "error-handling.hpp"
#include "meth-head-enum.hpp"
#include "meth-head.hpp"
#include "random.hpp"
#include "utils.hpp"

#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <tuple>
#include <vector>

#include <SFML/Graphics.hpp>

using namespace methhead;

using IActorUPtr_t = std::unique_ptr<IActor>;
using IActorUVec_t = std::vector<IActorUPtr_t>;

void printResults(const std::size_t lazyFinalScore, const std::size_t greedyFinalScore);

void spawnMethHead(
    const Motivation motive,
    std::vector<IActorUPtr_t> & actors,
    BoardMap_t & board,
    const DisplayConstants & displayConstants,
    const Random & random)
{
    assertOrThrow((motive != Motivation::none), "Tried to spawn a None meth head.");

    const auto cellPositions(MethHeadBase::makeUnoccupiedCellPositions(board));

    if (cellPositions.empty())
    {
        std::cerr << "spawnMethHead() failed to find any unoccupied cells." << std::endl;
    }
    else
    {
        const sf::Vector2i randomCellPos(random.select(cellPositions));
        board[randomCellPos].motivation = motive;

        if (motive == Motivation::lazy)
        {
            actors.push_back(
                std::make_unique<Lazy>(displayConstants, "image/head-1.png", randomCellPos, board));
        }
        else
        {
            actors.push_back(std::make_unique<Greedy>(
                displayConstants, "image/head-2.png", randomCellPos, board));
        }
    }
}

int main()
{
    Random random;
    Audio audio;

    sf::Texture lootTexture;
    lootTexture.loadFromFile("image/loot.png");

    sf::Sprite lootSprite(lootTexture);
    lootSprite.setColor(sf::Color(255, 255, 255, 127));

    sf::VideoMode videoMode(1024, 768, sf::VideoMode::getDesktopMode().bitsPerPixel);
    sf::RenderWindow window(videoMode, "Meth Heads", sf::Style::Default);
    // window.setVerticalSyncEnabled(true);
    // window.setFramerateLimit(60);

    const DisplayConstants displayConstants(window.getSize());

    BoardMap_t board(displayConstants.makeBoard());

    std::vector<IActorUPtr_t> actors;

    // loot creation
    for (int i(0); i < 5; ++i)
    {
        // TODO make board class and move spawn loot there.
        MethHeadBase::spawnLoot(board, random);
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

    sf::Clock fiveSecClock;

    std::size_t firstLazyScore(0);
    std::size_t firstGreedyScore(0);

    bool isDisplayEnabled(false);

    if (!isDisplayEnabled)
    {
        spawnMethHead(Motivation::lazy, actors, board, displayConstants, random);
        spawnMethHead(Motivation::greedy, actors, board, displayConstants, random);
    }

    while (window.isOpen())
    {
        if (isDisplayEnabled)
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if ((event.type == sf::Event::Closed)
                    || (event.type == sf::Event::MouseButtonPressed))
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
                    else if (sf::Keyboard::L == event.key.code)
                    {
                        spawnMethHead(Motivation::lazy, actors, board, displayConstants, random);
                    }
                    else if (sf::Keyboard::G == event.key.code)
                    {
                        spawnMethHead(Motivation::greedy, actors, board, displayConstants, random);
                    }
                    else if (sf::Keyboard::S == event.key.code)
                    {
                        spawnMethHead(Motivation::lazy, actors, board, displayConstants, random);
                        spawnMethHead(Motivation::greedy, actors, board, displayConstants, random);
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
        }
        else
        {
            sf::Event e;
            window.pollEvent(e);
        }

        // sf::sleep(sf::seconds(1.0f));

        // if (!isDisplayEnabled && (fiveSecClock.getElapsedTime().asSeconds() > 15.0f))
        //{
        //    window.close();
        //}

        ++frameCount;
        const float elapsedTimeSec(frameClock.getElapsedTime().asSeconds());

        if (elapsedTimeSec > 1.0f)
        {
            if (turnsPerSecMax < frameCount)
            {
                turnsPerSecMax = frameCount;
            }

            std::cout << "turns_per_sec=" << frameCount
                      << ", lazy_score=" << actors.front()->getScore()
                      << ", greedy_score=" << actors.back()->getScore()
                      << ", turns_per_sec_max=" << turnsPerSecMax << std::endl;

            frameCount = 0;
            frameClock.restart();
        }

        for (IActorUPtr_t & uptr : actors)
        {
            uptr->act(displayConstants, board, audio, random);
        }

        if (isDisplayEnabled)
        {
            // TODO quick hack to get scores on the screen, if any
            for (const IActorUPtr_t & uptr : actors)
            {
                const auto score(uptr->getScore());

                if ((uptr->getMotivation() == Motivation::lazy) && (score > firstLazyScore))
                {
                    firstLazyScore = score;
                }

                if ((uptr->getMotivation() == Motivation::greedy) && (score > firstGreedyScore))
                {
                    firstGreedyScore = score;
                }
            }

            scoreBarSetup(
                firstLazyScore,
                firstGreedyScore,
                lazyScoreRectangle,
                greedyScoreRectangle,
                displayConstants);

            window.clear();
            for (const auto & cellPair : board)
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

            for (IActorUPtr_t & uptr : actors)
            {
                uptr->draw(window, sf::RenderStates());
            }

            window.display();
        }
    }

    std::cout << "Maximum turns per second was: " << turnsPerSecMax << std::endl;

    if (isDisplayEnabled)
    {
        printResults(firstLazyScore, firstGreedyScore);
    }
    else
    {
        printResults(actors.front()->getScore(), actors.back()->getScore());
    }
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
