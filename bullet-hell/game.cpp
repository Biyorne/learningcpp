
#include "game.hpp"

#include <iostream>

Game::Game()
    : m_window(sf::VideoMode(640, 480), "Bullet Hell")
    , m_playerTexture()
    , m_playerSprite()
    , m_isMovingUp()
    , m_isMovingDown()
    , m_isMovingLeft()
    , m_isMovingRight()
{
    if (!m_playerTexture.loadFromFile("C:/src/learningcpp/media/image/head-2.png"))
    {
        std::cout << "Unable to load texure" << std::endl;
    }
    m_playerSprite.setTexture(m_playerTexture);
    m_playerSprite.setPosition(100.0f, 100.0f);
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    const auto TimePerFrame(sf::seconds(1.0f / 60.0f));
    while (m_window.isOpen())
    {
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            update(TimePerFrame);
        }
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::KeyPressed:
            {
                handlePlayerInput(event.key.code, true);
                break;
            }
            case sf::Event::KeyReleased:
            {
                handlePlayerInput(event.key.code, false);
                break;
            }
            case sf::Event::Closed:
            {
                m_window.close();
                break;
            }
        }
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }
    }
}

void Game::update(sf::Time deltaTime)
{
    sf::Vector2f movement(0.0f, 0.0f);
    float playerSpeed(60.0f);
    if (m_isMovingUp)
    {
        movement.y -= playerSpeed;
    }

    if (m_isMovingDown)
    {
        movement.y += playerSpeed;
    }

    if (m_isMovingLeft)
    {
        movement.x -= playerSpeed;
    }

    if (m_isMovingRight)
    {
        movement.x += playerSpeed;
    }

    m_playerSprite.move(movement * deltaTime.asSeconds());
}

void Game::render()
{
    m_window.clear();
    m_window.draw(m_playerSprite);
    m_window.display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
    if (key == sf::Keyboard::W)
    {
        m_isMovingUp = isPressed;
    }
    else if (key == sf::Keyboard::S)
    {
        m_isMovingDown = isPressed;
    }
    else if (key == sf::Keyboard::A)
    {
        m_isMovingLeft = isPressed;
    }
    else if (key == sf::Keyboard::D)
    {
        m_isMovingRight = isPressed;
    }
}
