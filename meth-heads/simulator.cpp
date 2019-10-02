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
    Simulator::Simulator(const Mode mode)
        : m_mode(mode)
        , m_videoMode(1280u, 1024u, sf::VideoMode::getDesktopMode().bitsPerPixel)
        , m_window()
        , m_random()
        , m_audio(mode, m_random)
        , m_displayVars(sf::Vector2u(m_videoMode.width, m_videoMode.height))
        , m_board(m_displayVars.makeBoard())
        , m_actorTurnIndex(0)
        , m_actors()
        , m_secondsPerTurn(1.0f)
        , m_secondsPerTurnMultipler(1.0f)
        , m_consoleStatusClock()
        , m_consoleStatusFrameCount(0)
        , m_consoleStatusFrameCountMax(0)
        , m_lootTexture()
    {
        if (Mode::Normal == m_mode)
        {
            m_window.create(m_videoMode, "Meth Heads", sf::Style::Fullscreen);
        }
        else
        {
            std::cout << "Graphics disabled during speed tests." << std::endl;
        }

        if (!m_lootTexture.loadFromFile("image/loot.png"))
        {
            std::cout << "Error:  Unable to load loot image from: image/loot.png" << std::endl;
        }

        const std::size_t initialLootCount(5);
        for (std::size_t i(0); i < initialLootCount; ++i)
        {
            MethHeadBase::spawnLoot(m_board, m_random);
        }

        spawnMethHead(Motivation::lazy);
        spawnMethHead(Motivation::greedy);
    }

    void Simulator::run()
    {
        while (willKeepRunning())
        {
            handleEvents();
            update();

            if (m_mode == Mode::Normal)
            {
                draw();
                sf::sleep(sf::seconds(m_secondsPerTurn));
            }
            else
            {
                consoleStatus();
            }
        }
    }

    bool Simulator::willKeepRunning() const
    {
        if (Mode::Normal == m_mode)
        {
            return m_window.isOpen();
        }
        else
        {
            return true;
        }
    }

    void Simulator::updateSecondsPerTurn()
    {
        if ((Mode::SpeedTest == m_mode) || m_actors.empty())
        {
            m_secondsPerTurn = 1.0f;
            m_secondsPerTurnMultipler = 1.0f;
        }
        else
        {
            m_secondsPerTurnMultipler = std::clamp(m_secondsPerTurnMultipler, 0.0001f, 10000.0f);

            m_secondsPerTurn = (1.0f / static_cast<float>(m_actors.size()));
            m_secondsPerTurn *= m_secondsPerTurnMultipler;

            m_secondsPerTurn = std::clamp(m_secondsPerTurn, 0.0001f, 0.5f);
        }
    }

    void Simulator::consoleStatus()
    {
        ++m_consoleStatusFrameCount;

        if (m_consoleStatusClock.getElapsedTime().asSeconds() > 1.0f)
        {
            if (m_consoleStatusFrameCountMax < m_consoleStatusFrameCount)
            {
                m_consoleStatusFrameCountMax = m_consoleStatusFrameCount;
            }

            printConsoleStatus();

            m_consoleStatusFrameCount = 0;
            m_consoleStatusClock.restart();
        }
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

        const sf::Vector2i boardPos(m_random.from(cellPositions));

        Cell & cell(m_board[boardPos]);
        cell.motivation = motive;

        if (motive == Motivation::lazy)
        {
            m_actors.push_back(std::make_unique<Lazy>("image/head-1.png", boardPos, cell.bounds()));
        }
        else
        {
            m_actors.push_back(
                std::make_unique<Greedy>("image/head-2.png", boardPos, cell.bounds()));
        }

        updateSecondsPerTurn();
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
        if (m_mode == Mode::SpeedTest)
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
            else if (sf::Keyboard::Left == event.key.code)
            {
                m_secondsPerTurnMultipler *= 1.25f;
                updateSecondsPerTurn();
            }
            else if (sf::Keyboard::Right == event.key.code)
            {
                m_secondsPerTurnMultipler *= 0.75f;
                updateSecondsPerTurn();
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

    void Simulator::update()
    {
        if (m_actors.empty())
        {
            return;
        }

        if (m_actorTurnIndex >= m_actors.size())
        {
            m_actorTurnIndex = 0;
        }

        m_actors.at(m_actorTurnIndex)->act(m_board, m_audio, m_random);

        ++m_actorTurnIndex;
    }

    void Simulator::printConsoleStatus()
    {
        auto printWinnerPercentString = [&](const Scores & scores) {
            const std::size_t lowScore(std::min(scores.lazy, scores.greedy));
            if (0 == lowScore)
            {
                return;
            }

            const float highScoreF(static_cast<float>(std::max(scores.lazy, scores.greedy)));
            const float lowScoreF(static_cast<float>(lowScore));

            const float winRatio(1.0f - (lowScoreF / highScoreF));
            const std::size_t winPercent(static_cast<std::size_t>(winRatio * 100.0f));

            std::cout << winPercent << "%";
        };

        const Scores scores(calcScores());

        std::cout << "fps=" << m_consoleStatusFrameCount;
        std::cout << " (" << m_consoleStatusFrameCountMax << ")";
        std::cout << ",   lazy=" << scores.lazy;
        std::cout << ",   greedy=" << scores.lazy;

        if (scores.greedy > scores.lazy)
        {
            std::cout << "\t\t GREEDY winning by ";
            printWinnerPercentString(scores);
        }
        else if (scores.lazy > scores.greedy)
        {
            std::cout << "\t\t LAZY winning by ";
            printWinnerPercentString(scores);
        }

        std::cout << std::endl;
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

        m_displayVars.scoreBarSetup(
            scores.lazy, scores.greedy, lazyScoreRectangle, greedyScoreRectangle);

        // TODO put somewhere else, look at the TODO above for where
        sf::Sprite lootSprite(m_lootTexture);
        lootSprite.setColor(sf::Color(255, 255, 255, 127));
        sf::Text lootText(m_displayVars.constants().default_text);
        lootText.setFillColor(sf::Color::Yellow);

        m_window.clear(sf::Color(64, 64, 64));

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