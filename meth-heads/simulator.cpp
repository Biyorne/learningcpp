// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "simulator.hpp"

#include "audio.hpp"
#include "error-handling.hpp"
#include "utils.hpp"

#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <tuple>

#include <SFML/Graphics.hpp>

namespace methhead
{
    Simulator::Simulator()
        : m_window(sf::VideoMode::getDesktopMode(), "Meth Heads", sf::Style::Default)
        , m_audio()
        , m_random()
        , m_displayConstants(m_window.getSize())
        , m_board(m_displayConstants.makeBoard())
        , m_actors()
    {
        // loot creation
        for (int i(0); i < 5; ++i)
        {
            // TODO make board class and move spawn loot there.
            MethHeadBase::spawnLoot(m_board, m_random);
        }

        spawnMethHead(Motivation::lazy);
        spawnMethHead(Motivation::greedy);
    }

    void Simulator::run()
    {
        // TODO Put in a display thing
        sf::Texture lootTexture;
        lootTexture.loadFromFile("image/loot.png");
        sf::Sprite lootSprite(lootTexture);
        lootSprite.setColor(sf::Color(255, 255, 255, 127));
        sf::Text lootText(m_displayConstants.default_text);
        lootText.setFillColor(sf::Color::Yellow);

        // Score Column Objects Here
        sf::RectangleShape lazyScoreRectangle;
        lazyScoreRectangle.setFillColor(m_displayConstants.lazy_color);
        sf::RectangleShape greedyScoreRectangle;
        greedyScoreRectangle.setFillColor(m_displayConstants.greedy_color);

        bool isDisplayEnabled(true);

        std::size_t firstLazyScore(0);
        std::size_t firstGreedyScore(0);

        sf::Clock frameClock;
        std::size_t frameCount(0);
        std::size_t turnsPerSecMax(0);

        sf::Clock fiveSecClock;

        while (m_window.isOpen())
        {
            if (isDisplayEnabled)
            {
                sf::Event event;
                while (m_window.pollEvent(event))
                {
                    if ((event.type == sf::Event::Closed)
                        || (event.type == sf::Event::MouseButtonPressed))
                    {
                        m_window.close();
                    }

                    if (sf::Event::KeyPressed == event.type)
                    {
                        if (sf::Keyboard::Up == event.key.code)
                        {
                            m_audio.volumeUp();
                        }
                        else if (sf::Keyboard::Down == event.key.code)
                        {
                            m_audio.volumeDown();
                        }
                        else if (sf::Keyboard::L == event.key.code)
                        {
                            spawnMethHead(Motivation::lazy);
                        }
                        else if (sf::Keyboard::G == event.key.code)
                        {
                            spawnMethHead(Motivation::greedy);
                        }
                        else if (sf::Keyboard::S == event.key.code)
                        {
                            spawnMethHead(Motivation::lazy);

                            spawnMethHead(Motivation::greedy);
                        }
                        else
                        {
                            m_window.close();
                        }
                    }
                }

                if (!m_window.isOpen())
                {
                    break;
                }
            }
            else
            {
                sf::Event e;
                m_window.pollEvent(e);
            }

            ++frameCount;
            const float elapsedTimeSec(frameClock.getElapsedTime().asSeconds());

            if (!isDisplayEnabled && (elapsedTimeSec > 1.0f))
            {
                if (turnsPerSecMax < frameCount)
                {
                    turnsPerSecMax = frameCount;
                }

                std::cout << "turns_per_sec=" << frameCount
                          << ", lazy_score=" << m_actors.front()->getScore()
                          << ", greedy_score=" << m_actors.back()->getScore()
                          << ", turns_per_sec_max=" << turnsPerSecMax << std::endl;

                frameCount = 0;
                frameClock.restart();
            }

            for (IActorUPtr_t & uptr : m_actors)
            {
                uptr->act(m_displayConstants, m_board, m_audio, m_random);
            }

            if (isDisplayEnabled)
            {
                sf::sleep(sf::seconds(1.0f));

                // TODO quick hack to get scores on the screen, if any
                for (const IActorUPtr_t & uptr : m_actors)
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
                    m_displayConstants);

                m_window.clear();
                for (const auto & cellPair : m_board)
                {
                    m_window.draw(cellPair.second.rectangle);

                    if (cellPair.second.loot > 0)
                    {
                        placeInBounds(lootSprite, cellPair.second.bounds());
                        m_window.draw(lootSprite);

                        lootText.setString(std::to_string(cellPair.second.loot));
                        placeInBounds(lootText, cellPair.second.bounds());
                        m_window.draw(lootText);
                    }
                }

                m_window.draw(lazyScoreRectangle);
                m_window.draw(greedyScoreRectangle);

                for (IActorUPtr_t & uptr : m_actors)
                {
                    uptr->draw(m_window, sf::RenderStates());
                }

                m_window.display();
            }
        }

        std::cout << "Maximum turns per second was: " << turnsPerSecMax << std::endl;

        if (isDisplayEnabled)
        {
            printResults(firstLazyScore, firstGreedyScore);
        }
        else
        {
            printResults(m_actors.front()->getScore(), m_actors.back()->getScore());
        }
    }

    void Simulator::spawnMethHead(const Motivation motive)
    {
        assertOrThrow((motive != Motivation::none), "Tried to spawn a None meth head.");

        // TODO only added to find a bug REMOVE
        std::vector<sf::Vector2i> cellPositions;

        try
        {
            cellPositions = MethHeadBase::makeUnoccupiedCellPositions(m_board);
        }
        catch (...)
        {
            std::cout << "makeUnoccupiedCellPositions() threw the execption, but was called by "
                         "spawnMethHead()"
                      << std::endl;

            throw;
        }

        if (cellPositions.empty())
        {
            std::cout << "spawnMethHead() failed to find any unoccupied cells." << std::endl;
        }
        else
        {
            const sf::Vector2i randomCellPos(m_random.select(cellPositions));
            m_board[randomCellPos].motivation = motive;

            if (motive == Motivation::lazy)
            {
                m_actors.push_back(std::make_unique<Lazy>(
                    m_displayConstants, "image/head-1.png", randomCellPos, m_board));
            }
            else
            {
                m_actors.push_back(std::make_unique<Greedy>(
                    m_displayConstants, "image/head-2.png", randomCellPos, m_board));
            }
        }
    }

    void Simulator::printResults(
        const std::size_t lazyFinalScore, const std::size_t greedyFinalScore)
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

} // namespace methhead
