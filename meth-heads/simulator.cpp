// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "simulator.hpp"

#include <iostream>

namespace methhead
{
    Simulator::Simulator(const Mode mode)
        : m_isModeNormal(mode == Mode::Normal)
        , m_willStop(false)
        , m_videoMode(1600u, 1200u, sf::VideoMode::getDesktopMode().bitsPerPixel)
        , m_window()
        , m_random()
        , m_settings()
        , m_soundPlayer()
        , m_animationPlayer()
        , m_displayVars(sf::Vector2u(m_videoMode.width, m_videoMode.height))
        , m_actors()
        , m_frameClock()
        , m_simTimeMultiplier(0.3f)
        , m_spinCount(0)
        , m_framesPerSecondMax(0)
        , m_statusClock()
        , m_framesSinceStatusCount(0)
        , m_statusIntervalSec(2.0f)
        , m_context(m_random, m_actors, m_pickups, m_displayVars.constants(), m_settings)
    {
        m_settings.printAll();

        if (m_isModeNormal)
        {
            m_window.create(m_videoMode, "Meth Heads", sf::Style::Default);
        }

        spawnInitialPieces();

        m_animationPlayer.load({ "orb", "puff" });
        m_soundPlayer.load({ "coins-1", "coins-2" });
    }

    void Simulator::run()
    {
        while (!m_willStop)
        {
            if (m_isModeNormal)
            {
                handleEvents();
                update(getSimFrameTimeElapsed());
                draw();
            }
            else
            {
                update(1.0f);
            }

            printStatus();
        }
    }

    float Simulator::getSimFrameTimeElapsed()
    {
        float actualElapsedSec(m_frameClock.getElapsedTime().asSeconds());
        while (actualElapsedSec < std::numeric_limits<float>::epsilon())
        {
            actualElapsedSec = m_frameClock.getElapsedTime().asSeconds();
            ++m_spinCount;
        }

        m_frameClock.restart();
        return (actualElapsedSec * m_simTimeMultiplier);
    }

    void Simulator::spawnInitialPieces()
    {
        const std::size_t initialLootCount(5);
        for (std::size_t i(0); i < initialLootCount; ++i)
        {
            spawnLoot();
        }

        spawnMethHead(Motivation::lazy);
        spawnMethHead(Motivation::greedy);
    }

    BoardPos_t Simulator::findRandomFreeBoardPos() const
    {
        assert(isBoardFull() == false);

        std::vector<BoardPos_t> positions;

        for (int vert(0); vert < m_displayVars.constants().cell_countsI.y; ++vert)
        {
            for (int horiz(0); horiz < m_displayVars.constants().cell_countsI.x; ++horiz)
            {
                const BoardPos_t pos(horiz, vert);

                if (!m_context.isAnythingAt(pos))
                {
                    positions.push_back(pos);
                }
            }
        }

        return m_random.from(positions);
    }

    void Simulator::spawnMethHead(const Motivation motive)
    {
        assert(Motivation::none != motive);

        if (isBoardFull())
        {
            return;
        }

        const BoardPos_t freeBoardPos(findRandomFreeBoardPos());

        if (motive == Motivation::lazy)
        {
            m_actors.push_back(std::make_unique<Lazy>(freeBoardPos));
        }
        else
        {
            m_actors.push_back(std::make_unique<Greedy>(freeBoardPos));
        }
    }

    void Simulator::spawnLoot()
    {
        if (isBoardFull())
        {
            return;
        }

        const BoardPos_t freeBoardPos(findRandomFreeBoardPos());

        const int lootAmount(m_random.fromTo(1, 100));

        m_pickups.push_back(std::make_unique<Loot>(freeBoardPos, lootAmount));
    }

    void Simulator::handleEvents()
    {
        sf::Event event;
        while (!m_willStop && m_window.pollEvent(event))
        {
            handleEvent(event);
        }
    }

