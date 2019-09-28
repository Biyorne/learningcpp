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
    Simulator::Simulator(const Visuals visuals)
        : m_visuals(visuals)
        , m_window(
              ((visuals == Visuals::Enabled) ? sf::VideoMode::getDesktopMode() : sf::VideoMode()),
              "Meth Heads",
              sf::Style::Fullscreen)
        , m_audio()
        , m_random()
        , m_displayVars(m_window.getSize())
        , m_board(m_displayVars.makeBoard())
        , m_actors()
        , m_maxTurnsPerSec(0)
    {
        if (!m_window.isOpen())
        {
            std::cout << "Visuals are disabled.  No window." << std::endl;
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

            if (m_visuals == Visuals::Enabled)
            {
                draw();
                sf::sleep(sf::seconds(1.0f));
            }
        }

        printResults(calcScores());
    }

    void Simulator::spawnMethHead(const Motivation motive)
    {
        // TODO when cleanup is finished, we shouldn't have a none motive.
        assertOrThrow((motive != Motivation::none), "Tried to spawn a None meth head.");

        std::vector<sf::Vector2i> cellPositions(MethHeadBase::makeUnoccupiedCellPositions(m_board));

        if (cellPositions.empty())
        {
            std::cout << "spawnMethHead() failed to find any unoccupied cells." << std::endl;
            return;
        }

        const sf::Vector2i randomCellPos(m_random.select(cellPositions));
        m_board[randomCellPos].motivation = motive;

        if (motive == Motivation::lazy)
        {
            m_actors.push_back(std::make_unique<Lazy>(
                m_displayVars.constants(), "image/head-1.png", randomCellPos, m_board));
        }
        else
        {
            m_actors.push_back(std::make_unique<Greedy>(
                m_displayVars.constants(), "image/head-2.png", randomCellPos, m_board));
        }
    }

    void Simulator::printResults(const Scores & scores)
    {
        const std::size_t lazyFinalScore(scores.lazy);
        const std::size_t greedyFinalScore(scores.greedy);

        const std::size_t lowScore { std::min(lazyFinalScore, greedyFinalScore) };
        const std::size_t highScore { std::max(lazyFinalScore, greedyFinalScore) };

        float winnerScorePercent { 0.0f };

        if (highScore > 0)
        {
            const float loserScoreRatio(
                static_cast<float>(lowScore) / static_cast<float>(highScore));

            const float winnerScoreRatio(1.0f - loserScoreRatio);
            winnerScorePercent = (winnerScoreRatio * 100.0f);
        }

        std::cout << "Lazy Score:   " << std::setw(10) << std::right << lazyFinalScore;
        if (lazyFinalScore > greedyFinalScore)
        {
            std::cout << " " << std::setprecision(3) << winnerScorePercent << "%";
        }
        std::cout << std::endl;

        std::cout << "Greedy Score: " << std::setw(10) << std::right << greedyFinalScore;
        if (lazyFinalScore < greedyFinalScore)
        {
            std::cout << " " << std::setprecision(3) << winnerScorePercent << "%";
        }
        std::cout << std::endl;
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

        if (event.type == sf::Event::Closed)
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
            else if (sf::Keyboard::Escape == event.key.code)
            {
                m_window.close();
            }
        }
    }

    void Simulator::update(std::size_t & frameCount, sf::Clock & frameClock)
    {
        for (IActorUPtr_t & uptr : m_actors)
        {
            uptr->act(m_displayVars.constants(), m_board, m_audio, m_random);
        }

        if (printOncePerSecondConsoleStatus(frameClock.getElapsedTime().asSeconds(), ++frameCount))
        {
            frameCount = 0;
            frameClock.restart();
        }
    }

    bool Simulator::printOncePerSecondConsoleStatus(
        const float elapsedTimeSec, const std::size_t frameCount)
    {
        if (elapsedTimeSec > 1.0f)
        {
            if (m_maxTurnsPerSec < frameCount)
            {
                m_maxTurnsPerSec = frameCount;
            }

            const Scores scores(calcScores());

            std::cout << "turns_per_sec=" << frameCount << ", lazy_score=" << scores.lazy
                      << ", greedy_score=" << scores.greedy
                      << ", turns_per_sec_max=" << m_maxTurnsPerSec << std::endl;

            return true;
        }
        else
        {
            return false;
        }
    }

    void Simulator::draw()
    {
        const Scores scores(calcScores());

        // TODO score stuff is display stuff that is NOT constant
        // this score stuff should not be created/destroyed on the stack every frame
        // if only we had a DisplayNonConstants class or DisplayVars or DisplayState
        //..this all would go in there...TODO
        sf::RectangleShape lazyScoreRectangle;
        sf::RectangleShape greedyScoreRectangle;
        lazyScoreRectangle.setFillColor(m_displayVars.constants().lazy_color);
        greedyScoreRectangle.setFillColor(m_displayVars.constants().greedy_color);

        scoreBarSetup(
            scores.lazy,
            scores.greedy,
            lazyScoreRectangle,
            greedyScoreRectangle,
            m_displayVars.constants());

        // TODO put somewhere else, look at the TODO above for where
        sf::Texture lootTexture;
        lootTexture.loadFromFile("image/loot.png");
        sf::Sprite lootSprite(lootTexture);
        lootSprite.setColor(sf::Color(255, 255, 255, 127));
        sf::Text lootText(m_displayVars.constants().default_text);
        lootText.setFillColor(sf::Color::Yellow);

        m_window.clear();

        m_window.draw(lazyScoreRectangle);
        m_window.draw(greedyScoreRectangle);

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

        for (IActorUPtr_t & uptr : m_actors)
        {
            uptr->draw(m_window, sf::RenderStates());
        }

        m_window.display();
    }

    Simulator::Scores Simulator::calcScores() const
    {
        Scores scores;

        for (const IActorUPtr_t & actorUPtr : m_actors)
        {
            if (actorUPtr->getMotivation() == Motivation::lazy)
            {
                scores.lazy = actorUPtr->getScore();
            }
            else
            {
                scores.greedy = actorUPtr->getScore();
            }
        }

        return scores;
    }

} // namespace methhead
