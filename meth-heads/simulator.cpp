// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "simulator.hpp"

#include "animation-player.hpp"
#include "error-handling.hpp"
#include "sound-player.hpp"
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
        , m_soundPlayer()
        , m_animationPlayer()
        , m_displayVars(sf::Vector2u(m_videoMode.width, m_videoMode.height))
        , m_actors()
        , m_frameClock()
        , m_timeMultiplier(1.0f)
        , m_consoleStatusClock()
        , m_consoleStatusFrameCount(0)
        , m_consoleStatusFrameCountMax(0)
    {
        if (Mode::Normal == m_mode)
        {
            m_window.create(m_videoMode, "Meth Heads", sf::Style::Default);
            m_window.setFramerateLimit(60);
        }

        const std::size_t initialLootCount(5);
        for (std::size_t i(0); i < initialLootCount; ++i)
        {
            spawnLoot();
        }

        spawnMethHead(Motivation::lazy);
        spawnMethHead(Motivation::greedy);
    }

    void Simulator::run()
    {
        while (willKeepRunning())
        {
            if (Mode::Normal == m_mode)
            {
                handleEvents();

                const float actualElapsedSec(m_frameClock.getElapsedTime().asSeconds());
                m_frameClock.restart();
                const float gameElapsedSec(actualElapsedSec * m_timeMultiplier);

                update(gameElapsedSec);
                draw();
            }
            else
            {
                update(1.0f);
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

    std::pair<BoardPos_t, sf::FloatRect> Simulator::randomSpawnPos() const
    {
        BoardMap_t onScreenMap(m_displayVars.constants().board_map);

        for (const IActorUPtr_t & actor : m_actors)
        {
            onScreenMap.erase(actor->boardPos());
        }

        for (const IPickupUPtr_t & pickup : m_pickups)
        {
            onScreenMap.erase(pickup->boardPos());
        }

        if (onScreenMap.empty())
        {
            return m_displayVars.constants().off_screen_pair;
        }

        const auto & [boardPos, rectangleShape] = m_random.from(onScreenMap);

        return { boardPos, rectangleShape.getGlobalBounds() };
    }

    void Simulator::spawnMethHead(const Motivation motive)
    {
        assertOrThrow((motive != Motivation::none), "Tried to spawn a None meth head.");

        const auto & [boardPos, floatRect] = randomSpawnPos();

        if (motive == Motivation::lazy)
        {
            m_actors.push_back(std::make_unique<Lazy>("image/head-1.png", boardPos, floatRect));
        }
        else
        {
            m_actors.push_back(std::make_unique<Greedy>("image/head-2.png", boardPos, floatRect));
        }
    }

    void Simulator::spawnLoot()
    {
        const auto & [boardPos, floatRect] = randomSpawnPos();

        m_pickups.push_back(std::move(std::make_unique<Loot>(
            "image/loot.png",
            boardPos,
            floatRect,
            m_random.fromTo(1, 100),
            m_displayVars.constants())));
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
                m_soundPlayer.volumeUp();
            }
            else if (sf::Keyboard::Down == event.key.code)
            {
                m_soundPlayer.volumeDown();
            }
            else if (sf::Keyboard::Left == event.key.code)
            {
                m_timeMultiplier = std::clamp((m_timeMultiplier *= 0.5f), 0.1f, 1000.0f);
            }
            else if (sf::Keyboard::Right == event.key.code)
            {
                m_timeMultiplier = std::clamp((m_timeMultiplier *= 1.5f), 0.1f, 1000.0f);
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

    void Simulator::update(const float elapsedSec)
    {
        m_animationPlayer.update(elapsedSec);

        for (auto & actor : m_actors)
        {
            actor->update(elapsedSec, m_soundPlayer, m_random, m_animationPlayer);
        }

        const Scores scores(calcScores());
        m_displayVars.update(elapsedSec, scores.lazy, scores.greedy);
    }

    void Simulator::printConsoleStatus()
    {
        auto printWinnerPercentString = [&](const Scores & scores) {
            const int lowScore(std::min(scores.lazy, scores.greedy));
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
        std::cout << ",   greedy=" << scores.greedy;

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
        m_window.clear(sf::Color(64, 64, 64));

        const sf::RenderStates renderState;

        m_displayVars.draw(m_window, renderState);

        for (IActorUPtr_t & uptr : m_actors)
        {
            uptr->draw(m_window, renderState);
        }

        for (IPickupUPtr_t & uptr : m_pickups)
        {
            uptr->draw(m_window, renderState);
        }

        m_animationPlayer.draw(m_window, renderState);

        m_window.display();
    }

    Simulator::Scores Simulator::calcScores() const
    {
        Scores scores;

        for (const IActorUPtr_t & actorUPtr : m_actors)
        {
            if (actorUPtr->motivation() == Motivation::lazy)
            {
                scores.lazy = actorUPtr->score();
            }
            else
            {
                scores.greedy = actorUPtr->score();
            }
        }

        return scores;
    }

} // namespace methhead
