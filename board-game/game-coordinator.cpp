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
    GameCoordinator::GameCoordinator()
        : m_map()
        , m_config()
        , m_window()
        , m_random()
        , m_soundPlayer(m_random)
        , m_animationPlayer(m_random)
        , m_layout()
        , m_media()
        , m_game()
        , m_context(
              m_game,
              m_map,
              m_config,
              m_layout,
              m_media,
              m_random,
              m_soundPlayer,
              m_animationPlayer)
    {}

    void GameCoordinator::reset(const GameConfig & config, const MapChars_t & mapChars)
    {
        m_config = config;
        M_CHECK_SS(std::filesystem::exists(m_config.media_dir_path), m_config.media_dir_path);
        M_CHECK_SS(std::filesystem::is_directory(m_config.media_dir_path), m_config.media_dir_path);

        m_window.close();
        openWindow();
        m_window.setFramerateLimit(m_config.frame_rate_limit);

        m_soundPlayer.volume(0.0f);
        m_soundPlayer.stopAll();
        m_soundPlayer.setMediaPath((m_config.media_dir_path / "sfx").string());
        m_soundPlayer.volume(75.0f);

        m_animationPlayer.stopAll();
        m_animationPlayer.setMediaPath((m_config.media_dir_path / "animation").string());

        m_media.setup(m_config);

        m_map.reset(m_context, mapChars);
        switchToMap(m_map);
    }

    void GameCoordinator::switchToMap(const Map & map)
    {
        m_map = map;

        M_CHECK_SS((!m_map.empty()), "Map is empty.");

        m_layout.setup(m_map, m_config);
        m_game.reset();
    }

    void GameCoordinator::openWindow()
    {
        const auto style{ (m_config.is_fullscreen) ? sf::Style::Fullscreen : sf::Style::Default };

        m_window.create(m_config.video_mode, m_config.game_name, style);

        M_CHECK_SS(
            m_window.isOpen(),
            "Failed to open the window specified: " << m_config.windowSize<int>() << ":"
                                                    << m_config.video_mode.bitsPerPixel);

        // verify the window size is what was specified/expected,
        // otherwise all the size/positions calculations will be wrong
        const sf::Vector2u windowExpectedSize{ m_config.windowSize<unsigned>() };

        const sf::Vector2u windowActualSize{ m_window.getSize() };

        M_CHECK_LOG_SS(
            (windowActualSize == windowExpectedSize),
            "Failed to create a window at the resolution specified: "
                << windowExpectedSize
                << ".  Strangely, a window did open, just at a different resolution: "
                << windowActualSize << ".  So...meh.  Let's just run with it.");

        m_config.video_mode.width = windowActualSize.x;
        m_config.video_mode.height = windowActualSize.y;
        m_config.video_mode.bitsPerPixel = m_window.getSettings().depthBits;
    }

    void GameCoordinator::run()
    {
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
            if ((sf::Keyboard::Q == event.key.code) || (sf::Keyboard::Escape == event.key.code))
            {
                m_game.endGame(false);
                return true;
            }

            // if (sf::Keyboard::R == event.key.code)
            //{
            //    reset(m_config, m_map);
            //    return true;
            //}
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

    void GameCoordinator::update(const float) {}

    void GameCoordinator::draw()
    {
        m_window.clear(m_config.background_color);

        const float mapCellDimm{ m_config.mapCellDimm() };
        const sf::Vector2f boardPos{ util::position(m_layout.boardBounds()) };
        // draw draw bottom pieces (floor, etc.)
        sf::Vector2f pos{ boardPos };
        for (const std::string & mapLine : m_map.m_floorChars)
        {
            for (const char mapChar : mapLine)
            {
                sf::Sprite & sprite = m_media.sprite(mapCharToTileImage(mapChar));
                sprite.setPosition(pos);
                m_window.draw(sprite);

                pos.x += mapCellDimm;
            }

            pos.x = boardPos.x;
            pos.y += mapCellDimm;
        }

        // draw top pieces (walls, players, items, etc.)
        pos = boardPos;
        for (const std::string & mapLine : m_map.m_mapChars)
        {
            char prevMapChar(0);
            for (const char mapChar : mapLine)
            {
                sf::Sprite & sprite = m_media.sprite(mapCharToTileImage(mapChar));
                sprite.setPosition(pos);
                m_window.draw(sprite);

                // draw horiz wall shadow accents
                if (('-' == mapChar) && ('-' != prevMapChar))
                {
                    sf::Sprite & shadow = m_media.sprite(TileImage::WallHorizShadow);
                    shadow.setPosition(pos);
                    m_window.draw(shadow);
                }

                prevMapChar = mapChar;
                pos.x += mapCellDimm;
            }

            pos.x = boardPos.x;
            pos.y += mapCellDimm;
        }

        m_window.display();
    }

} // namespace boardgame
