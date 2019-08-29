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
    Simulator::Simulator(const unsigned int windowWidth, const unsigned int windowHeight)
        : m_visuals(
              ((windowWidth > 0) && (windowHeight > 0)) ? Visuals::Enabled : Visuals::Disabled)
        , m_window()
        , m_audio()
        , m_random()
        , m_displayConstants(sf::Vector2u(windowWidth, windowHeight))
        , m_board(m_displayConstants.makeBoard())
        , m_actors()
        , m_maxIterationsPerSec(0)
    {
        if (m_visuals == Visuals::Enabled)
        {
            m_window.create(sf::VideoMode::getDesktopMode(), "Meth Heads", sf::Style::Fullscreen);
        }
        else
        {
            std::cout << "Window size of " << windowWidth << "x" << windowHeight
                      << " not valid, so visuals are disabled." << std::endl;
        }

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
        sf::Clock frameClock;
        std::size_t frameCount(0);

        while (m_window.isOpen())
        {
            handleEvents();
            update(frameCount, frameClock);
            draw();
        }

        printFinalResults();
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

    void Simulator::handleEvents()
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            handleEvent(event);
        }
    }

    void Simulator::handleEvent(const sf::Event & event)
    {
        if (m_visuals == Visuals::Disabled)
        {
            return;
        }

        if ((event.type == sf::Event::Closed) || (event.type == sf::Event::MouseButtonPressed))
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

    void Simulator::update(std::size_t & frameCount, sf::Clock & frameClock)
    {
        ++frameCount;
        const float elapsedTimeSec { frameClock.getElapsedTime().asSeconds() };

        if ((m_visuals == Visuals::Disabled) && (elapsedTimeSec > 1.0f))
        {
            if (m_maxIterationsPerSec < frameCount)
            {
                m_maxIterationsPerSec = frameCount;
            }

            std::cout << "turns_per_sec=" << frameCount
                      << ", lazy_score=" << m_actors.front()->getScore()
                      << ", greedy_score=" << m_actors.back()->getScore()
                      << ", turns_per_sec_max=" << m_maxIterationsPerSec << std::endl;

            frameCount = 0;
            frameClock.restart();
        }

        for (IActorUPtr_t & uptr : m_actors)
        {
            uptr->act(m_displayConstants, m_board, m_audio, m_random);
        }
    }

    void Simulator::draw()
    {
        if (m_visuals == Visuals::Disabled)
        {
            return;
        }

        sf::sleep(sf::seconds(1.0f));

        // TODO quick hack to get scores on the screen, if any
        std::size_t firstLazyScore(10);
        std::size_t firstGreedyScore(20);
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

        // Score Column Objects Here
        sf::RectangleShape lazyScoreRectangle;
        sf::RectangleShape greedyScoreRectangle;
        lazyScoreRectangle.setFillColor(m_displayConstants.lazy_color);
        greedyScoreRectangle.setFillColor(m_displayConstants.greedy_color);

        scoreBarSetup(
            firstLazyScore,
            firstGreedyScore,
            lazyScoreRectangle,
            greedyScoreRectangle,
            m_displayConstants);

        // TODO Put in a display thing
        sf::Texture lootTexture;
        lootTexture.loadFromFile("image/loot.png");
        sf::Sprite lootSprite(lootTexture);
        lootSprite.setColor(sf::Color(255, 255, 255, 127));
        sf::Text lootText(m_displayConstants.default_text);
        lootText.setFillColor(sf::Color::Yellow);

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

    void Simulator::printFinalResults()
    {
        std::cout << "Maximum turns per second was: " << m_maxIterationsPerSec << std::endl;
        //
        // TODO Fix so that the same scores are shown at the end of every run regardless of
        // display.
        if (m_visuals == Visuals::Enabled)
        {
            // TODO
        }
        else
        {
            // TODO
        }
    }

} // namespace methhead
