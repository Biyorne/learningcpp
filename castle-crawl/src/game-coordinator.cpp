// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// game-coordinator.cpp
//
#include "game-coordinator.hpp"

#include "util.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>

#include <SFML/Graphics.hpp>

namespace castlecrawl
{
    GameCoordinator::GameCoordinator()
        : m_window()
        , m_fps()
        , m_maps()
        , m_media()
        , m_board()
        , m_layout()
        , m_game()
        , m_config()
        , m_random()
        , m_soundPlayer(m_random)
        , m_animationPlayer(m_random)
        , m_context(
              m_game,
              m_maps,
              m_board,
              m_config,
              m_layout,
              m_media,
              m_random,
              m_soundPlayer,
              m_animationPlayer)
    {}

    void GameCoordinator::initializeSubsystems(const GameConfig & config)
    {
        m_game.reset();

        m_config = config;
        M_CHECK_SS(std::filesystem::exists(m_config.media_dir_path), m_config.media_dir_path);
        M_CHECK_SS(std::filesystem::is_directory(m_config.media_dir_path), m_config.media_dir_path);

        // this can change m_config so call this right after m_config is set
        openWindow();

        m_soundPlayer.setMediaPath((m_config.media_dir_path / "sfx").string());
        m_soundPlayer.volume(75.0f);

        m_animationPlayer.setMediaPath((m_config.media_dir_path / "anim").string());

        m_media.load(m_config, m_soundPlayer);

        // depends only on m_random only so passing context here is safe TODO
        m_maps.load(m_context);

        m_context.switchToMap({ { 0, 0 }, "level-1-entry-hall", { 5, 2 } });
    }

    void GameCoordinator::openWindow()
    {
        m_window.close();

        const auto style{ (m_config.is_fullscreen) ? sf::Style::Fullscreen : sf::Style::Default };

        m_window.create(m_config.video_mode, m_config.game_name, style);

        m_window.setFramerateLimit(m_config.frame_rate_limit);
        m_window.setKeyRepeatEnabled(false);

        std::cout << "Game Window: " << m_config.windowSize<int>() << " at "
                  << m_config.video_mode.bitsPerPixel << "bits per pixel and a "
                  << m_config.frame_rate_limit << " fps limit." << std::endl;

        M_CHECK_SS(
            m_window.isOpen(),
            "Failed to make and open the graphics window.  (sf::RenderWindow::isOpen() == false)");

        // verify the window size is what was specified/expected,
        // otherwise all the size/positions calculations will be wrong
        const sf::Vector2u windowExpectedSize{ m_config.windowSize<unsigned>() };

        const sf::Vector2u windowActualSize{ m_window.getSize() };

        if (windowActualSize != windowExpectedSize)
        {
            std::cout << "Failed to create a window at " << windowExpectedSize
                      << ", but strangely, a window did open at " << windowActualSize
                      << ".  So...meh." << std::endl;
        }

        m_config.video_mode.width = windowActualSize.x;
        m_config.video_mode.height = windowActualSize.y;
        m_config.video_mode.bitsPerPixel = m_window.getSettings().depthBits;

        std::cout << "Game Window Cells: width_ratio=" << m_config.map_cell_size_ratio
                  << ", pixels=" << m_config.mapCellDimm()
                  << ", grid=" << (m_config.windowSize<float>() / m_config.mapCellSize())
                  << std::endl;
    }

    void GameCoordinator::run(const GameConfig & config)
    {
        initializeSubsystems(config);

        m_fps.reset(m_context);

        sf::Clock frameClock;
        while (m_window.isOpen() && !m_game.isGameOver())
        {
            handleEvents();
            update(frameClock.restart().asSeconds());
            draw();
        }

        printFinalStatusToConsole();
    }

    void GameCoordinator::printFinalStatusToConsole()
    {
        if (m_game.isGameOver())
        {
            std::cout << "Game Over.   You " << ((m_game.didPlayerWin()) ? "WON!" : "LOST  (loser)")
                      << std::endl;
        }
        else
        {
            std::cout << "Application shutdown before game was over." << std::endl;
        }

        std::cout << "Final score = " << m_game.score() << std::endl;
    }

    void GameCoordinator::handleEvents()
    {
        sf::Event event;
        while (m_window.isOpen() && !m_game.isGameOver() && m_window.pollEvent(event))
        {
            handleEvent(event);
        }
    }

    bool GameCoordinator::handleExitEvent(const sf::Event & event)
    {
        if (sf::Event::Closed == event.type)
        {
            m_window.close();
            return true;
        }

        if (sf::Event::KeyPressed == event.type)
        {
            const sf::Keyboard::Key key{ event.key.code };

            if ((sf::Keyboard::Q == key) || (sf::Keyboard::Escape == key))
            {
                m_game.endGame(false);
                return true;
            }

            m_board.player.handleEvent(m_context, event);
        }

        return false;
    }

    void GameCoordinator::handleEvent(const sf::Event & event)
    {
        if (GameCoordinator::handleExitEvent(event))
        {
            return;
        }
    }

    void GameCoordinator::update(const float elapsedSec)
    {
        m_animationPlayer.update(elapsedSec);
        m_fps.update();
    }

    void GameCoordinator::draw()
    {
        m_window.clear(m_config.background_color);

        // draw floor and wall tiles
        m_context.map().draw(m_context, m_window, {});

        // draw all other pieces
        m_window.draw(m_board);

        m_window.draw(m_fps);

        sf::RectangleShape rs;
        rs.setPosition(1.0f, 1.0f);
        rs.setSize(m_config.windowSize() - sf::Vector2f{ 2.0f, 2.0f });
        rs.setFillColor(sf::Color::Transparent);
        rs.setOutlineThickness(1.0f);
        rs.setOutlineColor(sf::Color(80, 80, 80));
        m_window.draw(rs);

        m_window.draw(m_animationPlayer);

        m_window.display();
    }

} // namespace castlecrawl
