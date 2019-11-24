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
        , m_soundPlayer()
        , m_animationPlayer()
        , m_displayVars(sf::Vector2u(m_videoMode.width, m_videoMode.height))
        , m_actors()
        , m_frameClock()
        , m_simTimeMultiplier(1.0f)
        , m_statusClock()
        , m_framesSincePrevStatusCount(0)
        , m_framesSincePrevStatusCountMax(0)
        , m_statusIntervalSec(1.0f)
        , m_actorContext(m_random, m_pickups, m_actors, m_displayVars.constants())
        , m_spinCount(0)
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
                update(0.0f);
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
        ++m_framesSincePrevStatusCount;

        const float elapsedStatusTimeSec{ m_statusClock.getElapsedTime().asSeconds() };

        if (elapsedStatusTimeSec > m_statusIntervalSec)
        {
            if (m_framesSincePrevStatusCountMax < m_framesSincePrevStatusCount)
            {
                m_framesSincePrevStatusCountMax = m_framesSincePrevStatusCount;
            }

            if (Mode::Normal == m_mode)
            {
                const float fps(
                    static_cast<float>(m_framesSincePrevStatusCount) / elapsedStatusTimeSec);

                m_displayVars.setFps(static_cast<std::size_t>(fps));
            }
            else
            {
                printStatus();
            }

            m_framesSincePrevStatusCount = 0;
            m_statusClock.restart();

            if (m_spinCount > 0)
            {
                std::cout << "spin_count=" << m_spinCount << std::endl;
            }
        }
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

        const sf::FloatRect freeWindowRect(
            m_displayVars.constants().boardPosToWindowRect(freeBoardPos));

        // std::cout << "spawning " << ((Motivation::lazy == motive) ? "Lazy" : "Greedy")
        //          << " methhead #" << m_pickups.size() << ", at pos=" << freeBoardPos
        //          << ", inside rect=" << freeWindowRect << std::endl;

        assert(isRealClose(freeWindowRect.width, m_displayVars.constants().cell_size.x, 2.0f));
        assert(isRealClose(freeWindowRect.height, m_displayVars.constants().cell_size.y, 2.0f));

        if (motive == Motivation::lazy)
        {
            IActorUPtr_t lazyUPtr(
                std::make_unique<Lazy>("image/head-1.png", freeBoardPos, freeWindowRect));

            m_actors.push_back(std::move(lazyUPtr));
        }
        else
        {
            IActorUPtr_t greedyUPtr(
                std::make_unique<Greedy>("image/head-2.png", freeBoardPos, freeWindowRect));

            m_actors.push_back(std::move(greedyUPtr));
        }
    }

    void Simulator::spawnLoot()
    {
        const BoardPos_t freeBoardPos(findRandomFreeBoardPos());

        const sf::FloatRect freeWindowRect(
            m_displayVars.constants().boardPosToWindowRect(freeBoardPos));

        // std::cout << "spawning loot #" << m_pickups.size() << ", at pos=" << freeBoardPos
        //          << ", inside rect=" << freeWindowRect;

        assert(isRealClose(freeWindowRect.width, m_displayVars.constants().cell_size.x, 2.0f));
        assert(isRealClose(freeWindowRect.height, m_displayVars.constants().cell_size.y, 2.0f));

        IPickupUPtr_t pickupUPtr(std::make_unique<Loot>(
            "image/loot.png",
            freeBoardPos,
            freeWindowRect,
            m_random.fromTo(1, 100),
            m_displayVars.constants()));

        m_pickups.push_back(std::move(pickupUPtr));

        // std::cout << ", of value=" << m_pickups.back()->value() << std::endl;
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
        else if (sf::Keyboard::Left == event.key.code)
        {
            scaleTimeMultiplier(0.5f);
        }
        else if (sf::Keyboard::Right == event.key.code)
        {
            scaleTimeMultiplier(1.5f);
        }
        else if (sf::Keyboard::S == event.key.code)
        {
            spawnMethHead(Motivation::lazy);
            spawnMethHead(Motivation::greedy);
        }
        else if (sf::Keyboard::K == event.key.code)
        {
            if (!m_actors.empty())
            {
                m_actors.pop_back();
            }

            if (!m_pickups.empty())
            {
                m_pickups.pop_back();
            }
        }
        else if (sf::Keyboard::R == event.key.code)
        {
            m_soundPlayer.reset();
            m_animationPlayer.reset();
            m_displayVars.setFps(0);
            m_actors.clear();
            m_pickups.clear();
            m_frameClock.restart();
            m_statusClock.restart();
            m_framesSincePrevStatusCount = 0;
            m_framesSincePrevStatusCountMax = 0;
            m_spinCount = 0;
            m_simTimeMultiplier = 1.0f;
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
            if (!actor->update(elapsedSec, m_actorContext))
            {
                continue;
            }

            if (m_pickups.empty())
            {
                continue;
            }

            std::size_t pickupIndex(m_pickups.size() + 1);
            for (std::size_t i(0); i < m_pickups.size(); ++i)
            {
                if (actor->boardPos() == m_pickups.at(i)->boardPos())
                {
                    pickupIndex = i;
                    break;
                }
            }

            if (pickupIndex >= m_pickups.size())
            {
                continue;
            }

            if (Mode::Normal == m_mode)
            {
                const sf::FloatRect actorWindowBoundsOrig(
                    m_displayVars.constants().boardPosToWindowRect(actor->boardPos()));

                const sf::FloatRect actorWindowBoundsBigger(
                    scaleRectInPlaceCopy(actorWindowBoundsOrig, 6.0f));

                if (actor->motivation() == Motivation::lazy)
                {
                    m_soundPlayer.play("coins-1", m_random);
                    m_animationPlayer.play(m_random, "spark-ball", actorWindowBoundsBigger);
                }
                else
                {
                    m_soundPlayer.play("coins-2", m_random);
                    m_animationPlayer.play(m_random, "sparkle-burst", actorWindowBoundsBigger);
                }
            }

            m_pickups.at(pickupIndex)->changeActor(*actor);
            m_pickups.erase(std::begin(m_pickups) + static_cast<std::ptrdiff_t>(pickupIndex));

            spawnLoot();
        }

        const Scores scores(calcScores());
        m_displayVars.update(elapsedSec, scores.lazy, scores.greedy);
    }

    void Simulator::printStatus()
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

        std::cout << "fps=" << m_framesSincePrevStatusCount;
        std::cout << " (max=" << m_framesSincePrevStatusCountMax << ")";
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

        const sf::RenderStates renderStates;

        m_displayVars.draw(m_window, renderStates);

        for (IPickupUPtr_t & uptr : m_pickups)
        {
            uptr->draw(m_window, renderStates);
        }

        // TODO Nel:  Shoul we partition /sort by image first?
        for (IActorUPtr_t & uptr : m_actors)
        {
            uptr->draw(m_window, renderStates);
        }

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