    void Simulator::handleEvent(const sf::Event & event)
    {
        if (event.type == sf::Event::Closed)
        {
            m_willStop = true;
            return;
        }

        // at this point only keypresses are valid/handled
        if (sf::Event::KeyPressed != event.type)
        {
            return;
        }

        if ((sf::Keyboard::Escape == event.key.code) || (sf::Keyboard::W == event.key.code))
        {
            m_willStop = true;
        }
        else if (sf::Keyboard::Up == event.key.code)
        {
            m_soundPlayer.volumeUp();
        }
        else if (sf::Keyboard::Down == event.key.code)
        {
            m_soundPlayer.volumeDown();
        }
        else if (sf::Keyboard::Left == event.key.code)
        {
            if (event.key.shift)
            {
                scaleTimeMultiplier(0.5f);
            }
            else
            {
                for (const IActorUPtr_t & actor : m_actors)
                {
                    const float newWaitTimeSec(actor->timeBetweenMovesSec() * 1.1f);
                    actor->timeBetweenMovesSec(newWaitTimeSec);
                }
            }
        }
        else if (sf::Keyboard::Right == event.key.code)
        {
            if (event.key.shift)
            {
                scaleTimeMultiplier(1.25f);
            }
            else
            {
                for (const IActorUPtr_t & actor : m_actors)
                {
                    const float newWaitTimeSec(actor->timeBetweenMovesSec() * 0.9f);
                    actor->timeBetweenMovesSec(newWaitTimeSec);
                }
            }
        }
        else if (sf::Keyboard::M == event.key.code)
        {
            if (event.key.shift)
            {
                if (!m_actors.empty())
                {
                    m_actors.pop_back();
                }

                if (!m_actors.empty())
                {
                    m_actors.pop_back();
                }
            }
            else
            {
                spawnMethHead(Motivation::lazy);
                spawnMethHead(Motivation::greedy);
            }
        }
        else if (sf::Keyboard::L == event.key.code)
        {
            if (event.key.shift)
            {
                if (!m_pickups.empty())
                {
                    m_pickups.pop_back();
                }
            }
            else
            {
                spawnLoot();
            }
        }
        else if (sf::Keyboard::S == event.key.code)
        {
            if (event.key.shift)
            {
                if (!m_actors.empty())
                {
                    m_actors.pop_back();
                }

                if (!m_actors.empty())
                {
                    m_actors.pop_back();
                }

                if (!m_pickups.empty())
                {
                    m_pickups.pop_back();
                }

                if (!m_pickups.empty())
                {
                    m_pickups.pop_back();
                }
            }
            else
            {
                spawnMethHead(Motivation::lazy);
                spawnMethHead(Motivation::greedy);
                spawnLoot();
            }
        }
        else if (sf::Keyboard::R == event.key.code)
        {
            std::cout << "Resetting everything..." << std::endl;

            m_soundPlayer.stopAll();
            m_animationPlayer.stopAll();

            m_actors.clear();
            m_pickups.clear();

            m_displayVars.setFps(0);
            m_frameClock.restart();
            m_statusClock.restart();

            m_spinCount = 0;
            m_simTimeMultiplier = 1.0f;
            m_framesSinceStatusCount = 0;

            spawnInitialPieces();
        }
        else
        {
            m_settings.flip(event.key.code);
        }
    }

    void Simulator::scaleTimeMultiplier(const float multiplyBy)
    {
        m_simTimeMultiplier *= multiplyBy;
        m_simTimeMultiplier = std::clamp(m_simTimeMultiplier, 0.01f, 100000.0f);
    }

    void Simulator::update(const float elapsedSec)
    {
        if (m_isModeNormal && m_settings.query(Settings::SpecialEffects))
        {
            m_animationPlayer.update(elapsedSec);
        }

        for (IActorUPtr_t & actor : m_actors)
        {
            const BoardPos_t posBefore(actor->boardPos());
            actor->update(elapsedSec, m_context);
            const BoardPos_t posAfter(actor->boardPos());

            if (posBefore != posAfter)
            {
                handleActorPickup(*actor);
            }
        }
    }

