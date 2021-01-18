// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// splash-coordinator.cpp
//
#include "splash-coordinator.hpp"

#include "check-macros.hpp"
#include "util.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>

#include <SFML/Graphics.hpp>

namespace castlecrawl
{
    SplashCoordinator::SplashCoordinator()
        : m_window()
        , m_texture()
        , m_sprite()
        , m_config()
    {}

    void SplashCoordinator::loadAll(const GameConfig & config)
    {
        m_config = config;
        M_CHECK_SS(std::filesystem::exists(m_config.media_dir_path), m_config.media_dir_path);
        M_CHECK_SS(std::filesystem::is_directory(m_config.media_dir_path), m_config.media_dir_path);

        // this can change m_config so call this right after m_config is set
        openWindow();

        const bool loadSuccess =
            m_texture.loadFromFile((m_config.media_dir_path / "image" / "splash.png").string());

        M_CHECK_SS(loadSuccess, "failed to load splash.png");

        m_sprite.setTexture(m_texture);

        util::scale(m_sprite, (m_config.windowSize() * 0.5f));
        util::centerInside(m_sprite, sf::FloatRect({ 0.0f, 0.0f }, m_config.windowSize()));
    }

    void SplashCoordinator::openWindow()
    {
        m_window.close();

        const auto style{ (m_config.is_fullscreen) ? sf::Style::Fullscreen : sf::Style::Default };

        m_window.create(m_config.video_mode, m_config.game_name, style);

        M_CHECK_SS(
            m_window.isOpen(),
            "Failed to make and open the splash graphics window.  (sf::RenderWindow::isOpen() == "
            "false)");

        m_window.setFramerateLimit(60);

        const sf::Vector2u windowActualSize{ m_window.getSize() };

        m_config.video_mode.width = windowActualSize.x;
        m_config.video_mode.height = windowActualSize.y;
        m_config.video_mode.bitsPerPixel = m_window.getSettings().depthBits;
    }

    void SplashCoordinator::run(const GameConfig & config)
    {
        loadAll(config);

        sf::Clock frameClock;
        while (m_window.isOpen())
        {
            handleEvents();
            update(frameClock.restart().asSeconds());
            draw();
        }
    }

    void SplashCoordinator::handleEvents()
    {
        sf::Event event;
        while (m_window.isOpen() && m_window.pollEvent(event))
        {
            handleEvent(event);
        }
    }

    void SplashCoordinator::handleEvent(const sf::Event & event)
    {
        if ((sf::Event::Closed == event.type) || (sf::Event::KeyPressed == event.type) ||
            (sf::Event::MouseButtonPressed == event.type))
        {
            m_window.close();
        }
    }

    void SplashCoordinator::update(const float) {}

    void SplashCoordinator::draw()
    {
        m_window.clear(sf::Color::Black);
        m_window.draw(m_sprite);
        m_window.display();
    }

} // namespace castlecrawl
