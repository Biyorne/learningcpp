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
        , m_actors()
        , m_frameClock()
        , m_framesPerSecondMax(0)
        , m_statusClock()
        , m_framesSinceStatusCount(0)
        , m_statusIntervalSec(1.0f)
        , m_statusCount(0)
        , m_context(m_random, m_actors, m_pickups, m_displayVars.constants(), m_settings)
        , m_statsFile("timing.csv", std::fstream::trunc)
    {
        m_settings.printAll();

        if (m_isModeNormal)
        {
            m_window.create(m_videoMode, "Meth Heads", sf::Style::Default);

            m_animationPlayer.load({ "orb", "puff" });
            m_soundPlayer.load({ "coins-1", "coins-2" });
        }

        spawnInitialPieces();

        const std::string statsHeader{ "time,version,os,build,mode,draw_type,fps,"
                                       "fps_max,actor_count,pickup_count,lazy_score,greedy_score" };

        std::cout << statsHeader << std::endl;
        m_statsFile << statsHeader << std::endl;
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

    std::optional<BoardPos_t> Simulator::findRandomFreeBoardPos() const
    {
        std::vector<BoardPos_t> positions(m_displayVars.constants().cell_count);

        std::size_t index(0);
        for (int vert(0); vert < m_displayVars.constants().cell_countsI.y; ++vert)
        {
            positions[index].y = vert;
            for (int horiz(0); horiz < m_displayVars.constants().cell_countsI.x; ++horiz)
            {
                BoardPos_t & pos{ positions[index] };
                pos.x = horiz;
                pos.y = vert;
                ++index;
            }
        }

        for (const auto & actor : m_actors)
        {
            const BoardPos_t & actorPos{ actor->boardPos() };

            positions.erase(
                std::remove_if(
                    std::begin(positions),
                    std::end(positions),
                    [&](const BoardPos_t & posToCheck) { return (posToCheck == actorPos); }),
                std::end(positions));
        }

        for (const auto & pickup : m_pickups)
        {
            const BoardPos_t & pickupPos{ pickup->boardPos() };

            positions.erase(
                std::remove_if(
                    std::begin(positions),
                    std::end(positions),
                    [&](const BoardPos_t & posToCheck) { return (posToCheck == pickupPos); }),
                std::end(positions));
        }

        if (positions.empty())
        {
            return std::nullopt;
        }

        const BoardPos_t randomFreeBoardPos{ m_random.from(positions) };

        assert(m_context.isActorAt(randomFreeBoardPos) == false);
        assert(m_context.isPickupAt(randomFreeBoardPos) == false);
        assert(m_context.isAnythingAt(randomFreeBoardPos) == false);

        return randomFreeBoardPos;
    }

    void Simulator::spawnMethHead(const Motivation motive)
    {
        assert(Motivation::none != motive);

        const auto freeBoardPosOpt(findRandomFreeBoardPos());
        if (!freeBoardPosOpt)
        {
            return;
        }

        if (motive == Motivation::lazy)
        {
            m_actors.push_back(std::make_unique<Lazy>(freeBoardPosOpt.value()));
        }
        else
        {
            m_actors.push_back(std::make_unique<Greedy>(freeBoardPosOpt.value()));
        }
    }

    void Simulator::spawnLoot()
    {
        const auto freeBoardPosOpt(findRandomFreeBoardPos());
        if (!freeBoardPosOpt)
        {
            return;
        }

        const int lootAmount{ m_random.fromTo(1, 100) };
        m_pickups.push_back(std::make_unique<Loot>(freeBoardPosOpt.value(), lootAmount));
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
            std::cout << "Reset to initial state." << std::endl;

            m_settings = Settings();

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
                handleActorPickingUpSometing(*actor);
            }
        }

        if (m_isModeNormal && m_settings.query(Settings::SpecialEffects))
        {
            m_animationPlayer.update(elapsedSec);
        }
    }

    void Simulator::handleActorPickingUpSometing(IActor & actor)
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

        // for (const auto & actor : m_actors)
        //{
        //    std::cout << "\n\t actor="
        //              << ((actor->motivation() == Motivation::lazy) ? "lazy" : "greedy") << "="
        //              << actor->boardPos() << "$" << actor->score();
        //}
        //
        // for (const auto & pickup : m_pickups)
        //{
        //    std::cout << "\n\t pickup= "
        //              << "$" << pickup->value();
        //}
        //
        // std::cout << std::endl;

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

        std::ostringstream ss;

        ss << m_statusCount++ << ',';

        ss << "v1,";

#ifdef WIN32
        ss << "Win,";
#else
        ss << "Mac,";
#endif

#ifdef NDEBUG
        ss << "Release,";
#else
        ss << "Debug,";
#endif

        if (m_isModeNormal)
        {
            ss << "Normal,";
        }
        else
        {
            ss << "SpeedTest,";
        }

        if (m_settings.query(Settings::DrawBoardWithVerts))
        {
            ss << "DrawVerts,";
        }
        else
        {
            ss << "DrawRectangles,";
        }

        ss << fps << ',' << m_framesPerSecondMax << ',';
        ss << m_actors.size() << ',' << m_pickups.size() << ',';
        ss << scores.lazy << ',' << scores.greedy;

        m_statsFile << ss.str() << std::endl;
        std::cout << ss.str() << std::endl;
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