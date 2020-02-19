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
        , m_scoreText("", m_resources.font(Piece::Count), 99)
    {
        m_window.setFramerateLimit(m_settings.frame_rate_limit);

        reset();

        m_soundPlayer.load({ "cannon-miss.ogg",
                             "rpg-game-over.ogg",
                             "bounce-big.ogg",
                             "tap-1-a.ogg",
                             "miss.ogg",
                             "plop-sparkle.ogg",
                             "shine.ogg",
                             "mario-pause.ogg",
                             "mario-break-block.ogg" });

        if (m_settings.is_self_play_test)
        {
            m_board.printStatus();
            m_settings.printStatus();
        }
    }

    GameCoordinator::~GameCoordinator()
    {
        // if (m_settings.is_self_play_test)
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

        m_animationPlayer.stopAll();
        updateScore();

        if (m_settings.is_self_play_test)
        {
            m_soundPlayer.volume(0.0f);

            if (!m_soundPlayer.isMuted())
            {
                m_soundPlayer.muteButton();
            }
        }
        else
        {
            m_soundPlayer.volume(100.0f);
        }
    }

    void GameCoordinator::run()
    {
        try
        {
            sf::Clock frameClock;

            while (m_window.isOpen())
            {
                if (m_settings.is_self_play_test && m_settings.is_game_over)
                {
                    reset();
                }

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

        if (sf::Event::KeyPressed == event.type)
        {
            if (sf::Keyboard::R == event.key.code)
            {
                if (m_settings.is_game_over)
                {
                    std::cout << "'R' key pressed, so the game is being reset." << std::endl;
                    m_settings.printStatus();
                    reset();
                }
                else
                {
                    sf::Event newEvent{ event };
                    newEvent.key.code = sf::Keyboard::F;
                    handleEvent(newEvent);
                }

                return;
            }

            if (sf::Keyboard::S == event.key.code)
            {
                m_board.printStatus();
                m_settings.printStatus();
                return;
            }

            if (sf::Keyboard::Space == event.key.code)
            {
                if (!m_settings.is_game_over)
                {
                    m_soundPlayer.play("mario-pause");
                    m_settings.is_game_paused = !m_settings.is_game_paused;

                    std::cout << "'SPACE' key pressed, so the game will "
                              << ((m_settings.is_game_paused) ? "PAUSE" : "UN-PAUSE") << std::endl;
                }

                return;
            }
        }

        for (IPieceUPtr_t & pieceUPtr : m_board.pieces())
        {
            pieceUPtr->handleEvent(m_context, event);
        }
    }

    void GameCoordinator::update(const float frameTimeSec)
    {
        if (m_settings.is_game_paused || m_settings.is_game_over)
        {
            return;
        }

        if (!m_settings.is_self_play_test)
        {
            updateScore();
        }

        // m_animationPlayer.update(frameTimeSec);

        for (IPieceUPtr_t & pieceUPtr : m_board.pieces())
        {
            pieceUPtr->update(m_context, frameTimeSec);
        }
    }

    void GameCoordinator::updateScore()
    {
        std::string scoreStr{ std::to_string(m_settings.scoreIncrementAndGet()) };

        const std::size_t digitCount{ 6 };
        while (scoreStr.length() < digitCount)
        {
            scoreStr.insert(std::begin(scoreStr), '0');
        }

        m_scoreText.setString(scoreStr);

        auto textRegion{ m_context.settings.windowBounds() };
        textRegion.top += (textRegion.height / 2.0f);
        textRegion.height = (textRegion.top * 0.85f);
        util::scaleRectInPlace(textRegion, 0.7f);
        util::scaleAndCenterInside(m_scoreText, textRegion, true);

        m_scoreText.setFillColor(sf::Color(255, 255, 255, 32));
    }

    void GameCoordinator::draw()
    {
        if (!m_settings.is_self_play_test || m_settings.is_game_over)
        {
            m_window.clear();

            if (!m_settings.is_self_play_test)
            {
                m_window.draw(m_scoreText);
            }

            for (const IPieceUPtr_t & pieceUPtr : m_board.pieces())
            {
                m_window.draw(*pieceUPtr);
            }

            // m_window.draw(m_animationPlayer);

            // re-draw the text on top when game over or paused so the score is easy to see
            if (m_settings.is_game_paused || m_settings.is_game_over)
            {
                m_window.draw(m_scoreText);
            }

            m_window.display();
        }
    }
} // namespace boardgame