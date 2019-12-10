// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "simulator.hpp"

#include "scoped-board-pos-handler.hpp"

#include <algorithm>
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
        , m_actors()
        , m_frameClock()
        , m_framesPerSecondMax(0)
        , m_simTimeMult(1.0)
        , m_statusClock()
        , m_framesSinceStatusCount(0)
        , m_statusIntervalSec(1.0f)
        , m_statusCount(0)
        , m_context(m_random, m_actors, m_pickups, m_displayVars.constants(), m_settings)
    {
        m_actors.reserve(1000);
        m_pickups.reserve(1000);

        m_settings.printAll();

        if (m_isModeNormal)
        {
            m_window.create(m_videoMode, "Meth Heads", sf::Style::Default);

            if (m_settings.query(Settings::SpecialEffects))
            {
                m_animationPlayer.load({ "orb", "puff" });
                m_soundPlayer.load({ "coins-1", "coins-2" });
            }
        }

        spawnInitialPieces();
    }

    void Simulator::spawnInitialPieces()
    {
        spawnLoot(5);
        spawnMethHead(Motivation::lazy, 1);
        spawnMethHead(Motivation::greedy, 1);
    }

    void Simulator::reset()
    {
        std::cout << "Reset to initial state." << std::endl;

        m_soundPlayer.stopAll();
        m_animationPlayer.stopAll();

        m_settings = Settings();

        m_statusCount = 0;
        m_simTimeMult = 1.0f;
        m_framesPerSecondMax = 0;
        m_framesSinceStatusCount = 0;

        m_frameClock.restart();
        m_statusClock.restart();

        m_actors.clear();
        m_pickups.clear();

        // this must happen AFTER ALL pieces(actors and pickups) have destructed
        ScopedBoardPosHandler::reset(m_context);

        m_displayVars.updatePerStatus(0, 0, 0);
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

        reset();
        m_window.close();
    }

    float Simulator::getSimFrameTimeElapsed()
    {
        float actualElapsedSec{ 0.0f };
        while (actualElapsedSec < std::numeric_limits<float>::epsilon())
        {
            actualElapsedSec = m_frameClock.getElapsedTime().asSeconds();
            actualElapsedSec *= m_simTimeMult;
        }

        m_frameClock.restart();

        return actualElapsedSec;
    }

    void Simulator::spawnMethHead(const Motivation motive, const std::size_t count)
    {
        assert(Motivation::none != motive);

        for (std::size_t i(0); i < count; ++i)
        {
            if (!isFreeBoardPos())
            {
                break;
            }

            if (motive == Motivation::lazy)
            {
                m_actors.push_back(std::make_unique<Lazy>(m_context));
            }
            else
            {
                m_actors.push_back(std::make_unique<Greedy>(m_context));
            }

            // std::cout << "Spawning "
            //          << ((m_actors.back()->motivation() == Motivation::lazy) ? "Lazy" : "Greedy")
            //          << " at " << m_actors.back()->boardPos() << std::endl;
        }
    }

    void Simulator::spawnLoot(const std::size_t count)
    {
        for (std::size_t i(0); i < count; ++i)
        {
            if (!isFreeBoardPos())
            {
                break;
            }

            m_pickups.push_back(std::make_unique<Loot>(m_context));

            // std::cout << "Spawning Loot worth " << m_pickups.back()->value() << " at "
            //          << m_pickups.back()->boardPos() << std::endl;
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

            // std::cout << "Killing "
            //          << ((m_actors.back()->motivation() == Motivation::lazy) ? "Lazy" : "Greedy")
            //          << " with score of " << m_actors.back()->score() << " at "
            //          << m_actors.back()->boardPos() << std::endl;

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

            // std::cout << "Killing Loot worth " << m_pickups.back()->value() << " at "
            //          << m_pickups.back()->boardPos() << std::endl;

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
        else if (sf::Keyboard::Right == event.key.code)
        {
            m_simTimeMult *= 1.1f;

            if (m_simTimeMult > 50.0f)
            {
                m_simTimeMult = 50.0f;
            }
        }
        else if (sf::Keyboard::Left == event.key.code)
        {
            m_simTimeMult *= 0.9f;

            if (m_simTimeMult < 0.1f)
            {
                m_simTimeMult = 0.1f;
            }
        }
        else if (sf::Keyboard::M == event.key.code)
        {
            if (event.key.shift)
            {
                killMethHead((event.key.control) ? 20 : 1);
            }
            else
            {
                spawnMethHead(Motivation::lazy, ((event.key.control) ? 10 : 1));
                spawnMethHead(Motivation::greedy, ((event.key.control) ? 10 : 1));
            }
        }
        else if (sf::Keyboard::L == event.key.code)
        {
            if (event.key.shift)
            {
                killLoot((event.key.control) ? 20 : 1);
            }
            else
            {
                spawnLoot((event.key.control) ? 20 : 1);
            }
        }
        else if (sf::Keyboard::S == event.key.code)
        {
            if (event.key.shift)
            {
                killMethHead(((event.key.control) ? 20 : 2));
                killLoot(((event.key.control) ? 20 : 2));
            }
            else
            {
                spawnMethHead(Motivation::lazy, ((event.key.control) ? 20 : 2));
                spawnMethHead(Motivation::greedy, ((event.key.control) ? 20 : 2));
                spawnLoot(((event.key.control) ? 40 : 4));
            }
        }
        else if (sf::Keyboard::R == event.key.code)
        {
            reset();
            spawnInitialPieces();
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
            actor->update(m_context, elapsedSec);

            if (ScopedBoardPosHandler::refCount(m_context, actor->boardPos()) > 1)
            {
                handleActorPickingUp(*actor);
            }
        }

        if (m_isModeNormal)
        {
            m_displayVars.updatePerFrame(m_context, elapsedSec);

            if (m_settings.query(Settings::SpecialEffects))
            {
                m_animationPlayer.update(elapsedSec);
            }
        }

        assert((m_actors.size() + m_pickups.size()) <= m_displayVars.constants().cell_count);
    }

    void Simulator::handleActorPickingUp(IActor & actor)
    {
        const BoardPos_t actorPos{ actor.boardPos() };

        const auto foundIter = std::find_if(
            std::begin(m_pickups), std::end(m_pickups), [&actorPos](const IPickupUPtr_t & pickup) {
                return (pickup->boardPos() == actorPos);
            });

        if (foundIter == std::end(m_pickups))
        {
            return;
        }

        if (m_isModeNormal && m_settings.query(Settings::SpecialEffects))
        {
            const sf::FloatRect actorWindowBounds(
                m_displayVars.constants().boardPosToWindowRect(actorPos));

            const sf::FloatRect animWindowBounds(scaleRectInPlaceCopy(actorWindowBounds, 4.0f));

            if (actor.motivation() == Motivation::lazy)
            {
                m_soundPlayer.play("coins-1", m_random);

                m_animationPlayer.play(
                    m_random, "puff", animWindowBounds, (actor.turnDelaySec() * 2.5f));
            }
            else
            {
                m_soundPlayer.play("coins-2", m_random);

                m_animationPlayer.play(
                    m_random, "orb", animWindowBounds, (actor.turnDelaySec() * 2.5f));
            }
        }

        (*foundIter)->changeActor(actor);

        m_pickups.erase(foundIter);

        spawnLoot();
    }

    void Simulator::printStatus()
    {
        ++m_framesSinceStatusCount;

        // only bother checking if it's time to display status every 100th frame
        if ((m_framesSinceStatusCount % 100) != 0)
        {
            return;
        }

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
            m_displayVars.updatePerStatus(fps, scores.lazy, scores.greedy);
        }

        std::cout.imbue(std::locale("")); // this is only to put commas in the big
        std::cout << std::setw(2) << std::right << m_statusCount++ << "  ";

#ifdef WIN32
        std::cout << "Win/";
#else
        std::cout << "Mac/";
#endif

#ifdef NDEBUG
        std::cout << "Rel/";
#else
        std::cout << "Dbg/";
#endif

        if (m_isModeNormal)
        {
            std::cout << "Show";
        }
        else
        {
            std::cout << "Hide";
        }

        std::cout << "  " << fps << " / " << m_framesPerSecondMax << "  ";
        std::cout << m_actors.size() << "/" << m_pickups.size() << "  ";
        std::cout << scores.lazy << " / " << scores.greedy << std::endl;
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