    void Simulator::handleActorPickup(IActor & actor)
    {
        const auto foundIter = std::find_if(
            std::begin(m_pickups), std::end(m_pickups), [&](const IPickupUPtr_t & pickup) {
                return (pickup->boardPos() == actor.boardPos());
            });

        if (foundIter == std::end(m_pickups))
        {
            return;
        }

        if (m_isModeNormal && m_settings.query(Settings::SpecialEffects))
        {
            const sf::FloatRect actorWindowBounds(
                m_displayVars.constants().boardPosToWindowRect(actor.boardPos()));

            const sf::FloatRect animWindowBounds(scaleRectInPlaceCopy(actorWindowBounds, 4.0f));

            if (actor.motivation() == Motivation::lazy)
            {
                m_soundPlayer.play("coins-1", m_random);

                m_animationPlayer.play(
                    m_random, "puff", animWindowBounds, (actor.timeBetweenMovesSec() * 2.5f));
            }
            else
            {
                m_soundPlayer.play("coins-2", m_random);

                m_animationPlayer.play(
                    m_random, "orb", animWindowBounds, (actor.timeBetweenMovesSec() * 2.5f));
            }
        }

        (*foundIter)->changeActor(actor);
        m_pickups.erase(foundIter);

        spawnLoot();
    }

    void Simulator::printStatus()
    {
        ++m_framesSinceStatusCount;

        const float elapsedStatusTimeSec{ m_statusClock.getElapsedTime().asSeconds() };
        if (elapsedStatusTimeSec < m_statusIntervalSec)
        {
            return;
        }

        const Scores scores(calcScores());

        const float fpsReal(static_cast<float>(m_framesSinceStatusCount) / elapsedStatusTimeSec);
        const std::size_t fps(static_cast<std::size_t>(fpsReal));

        m_statusClock.restart();
        m_framesSinceStatusCount = 0;

        if (m_framesPerSecondMax < fps)
        {
            m_framesPerSecondMax = fps;
        }

        if (m_isModeNormal)
        {
            m_displayVars.setFps(fps);
            m_displayVars.updateScoreBars(scores.lazy, scores.greedy);
        }

        auto countOffBoard = [this](const auto & container) {
            return std::count_if(
                std::begin(container), std::end(container), [&](const auto & uptr) {
                    const BoardPos_t boardPos{ uptr->boardPos() };
                    return (
                        (boardPos.x < 0) ||
                        (boardPos.x >= m_displayVars.constants().cell_countsI.x) ||
                        (boardPos.y < 0) ||
                        (boardPos.y >= m_displayVars.constants().cell_countsI.y));
                });
        };

        std::cout << "fps=" << fps << "/" << m_framesPerSecondMax;
        std::cout << ", actors=" << m_actors.size() << "/" << countOffBoard(m_actors);
        std::cout << ", pickups=" << m_pickups.size() << "/" << countOffBoard(m_pickups);

        std::cout << ", free="
                  << ((m_displayVars.constants().cell_counts.x *
                       m_displayVars.constants().cell_counts.y) -
                      (m_actors.size() + m_pickups.size()));

        if ((0 == scores.greedy) || (0 == scores.lazy))
        {
            std::cout << std::endl;
            return;
        }

        if (scores.greedy == scores.lazy)
        {
            std::cout << ", TIED" << std::endl;
            return;
        }

        if (scores.greedy > scores.lazy)
        {
            std::cout << ", GREEDY winning by ";
        }
        else
        {
            std::cout << ", LAZY winning by ";
        }

        const int lowScore(std::min(scores.lazy, scores.greedy));
        const float highScoreF(static_cast<float>(std::max(scores.lazy, scores.greedy)));
        const float lowScoreF(static_cast<float>(lowScore));
        const float winRatio(1.0f - (lowScoreF / highScoreF));
        const std::size_t winPercent(static_cast<std::size_t>(winRatio * 100.0f));

        std::cout << winPercent << "%" << std::endl;
    }

    void Simulator::draw()
    {
        const sf::RenderStates renderStates;

        m_window.clear(sf::Color(64, 64, 64));

        m_displayVars.draw(m_context, m_window, renderStates);

        if (m_isModeNormal && m_settings.query(Settings::SpecialEffects))
        {
            m_animationPlayer.draw(m_window, renderStates);
        }

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