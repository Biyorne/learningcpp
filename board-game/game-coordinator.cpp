// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// game-coordinator.cpp
//
#include "game-coordinator.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>

namespace boardgame
{
    GameCoordinator::GameCoordinator(const SnakeGameSettings & settings)
        : m_settings(settings)
        , m_window(m_settings.video_mode, m_settings.game_name, m_settings.sf_window_style)
        , m_board()
        , m_random()
        , m_resources(settings.media_path)
        , m_soundPlayer(m_random, (settings.media_path / "sfx").string())
        , m_animationPlayer(m_random, (settings.media_path / "animation").string())
        , m_context(m_settings, m_resources, m_board, m_random, m_soundPlayer, m_animationPlayer)
        , m_frameClock()
        , m_frameTimeElapsedSec(0.0f)
        , m_frameTimeElapsedMinSec(0.001f)
        , m_periodClock()
    {
        m_window.setFramerateLimit(m_settings.frame_rate_limit);

        reset();
        if (m_settings.is_self_test)
        {
            m_settings.printStatus();
            m_board.printStatus();
        }
    }

    GameCoordinator::~GameCoordinator()
    {
        if (m_settings.is_self_test)
        {
            m_settings.printStatus();
            m_board.printStatus();
        }
    }

    void GameCoordinator::reset()
    {
        m_settings.reset();
        m_board.reset(m_context);
        m_soundPlayer.stopAll();
        m_animationPlayer.stopAll();

        m_frameClock.restart();
        m_frameTimeElapsedSec = 0.0f;

        m_periodClock.restart();
    }

    void GameCoordinator::run()
    {
        try
        {
            while (m_window.isOpen())
            {
                handleEvents();

                if (!m_settings.is_game_paused && !m_settings.is_game_over)
                {
                    updatePerFrame();
                    updatePerPeriod();
                }

                draw();
            }

            std::cout << "Final Score: " << m_settings.food_eaten_count << std::endl;
        }
        catch (const std::exception & ex)
        {
            std::cout << "EXCEPTION ERROR:  \"" << ex.what() << "\"" << std::endl;
        }
        catch (...)
        {
            std::cout << "EXCEPTION ERROR:  \"UNKOWNN\"" << std::endl;
        }
    }

    void GameCoordinator::handleEvents()
    {
        sf::Event event;
        while (m_window.isOpen() && m_window.pollEvent(event))
        {
            handleEvent(event);
        }
    }

    void GameCoordinator::handleEvent(const sf::Event & event)
    {
        if (sf::Event::Closed == event.type ||
            ((sf::Event::KeyPressed != event.type) &&
             (sf::Keyboard::Q == event.key.code || sf::Keyboard::Escape == event.key.code)))
        {
            std::cout << "The game is ending because the player's a wussy." << std::endl;
            m_settings.is_game_over = true;
            m_window.close();
        }
        else if (sf::Event::KeyPressed != event.type)
        {
            return;
        }
        else if (sf::Keyboard::R == event.key.code)
        {
            std::cout << "'R' key pressed, so the game is being reset." << std::endl;
            reset();
        }
        else if (sf::Keyboard::S == event.key.code)
        {
            m_board.printStatus();
            m_settings.printStatus();
        }
        else if (sf::Keyboard::Space == event.key.code)
        {
            m_settings.is_game_paused = !m_settings.is_game_paused;

            std::cout << "'SPACE' key pressed, so the game will "
                      << ((m_settings.is_game_paused) ? "PAUSE" : "UN-PAUSE") << std::endl;
        }
    }

    void GameCoordinator::updatePerFrame()
    {
        m_frameTimeElapsedSec += m_frameClock.getElapsedTime().asSeconds();
        if (m_frameTimeElapsedSec < m_frameTimeElapsedMinSec)
        {
            return;
        }

        m_frameClock.restart();

        m_animationPlayer.update(m_frameTimeElapsedSec);

        for (IPieceUPtr_t & piece : m_context.board.pieces())
        {
            piece->update(m_context, m_frameTimeElapsedSec);
        }

        m_frameTimeElapsedSec = 0.0f;
    }

    void GameCoordinator::updatePerPeriod()
    {
        if (m_periodClock.getElapsedTime().asSeconds() < m_settings.period_duration_sec)
        {
            return;
        }

        m_periodClock.restart();

        m_board.removePiecesThatAreNoLongerInPlay();
    }

    void GameCoordinator::draw()
    {
        m_window.clear();

        for (const IPieceUPtr_t & piece : m_board.pieces())
        {
            m_window.draw(*piece);
        }

        m_window.draw(m_animationPlayer);

        m_window.display();
    }
} // namespace boardgame