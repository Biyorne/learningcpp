// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "simulator.hpp"

#include "scoped-board-position.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace methhead
{
    Simulator::Simulator(const Mode mode)
        : m_willStop(false)
        , m_enableSpecialEffects(mode == Mode::Normal)
        , m_willReRunInTestMode(false)
        , m_videoMode(1600u, 1200u, sf::VideoMode::getDesktopMode().bitsPerPixel)
        , m_window()
        , m_score()
        , m_random()
        , m_soundPlayer()
        , m_animationPlayer()
        , m_displayVars(sf::Vector2u(m_videoMode.width, m_videoMode.height))
        , m_actors()
        , m_frameClock()
        , m_framesPerSecondMax(0)
        , m_simTimeMult(1.0f)
        , m_context(
              (mode == Mode::Normal), m_random, m_actors, m_pickups, m_displayVars.constants())
        , m_testTurnCountdown(1) // anything > 0 works here
    {
        m_actors.reserve(m_displayVars.constants().cell_count);
        m_pickups.reserve(m_displayVars.constants().cell_count);

        if (m_context.is_mode_normal)
        {
            m_window.create(m_videoMode, "Meth Heads", sf::Style::Default);

            if (m_enableSpecialEffects)
            {
                m_animationPlayer.load({ "orb", "puff" });
                m_soundPlayer.load({ "coins-1", "coins-2" });
            }
        }

        reset();
    }

    void Simulator::reset()
    {
        m_frameClock.restart();

        m_simTimeMult = 1.0f;
        m_framesPerSecondMax = 0;

        m_soundPlayer.stopAll();
        m_animationPlayer.stopAll();

        // the order of these remaining items is critical
        m_actors.clear();
        m_pickups.clear();
        ScopedBoardPosition::reset(m_context);
        //
        spawnMethHead(Motivation::lazy, 1);
        spawnMethHead(Motivation::greedy, 1);
        // if you spawn the loot last, you don't have to forceActorsToPickTargets()
        spawnLoot(5);
    }

    bool Simulator::run()
    {
        while (!m_willStop)
        {
            if (m_context.is_mode_normal)
            {
                handleEvents();
                update(getElapsedSimFrameTimeSec());
                draw();
            }
            else
            {
                randomTestAction();
                update(1.0f);
            }
        }

        reset();
        return m_willReRunInTestMode;
    }

    void Simulator::randomTestAction()
    {
        if (--m_testTurnCountdown > 0)
        {
            return;
        }

        m_testTurnCountdown = (10 + m_random.zeroTo(90));

        sf::Event event;
        event.type = sf::Event::KeyPressed;

        event.key.shift = false;
        if ((m_actors.size() > 50) && (m_pickups.size() > 50))
        {
            event.key.shift = (m_random.zeroTo(10) == 0);
        }

        event.key.control = m_random.boolean();

        event.key.code = [&]() {
            switch (m_random.zeroTo(10_st))
            {
                case 0: return sf::Keyboard::R;

                case 1:
                case 2:
                case 3: return sf::Keyboard::L;

                case 4:
                case 5:
                case 6: return sf::Keyboard::M;

                default: return sf::Keyboard::S;
            }
        }();

        handleEvent(event);
    }

    float Simulator::getElapsedSimFrameTimeSec()
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
            if (!ScopedBoardPosition::isAnyPosFree(m_context))
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
        }
    }

    void Simulator::spawnLoot(const std::size_t count)
    {
        for (std::size_t i(0); i < count; ++i)
        {
            if (!ScopedBoardPosition::isAnyPosFree(m_context))
            {
                break;
            }

            m_pickups.push_back(std::make_unique<Loot>(m_context));
        }

        forceActorsToPickTargets();
    }

    void Simulator::killMethHead(const std::size_t count)
    {
        for (std::size_t i(0); i < count; ++i)
        {
            if (m_actors.empty())
            {
                break;
            }

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

            m_pickups.pop_back();
        }

        forceActorsToPickTargets();
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

            if (m_simTimeMult > 100.0f)
            {
                m_simTimeMult = 100.0f;
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
        }
        else if (sf::Keyboard::E == event.key.code)
        {
            m_enableSpecialEffects = !m_enableSpecialEffects;
        }
        else if (sf::Keyboard::T == event.key.code)
        {
            m_willReRunInTestMode = true;
            m_willStop = true;
        }
    }

    void Simulator::update(const float elapsedSec)
    {
        for (IActorUPtr_t & actor : m_actors)
        {
            if (!actor->update(m_context, elapsedSec))
            {
                continue;
            }

            if (ScopedBoardPosition::refCount(m_context, actor->boardPos()) > 1)
            {
                handleActorPickingup(*actor);
            }
        }

        m_score.update(m_context, elapsedSec);

        if (m_context.is_mode_normal)
        {
            if (m_enableSpecialEffects)
            {
                m_animationPlayer.update(elapsedSec);
            }
        }

        assert((m_actors.size() + m_pickups.size()) <= m_displayVars.constants().cell_count);
    }

    void Simulator::handleActorPickingup(IActor & actor)
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

        if (m_context.is_mode_normal && m_enableSpecialEffects)
        {
            const sf::FloatRect actorWindowBounds(
                m_displayVars.constants().boardPosToWindowRect(actorPos));

            const sf::FloatRect animWindowBounds(scaleRectInPlaceCopy(actorWindowBounds, 4.0f));

            if (actor.motivation() == Motivation::lazy)
            {
                m_soundPlayer.play("coins-1", m_random);
                m_animationPlayer.play(m_random, "puff", animWindowBounds, 0.5f);
            }
            else
            {
                m_soundPlayer.play("coins-2", m_random);
                m_animationPlayer.play(m_random, "orb", animWindowBounds, 0.5f);
            }
        }

        actor.pickup(m_context, **foundIter);
        swapAndPop(m_pickups, foundIter);
        spawnLoot();
    }

    void Simulator::draw()
    {
        const sf::RenderStates renderStates;

        m_window.clear(sf::Color(64, 64, 64));

        m_displayVars.draw(m_context, m_window, renderStates);

        m_score.draw(m_window, renderStates);

        if (m_context.is_mode_normal && m_enableSpecialEffects)
        {
            m_animationPlayer.draw(m_window, renderStates);
        }

        m_window.display();
    }

    void Simulator::forceActorsToPickTargets()
    {
        for (IActorUPtr_t & actor : m_actors)
        {
            actor->pickTarget(m_context);
        }
    }
} // namespace methhead