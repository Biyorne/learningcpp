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
    GameCoordinator::GameCoordinator(const GameSettingsBase & settings)
        : m_settings(settings)
        , m_window(m_settings.videoMode(), m_settings.name(), m_settings.windowStyle())
        , m_board()
        , m_random()
        , m_resources()
        , m_soundPlayer(m_random, (settings.mediaDirPath() / "sfx").string())
        , m_animationPlayer(m_random, (settings.mediaDirPath() / "animation").string())
        , m_context(m_settings, m_resources, m_board, m_random, m_soundPlayer, m_animationPlayer)
    {
        m_window.setFramerateLimit(m_settings.frameRateLimit());

        m_resources.setup(settings.mediaDirPath());

        m_board.setup(m_settings);

        m_soundPlayer.load({ "cannon-miss.ogg",
                             "rpg-game-over.ogg",
                             "bounce-big.ogg",
                             "tap-1-a.ogg",
                             "miss.ogg",
                             "plop-sparkle.ogg",
                             "shine.ogg",
                             "mario-pause.ogg",
                             "mario-break-block.ogg",
                             "spawn-huge.ogg",
                             "little-spirits-shimmer.ogg" });

        m_soundPlayer.volume(75.0f);

        m_board.populateBoardWithMapPieces(m_context);
    }

    GameCoordinator::~GameCoordinator() {}

    void GameCoordinator::run()
    {
        sf::Clock frameClock;

        while (m_window.isOpen() && !m_settings.isGameOver())
        {
            handleEvents();
            update(frameClock.restart().asSeconds());
            draw();
        }
    }

    void GameCoordinator::handleEvents()
    {
        sf::Event event;
        while (m_window.isOpen() && !m_settings.isGameOver() && m_window.pollEvent(event))
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
            std::cout << "Player quit the game." << std::endl;
            m_settings.isGameOver(true);
            return;
        }

        if ((sf::Event::KeyPressed == event.type) && (sf::Keyboard::Space == event.key.code))
        {
            m_soundPlayer.play("mario-pause");

            std::cout << "Player pressed spacebar:  "
                      << ((m_settings.isGamePaused()) ? "RESUMING" : "PAUSING") << std::endl;

            m_settings.isGamePaused(!m_settings.isGamePaused());
            return;
        }

        // clang-format off
        const bool willPiecesIgnoreEvent{
            (sf::Event::KeyReleased == event.type) ||
            (sf::Event::MouseMoved == event.type) ||
            (sf::Event::MouseEntered == event.type) ||
            (sf::Event::MouseLeft == event.type) ||
            (sf::Event::GainedFocus == event.type) ||
            (sf::Event::LostFocus == event.type) ||
            (sf::Event::TouchBegan == event.type) ||
            (sf::Event::TouchEnded == event.type) ||
            (sf::Event::TouchMoved == event.type) };
        // clang-format on

        if (!willPiecesIgnoreEvent)
        {
            m_board.handleEvent(m_context, event);
        }
    }

    void GameCoordinator::update(const float elapsedTimeSec)
    {
        if (m_settings.isGamePaused() || m_settings.isGameOver())
        {
            return;
        }

        m_board.update(m_context, elapsedTimeSec);
    }

    void GameCoordinator::draw()
    {
        m_window.clear();
        m_window.draw(m_board);
        m_window.display();
    }
} // namespace boardgame