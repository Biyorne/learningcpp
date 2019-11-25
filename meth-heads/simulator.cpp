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
        , m_videoMode(1600u, 1200u, sf::VideoMode::getDesktopMode().bitsPerPixel)
        , m_window()
        , m_random()
        , m_settings()
        , m_soundPlayer(Mode::Normal == mode)
        , m_animationPlayer(Mode::Normal == mode)
        , m_displayVars(sf::Vector2u(m_videoMode.width, m_videoMode.height))
        , m_actors()
        , m_frameClock()
        , m_simTimeMultiplier(1.0f)
        , m_spinCount(0)
        , m_framesPerSecondMax(0)
        , m_statusClock()
        , m_framesSinceStatusCount(0)
        , m_statusIntervalSec(1.0f)
        , m_actorContext(m_random, m_pickups, m_actors, m_displayVars.constants(), m_settings)
    {
        if (Mode::Normal == m_mode)
        {
            m_window.create(m_videoMode, "Meth Heads", sf::Style::Default);
        }

        spawnInitialPieces();
    }

    void Simulator::run()
    {
        while (willKeepRunning())
        {
            if (Mode::Normal == m_mode)
            {
                handleEvents();

                float actualElapsedSec(m_frameClock.getElapsedTime().asSeconds());
                while (actualElapsedSec < std::numeric_limits<float>::epsilon())
                {
                    actualElapsedSec = m_frameClock.getElapsedTime().asSeconds();
                    ++m_spinCount;
                }

                m_frameClock.restart();
                const float simElapsedSec(actualElapsedSec * m_simTimeMultiplier);

                update(simElapsedSec);

                draw();
            }
            else
            {
                update(1.0f);
            }

            handleStatus();
        }
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

    void Simulator::handleStatus()
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

        if (m_framesPerSecondMax < fps)
        {
            m_framesPerSecondMax = fps;
        }

        if (Mode::Normal == m_mode)
        {
            m_displayVars.setFps(fps);
            m_displayVars.updateScoreBars(scores.lazy, scores.greedy);
        }

        printStatus(fps, scores);

        m_framesSinceStatusCount = 0;
        m_statusClock.restart();
    }

    BoardPos_t Simulator::findRandomFreeBoardPos() const
    {
        // create a vector of all possible board positions that includes the ring of positions
        // outside where all actors and pickups are, which ensures there is always a valid spawn
        // position -even if it is off of the map

        // TODO what's wrong with this code?

        int leftMostPos(0);
        int rightMostPos(static_cast<int>(m_displayVars.constants().horiz_cell_count));
        int highestPos(0);
        int lowestPos(static_cast<int>(m_displayVars.constants().vert_cell_count));

        for (const IActorUPtr_t & actor : m_actors)
        {
            const sf::Vector2i boardPos(actor->boardPos());

            if (boardPos.x < leftMostPos)
            {
                leftMostPos = boardPos.x;
            }

            if (boardPos.x > rightMostPos)
            {
                rightMostPos = boardPos.x;
            }

            if (boardPos.y < highestPos)
            {
                highestPos = boardPos.y;
            }

            if (boardPos.y > lowestPos)
            {
                lowestPos = boardPos.y;
            }
        }

        for (const IPickupUPtr_t & pickup : m_pickups)
        {
            const sf::Vector2i boardPos(pickup->boardPos());

            if (boardPos.x < leftMostPos)
            {
                leftMostPos = boardPos.x;
            }

            if (boardPos.x > rightMostPos)
            {
                rightMostPos = boardPos.x;
            }

            if (boardPos.y < highestPos)
            {
                highestPos = boardPos.y;
            }

            if (boardPos.y > lowestPos)
            {
                lowestPos = boardPos.y;
            }
        }

        assert(leftMostPos <= rightMostPos);
        assert(highestPos <= lowestPos);

        std::set<BoardPos_t> freeBoardPositions;
        for (int vert(highestPos); vert < lowestPos; ++vert)
        {
            for (int horiz(leftMostPos); horiz < rightMostPos; ++horiz)
            {
                freeBoardPositions.insert(sf::Vector2i(horiz, vert));
            }
        }

        for (const IActorUPtr_t & actor : m_actors)
        {
            freeBoardPositions.erase(actor->boardPos());
        }

        for (const IPickupUPtr_t & pickup : m_pickups)
        {
            freeBoardPositions.erase(pickup->boardPos());
        }

        // const sf::Vector2i rangePos(leftMostPos, highestPos);
        // const sf::Vector2i rangeSize((rightMostPos - leftMostPos), (lowestPos - highestPos));
        // const sf::IntRect rangeRect(rangePos, rangeSize);
        // const int rangeCellCount(std::abs(rangeSize.x * rangeSize.y));
        // std::cout << "Looking for free within range=" << rangeRect << ", of size=" << rangeSize
        //          << ", which has range_cell_count=" << rangeCellCount
        //          << ", and after removeing pickup_count=" << m_pickups.size()
        //          << ", and actor_count=" << m_actors.size()
        //          << ", there were possible_cell_board_pos_count=" << freeBoardPositions.size()
        //          << std::endl;

        if (freeBoardPositions.empty())
        {
            --leftMostPos;
            rightMostPos += 2;
            --highestPos;
            lowestPos += 2;

            assert(leftMostPos < rightMostPos);
            assert(highestPos < lowestPos);

            for (int vert(highestPos); vert < lowestPos; vert = lowestPos)
            {
                for (int horiz(leftMostPos); horiz < rightMostPos; horiz = rightMostPos)
                {
                    freeBoardPositions.insert(sf::Vector2i(horiz, vert));
                }
            }

            const sf::Vector2i rangePos(leftMostPos, highestPos);
            const sf::Vector2i rangeSize((rightMostPos - leftMostPos), (lowestPos - highestPos));
            const sf::IntRect rangeRect(rangePos, rangeSize);
            const int rangeCellCount(std::abs(rangeSize.x * rangeSize.y));
            std::cout << " *** REDO *** Looking for free within range=" << rangeRect
                      << ", of size=" << rangeSize
                      << ", which has range_cell_count=" << rangeCellCount
                      << ", and after removeing pickup_count=" << m_pickups.size()
                      << ", and actor_count=" << m_actors.size()
                      << ", there were possible_cell_board_pos_count=" << freeBoardPositions.size()
                      << std::endl;
        }

        assert(!freeBoardPositions.empty());

        // for (const BoardPos_t & pos : freeBoardPositions)
        //{
        //    assert(!m_actorContext.isEitherAtBoardPos(pos));
        //}

        return m_random.from(freeBoardPositions);
    }

    void Simulator::spawnMethHead(const Motivation motive)
    {
        assert(Motivation::none != motive);

        const BoardPos_t freeBoardPos(findRandomFreeBoardPos());

        if (motive == Motivation::lazy)
        {
            IActorUPtr_t lazyUPtr(std::make_unique<Lazy>(freeBoardPos));
            m_actors.push_back(std::move(lazyUPtr));
        }
        else
        {
            IActorUPtr_t greedyUPtr(std::make_unique<Greedy>(freeBoardPos));
            m_actors.push_back(std::move(greedyUPtr));
        }
    }

    void Simulator::spawnLoot()
    {
        const BoardPos_t freeBoardPos(findRandomFreeBoardPos());
        const int lootAmount(m_random.fromTo(1, 100));

        IPickupUPtr_t pickupUPtr(std::make_unique<Loot>(freeBoardPos, lootAmount));
        m_pickups.push_back(std::move(pickupUPtr));
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

        if (sf::Event::KeyPressed != event.type)
        {
            return;
        }

        if (sf::Keyboard::Up == event.key.code)
        {
            m_soundPlayer.volumeUp();
        }
        else if (sf::Keyboard::Down == event.key.code)
        {
            m_soundPlayer.volumeDown();
        }
        else if (sf::Keyboard::Num1 == event.key.code)
        {
            m_settings.will_draw_board_with_verts = !m_settings.will_draw_board_with_verts;

            std::cout << "Settings change:\t will_draw_board_with_verts=" << std::boolalpha
                      << m_settings.will_draw_board_with_verts << std::endl;
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

            m_settings = Settings();

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
        else if (sf::Keyboard::Escape == event.key.code)
        {
            m_window.close();
        }
    }

    void Simulator::scaleTimeMultiplier(const float multiplyBy)
    {
        m_simTimeMultiplier *= multiplyBy;
        m_simTimeMultiplier = std::clamp(m_simTimeMultiplier, 0.1f, 100000.0f);
    }

    void Simulator::update(const float elapsedSec)
    {
        if (Mode::Normal == m_mode)
        {
            m_animationPlayer.update(elapsedSec);
        }

        for (IActorUPtr_t & actor : m_actors)
        {
            const BoardPos_t posBefore(actor->boardPos());
            actor->update(elapsedSec, m_actorContext);
            const BoardPos_t posAfter(actor->boardPos());

            if (posBefore == posAfter)
            {
                continue;
            }

            const auto foundIter = std::find_if(
                std::begin(m_pickups),
                std::end(m_pickups),
                [&posAfter](const IPickupUPtr_t & pickup) {
                    return (pickup->boardPos() == posAfter);
                });

            if (foundIter == std::end(m_pickups))
            {
                continue;
            }

            if (Mode::Normal == m_mode)
            {
                const sf::FloatRect actorWindowBounds(
                    m_displayVars.constants().boardPosToWindowRect(actor->boardPos()));

                const sf::FloatRect animWindowBounds(scaleRectInPlaceCopy(actorWindowBounds, 4.0f));

                if (actor->motivation() == Motivation::lazy)
                {
                    m_soundPlayer.play("coins-1", m_random);
                    m_animationPlayer.play(m_random, "spark-ball", animWindowBounds);
                }
                else
                {
                    m_soundPlayer.play("coins-2", m_random);
                    m_animationPlayer.play(m_random, "sparkle-burst", animWindowBounds);
                }
            }

            (*foundIter)->changeActor(*actor);
            m_pickups.erase(foundIter);

            spawnLoot();
        }
    }

    void Simulator::printStatus(const std::size_t fps, const Scores & scores)
    {
        std::cout << "fps=" << fps;
        std::cout << " (max=" << m_framesPerSecondMax << ")";
        std::cout << ",   lazy=" << scores.lazy;
        std::cout << ",   greedy=" << scores.greedy;

        if ((0 == scores.greedy) || (0 == scores.lazy))
        {
            std::cout << std::endl;
            return;
        }

        std::cout << "...";

        if (scores.greedy == scores.lazy)
        {
            std::cout << "TIED" << std::endl;
            return;
        }

        if (scores.greedy > scores.lazy)
        {
            std::cout << "GREEDY winning by ";
        }
        else
        {
            std::cout << "LAZY winning by ";
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
        m_window.clear(sf::Color(64, 64, 64));

        const sf::RenderStates renderStates;

        m_displayVars.draw(
            m_settings.will_draw_board_with_verts, m_actors, m_pickups, m_window, renderStates);

        if (Mode::Normal == m_mode)
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