
#include "game.hpp"

#include <iostream>

// void Thing::update(const float frameTimeSec)
//{
//    // LOL CLANG FORMAT
//    sprite.setPosition();
//    sprite.rotate(m_rotateSpeed * frameTimeSec);
//}
//
// void Thing::draw(sf::RenderTarget & target, sf::RenderStates states)
//{
//    target.draw(sprite, states);
//}

Game::Game()
    : m_window(
          sf::VideoMode(1280, 1024, sf::VideoMode::getDesktopMode().bitsPerPixel), "Simple Effect")
    , m_texture()
    , m_states()
    , m_sprite()
    , m_rotateSpeed(0.0f)
    , m_moveSpeed(0.0f)
    , m_willClear(false)
    , m_willBlendAdd(false)
    , m_windowSize(m_window.getSize())
    , m_velocity(0.0f, 0.0f)
    , m_acceleration(0.0f, 0.0f)
{
    m_window.setFramerateLimit(60);

    if (!m_texture.loadFromFile("swirl.png"))
    {
        std::cout << "Unable to load texure" << std::endl;
    }

    m_texture.setSmooth(true);

    m_sprite.setTexture(m_texture);

    m_sprite.setOrigin(
        (m_sprite.getLocalBounds().width * 0.5f), (m_sprite.getLocalBounds().height * 0.5f));

    reset();
}

void Game::reset()
{
    m_sprite.setPosition(
        (m_windowSize.x * 0.5f),
        (m_windowSize.y - (m_sprite.getGlobalBounds().height * 0.5f)) + 2.0f);

    m_rotateSpeed = 150.0f;
    m_moveSpeed = 20.0f;
    m_willClear = true;
    m_willBlendAdd = false;
    m_velocity = { 0.0f, 0.0f };
    m_acceleration = { 0.0f, 0.5f };
}

void Game::run()
{
    sf::Clock clock;

    while (m_window.isOpen())
    {
        processEvents();

        const float frameTimeSec(clock.getElapsedTime().asSeconds());
        clock.restart();

        update(frameTimeSec);

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
                handleKeyPress(event.key.code);
                break;
            }

            case sf::Event::Closed:
            {
                m_window.close();
                break;
            }

            default: break;
        }
    }
}

void Game::update(const float frameTimeSec)
{
    const sf::Vector2f mousePos(sf::Mouse::getPosition(m_window));
    const sf::Vector2f spritePos(m_sprite.getPosition());

    const sf::Vector2f posDiff(mousePos - spritePos);

    m_velocity += (posDiff * frameTimeSec * 0.1f);

    m_velocity += m_acceleration;
    m_sprite.move(m_velocity * (frameTimeSec * m_moveSpeed));

    const float spriteBottom{ m_sprite.getPosition().y +
                              (m_sprite.getGlobalBounds().height * 0.5f) };

    if ((spriteBottom > m_windowSize.y) && (m_velocity.y > 0.0f))
    {
        // bounce
        // m_velocity.y *= -1.0f;

        // land and stop
        m_velocity.y = 0.0f;
    }

    m_sprite.rotate(m_rotateSpeed * frameTimeSec);
}

void Game::render()
{
    if (m_willBlendAdd)
    {
        m_states.blendMode = sf::BlendAdd;
    }
    else
    {
        m_states.blendMode = sf::BlendAlpha;
    }

    if (m_willClear)
    {
        m_window.clear();
    }

    m_window.draw(m_sprite, m_states);
    m_window.display();
}

void Game::handleKeyPress(const sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Escape)
    {
        m_window.close();
    }
    else if (key == sf::Keyboard::R)
    {
        reset();
    }
    else if (key == sf::Keyboard::C)
    {
        m_willClear = !m_willClear;
    }
    else if (key == sf::Keyboard::B)
    {
        m_willBlendAdd = !m_willBlendAdd;
    }
    else if (key == sf::Keyboard::Space)
    {
        m_velocity.y = -25.0f;
    }
    else if ((key == sf::Keyboard::Left) || (key == sf::Keyboard::Right))
    {
        const bool willIncrease(key == sf::Keyboard::Right);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            changeSpeed(willIncrease, m_rotateSpeed);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        {
            changeSpeed(willIncrease, m_moveSpeed);
        }
    }
}
