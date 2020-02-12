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
    GameCoordinator::GameCoordinator(const std::filesystem::path & mediaDirPath)
        : m_videoMode(2880u, 1800u, sf::VideoMode::getDesktopMode().bitsPerPixel)
        , m_window(m_videoMode, "Lame Board Game", sf::Style::Default)
        , m_board()
        , m_random()
        , m_resources(mediaDirPath)
        , m_soundPlayer(m_random, (mediaDirPath / "sfx").string())
        , m_animationPlayer(m_random, (mediaDirPath / "animation").string())
        , m_context(
              mediaDirPath,
              m_window,
              m_resources,
              m_board,
              m_random,
              m_soundPlayer,
              m_animationPlayer)
        , m_frameClock()
        , m_timeBetweenMovesSec(0.25f)
    {
        m_window.setFramerateLimit(60);
        reset();
    }

    void GameCoordinator::reset()
    {
        m_board.reset();
        m_board.setupMap(m_context);
        m_soundPlayer.stopAll();
        m_animationPlayer.stopAll();
        m_context.is_self_testing = false;
        m_frameClock.restart();
    }

    void GameCoordinator::run()
    {
        while (m_window.isOpen())
        {
            handleEvents();
            update(m_frameClock.restart().asSeconds());
            draw();
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
        if (sf::Event::Closed == event.type)
        {
            m_window.close();
        }
        // else if (sf::Event::KeyPressed != event.type)
        //{
        //    return;
        //}
        // else
        //{
        //}
    }

    void GameCoordinator::update(const float elapsedTimeSec)
    {
        m_animationPlayer.update(elapsedTimeSec);

        for (IPieceUPtr_t & piece : m_context.board.pieces())
        {
            piece->update(m_context, elapsedTimeSec);
        }
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