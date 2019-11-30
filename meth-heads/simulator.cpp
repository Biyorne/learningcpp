// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "simulator.hpp"

#include <iostream>
#include <sstream>

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
        , m_positions(m_displayVars.constants(), m_random)
        , m_actors()
        , m_frameClock()
        , m_framesPerSecondMax(0)
        , m_statusClock()
        , m_framesSinceStatusCount(0)
        , m_statusIntervalSec(1.0f)
        , m_statusCount(0)
        , m_context(m_random, m_actors, m_pickups, m_displayVars.constants(), m_settings)
    {
        m_settings.printAll();

        if (m_isModeNormal)
        {
            m_window.create(m_videoMode, "Meth Heads", sf::Style::Default);

            m_animationPlayer.load({ "orb", "puff" });
            m_soundPlayer.load({ "coins-1", "coins-2" });
        }

        spawnInitialPieces();
    }

    void Simulator::spawnInitialPieces()
    {
        spawnLoot(5);
        spawnMethHead(Motivation::lazy, 1);
        spawnMethHead(Motivation::greedy, 1);
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
                update(0.1f);
            }

            printStatus();
        }

        m_window.close();
    }

    float Simulator::getSimFrameTimeElapsed()
    {
        float actualElapsedSec{ 0.0f };
        while (actualElapsedSec < std::numeric_limits<float>::epsilon())
        {
            actualElapsedSec = m_frameClock.getElapsedTime().asSeconds();
        }

        m_frameClock.restart();

        return actualElapsedSec;
    }

    void Simulator::spawnMethHead(const Motivation motive, const std::size_t count)
    {
        assert(Motivation::none != motive);

        for (std::size_t i(0); i < count; ++i)
        {
            const auto freeBoardPosOpt(m_positions.findRandomFreeAndOccupy());
            if (!freeBoardPosOpt)
            {
                break;
            }

            assert(m_context.isAnythingAt(freeBoardPosOpt.value()) == false);
            assert(m_positions.isFree(freeBoardPosOpt.value()) == false);

            if (motive == Motivation::lazy)
            {
                m_actors.push_back(std::make_unique<Lazy>(freeBoardPosOpt.value()));
            }
            else
            {
                m_actors.push_back(std::make_unique<Greedy>(freeBoardPosOpt.value()));
            }

            assert(m_context.isActorAt(freeBoardPosOpt.value()) == true);
            assert(m_context.isPickupAt(freeBoardPosOpt.value()) == false);
            assert(m_positions.isFree(freeBoardPosOpt.value()) == false);
        }
    }

    void Simulator::spawnLoot(const std::size_t count)
    {
        for (std::size_t i(0); i < count; ++i)
        {
            const auto freeBoardPosOpt(m_positions.findRandomFreeAndOccupy());
            if (!freeBoardPosOpt)
            {
                break;
            }

            assert(m_context.isAnythingAt(freeBoardPosOpt.value()) == false);
            assert(m_positions.isFree(freeBoardPosOpt.value()) == false);

            const int lootAmount{ m_random.fromTo(1, 100) };

            m_pickups.push_back(std::make_unique<Loot>(freeBoardPosOpt.value(), lootAmount));

            assert(m_context.isActorAt(freeBoardPosOpt.value()) == false);
            assert(m_context.isPickupAt(freeBoardPosOpt.value()) == true);
            assert(m_positions.isFree(freeBoardPosOpt.value()) == false);
        }
    }

    void Simulator::killMethHead(const std::size_t count)
    {
        for (std::size_t i(0); i < count; ++i)
        {
            if (m_actors.empty())
            {
                break;
            }

            m_positions.free(m_actors.back()->boardPos());
            m_actors.pop_back();
        }
    }

    void Simulator::killLoot(const std::size_t count)
    {
        for (std::size_t i(0); i < count; ++i)
        {
            if (m_pickups.empty())
            {
                break;
            }

            m_positions.free(m_pickups.back()->boardPos());
            m_pickups.pop_back();
        }
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
            for (const IActorUPtr_t & actor : m_actors)
            {
                const float newWaitTimeSec(actor->timeBetweenMovesSec() * 1.1f);
                actor->timeBetweenMovesSec(newWaitTimeSec);
            }
        }
        else if (sf::Keyboard::Right == event.key.code)
        {
            for (const IActorUPtr_t & actor : m_actors)
            {
                const float newWaitTimeSec(actor->timeBetweenMovesSec() * 0.9f);
                actor->timeBetweenMovesSec(newWaitTimeSec);
            }
        }
        else if (sf::Keyboard::M == event.key.code)
        {
            if (event.key.shift)
            {
                killMethHead(2);
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
                killLoot(10);
            }
            else
            {
                spawnLoot(10);
            }
        }
        else if (sf::Keyboard::S == event.key.code)
        {
            if (event.key.shift)
            {
                killMethHead(2);
                killLoot(2);
            }
            else
            {
                spawnMethHead(Motivation::lazy);
                spawnMethHead(Motivation::greedy);
                spawnLoot(2);
            }
        }
        else if (sf::Keyboard::R == event.key.code)
        {
            std::cout << "Reset to initial state." << std::endl;

            m_settings = Settings();

            m_positions.reset();

            m_soundPlayer.stopAll();
            m_animationPlayer.stopAll();

            m_displayVars.setFps(0);
            m_displayVars.updateScoreBars(0, 0);

            m_actors.clear();
            m_pickups.clear();
            spawnInitialPieces();

            m_framesSinceStatusCount = 0;

            m_frameClock.restart();
            m_statusClock.restart();
        }
        else
        {
            m_settings.flip(event.key.code);
        }
    }

    void Simulator::update(const float elapsedSec)
    {
        for (IActorUPtr_t & actor : m_actors)
        {
            const BoardPos_t posBefore(actor->boardPos());
            actor->update(elapsedSec, m_context);
            const BoardPos_t posAfter(actor->boardPos());

            if (posBefore != posAfter)
            {
                assert(m_context.isActorAt(posBefore) == false);
                assert(m_context.isPickupAt(posBefore) == false);
                assert(m_positions.isFree(posBefore) == false);
                //
                assert(m_context.isActorAt(posAfter) == true);

                if (m_context.isPickupAt(posAfter))
                {
                    assert(m_positions.isFree(posAfter) == false);
                }
                else
                {
                    assert(m_positions.isFree(posAfter) == true);
                }

                m_positions.move(posBefore, posAfter);

                assert(m_context.isActorAt(posBefore) == false);
                assert(m_context.isPickupAt(posBefore) == false);
                assert(m_positions.isFree(posBefore) == true);
                //
                assert(m_context.isActorAt(posAfter) == true);
                assert(m_positions.isFree(posAfter) == false);

                handleActorPickingUpSometing(*actor);

                assert(m_context.isActorAt(posBefore) == false);
            }
        }

        if (m_isModeNormal && m_settings.query(Settings::SpecialEffects))
        {
            m_animationPlayer.update(elapsedSec);
        }
    }

    void Simulator::handleActorPickingUpSometing(IActor & actor)
    {
        assert(m_context.isActorAt(actor.boardPos()) == true);
        assert(m_positions.isFree(actor.boardPos()) == false);

        const auto foundIter = std::find_if(
            std::begin(m_pickups), std::end(m_pickups), [&](const IPickupUPtr_t & pickup) {
                return (pickup->boardPos() == actor.boardPos());
            });

        if (foundIter == std::end(m_pickups))
        {
            return;
        }

        assert(m_context.isPickupAt(actor.boardPos()) == true);

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

        // no need to m_positions.free(pickup) because the methhead that picked it up is there

        assert(m_context.isActorAt(actor.boardPos()) == true);
        assert(m_context.isPickupAt(actor.boardPos()) == true);
        assert(m_positions.isFree(actor.boardPos()) == false);

        m_pickups.erase(foundIter);

        assert(m_context.isActorAt(actor.boardPos()) == true);
        assert(m_context.isPickupAt(actor.boardPos()) == false);
        assert(m_positions.isFree(actor.boardPos()) == false);

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

        assert(fps != m_framesPerSecondMax);

        std::cout << m_statusCount++ << ',';

#ifdef WIN32
        std::cout << "Win,";
#else
        std::cout << "Mac,";
#endif

#ifdef NDEBUG
        std::cout << "Release,";
#else
        std::cout << "Debug,";
#endif

        if (m_isModeNormal)
        {
            std::cout << "Normal,";
        }
        else
        {
            std::cout << "SpeedTest,";
        }

        if (m_settings.query(Settings::DrawBoardWithVerts))
        {
            std::cout << "DrawVerts,";
        }
        else
        {
            std::cout << "DrawRectangles,";
        }

        std::cout << fps << ',' << m_framesPerSecondMax << ',';
        std::cout << m_actors.size() << ',' << m_pickups.size() << ',';
        std::cout << scores.lazy << ',' << scores.greedy << std::endl;
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