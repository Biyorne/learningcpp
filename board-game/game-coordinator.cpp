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
    {
        m_window.setFramerateLimit(m_settings.frame_rate_limit);

        reset();

        m_soundPlayer.load({ "cannon-miss.ogg",
                             "rpg-game-over.ogg",
                             "bounce-small-deep-rising.ogg",
                             "bounce-small-short.ogg",
                             "tap-1-a.ogg",
                             "miss.ogg",
                             "plop-sparkle.ogg",
                             "shine.ogg" });

        if (m_settings.is_self_test)
        {
            m_board.printStatus();
            m_settings.printStatus();
        }
    }

    GameCoordinator::~GameCoordinator()
    {
        // if (m_settings.is_self_test)
        //{
        //    m_board.printStatus();
        //    m_settings.printStatus();
        //}
    }

    void GameCoordinator::reset()
    {
        m_settings.reset();
        m_board.reset(m_context);
        m_soundPlayer.stopAll();
        m_soundPlayer.volume(100.0f);
        m_animationPlayer.stopAll();
    }

    void GameCoordinator::run()
    {
        try
        {
            sf::Clock frameClock;

            while (m_window.isOpen())
            {
                handleEvents();
                update(frameClock.restart().asSeconds());
                draw();
            }
        }
        catch (const std::exception & ex)
        {
            std::cout << "EXCEPTION ERROR:  \"" << ex.what() << "\"" << std::endl;
        }
        catch (...)
        {
            std::cout << "EXCEPTION ERROR:  \"UNKOWNN\"" << std::endl;
        }

        m_board.printStatus();
        m_settings.printStatus();
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
            std::cout << "Player quit the game." << std::endl;
            m_settings.is_game_over = true;
            m_window.close();
            return;
        }

        if (sf::Event::KeyPressed != event.type)
        {
            return;
        }

        if (sf::Keyboard::R == event.key.code)
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
        else if (util::isArrowKey(event.key.code))
        {
            for (IPieceUPtr_t & pieceUPtr : m_board.pieces())
            {
                if (pieceUPtr->which() == Piece::Head)
                {
                    pieceUPtr->handleEvent(m_context, event);
                }
            }
        }
    }

    void GameCoordinator::update(const float frameTimeSec)
    {
        if (m_settings.is_game_paused || m_settings.is_game_over)
        {
            return;
        }

        m_animationPlayer.update(frameTimeSec);

        for (IPieceUPtr_t & pieceUPtr : m_board.pieces())
        {
            M_ASSERT_OR_THROW(pieceUPtr);

            if (!pieceUPtr->isInPlay())
            {
                continue;
            }

            {
                const BoardPos_t pos{ pieceUPtr->position() };

                M_ASSERT_OR_THROW(
                    (pos.x >= 0) && (pos.x < m_board.cells().counts_int.x) && (pos.y >= 0) &&
                    (pos.y < m_board.cells().counts_int.x));
            }

            pieceUPtr->update(m_context, frameTimeSec);

            M_ASSERT_OR_THROW(pieceUPtr);

            {
                const BoardPos_t pos{ pieceUPtr->position() };

                M_ASSERT_OR_THROW(
                    (pos.x >= 0) && (pos.x < m_board.cells().counts_int.x) && (pos.y >= 0) &&
                    (pos.y < m_board.cells().counts_int.x));
            }
        }

        // misc periodic tasks
        if ((m_settings.total_turns_played % 10) == 0)
        {
            m_board.eraseAllPiecesNotInPlay();
            FoodPiece::spawn(m_context);
        }
    }

    void GameCoordinator::draw()
    {
        m_window.clear();

        for (const IPieceUPtr_t & pieceUPtr : m_board.pieces())
        {
            if (!pieceUPtr->isInPlay())
            {
                continue;
            }

            m_window.draw(*pieceUPtr);
        }

        m_window.draw(m_animationPlayer);

        m_window.display();
    }
} // namespace boardgame