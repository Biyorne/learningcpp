// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "game.hpp"

Game::Game()
    : m_window(sf::VideoMode(640, 480), "Bullet Hell")
    , m_player()
{
    m_player.setRadius(40.0f);
    m_player.setPosition(100.0f, 100.0f);
    m_player.setFillColor(sf::Color::Red);
}

void Game::run()
{
    while (m_window.isOpen())
    {
        handleEvents();
        update();
        draw();
    }
}

void Game::handleEvents()
{
    sf::Event event;

    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
            break;
        }
    }
}

void Game::update()
{
    // TODO
}

void Game::draw()
{
    m_window.clear(sf::Color(64, 64, 64));
    m_window.draw(m_player);
    m_window.display();
}
