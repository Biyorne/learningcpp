
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
    , m_willClear()
    , m_willBlendAdd()
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
    m_sprite.setPosition(0.0f, 0.0f);
    m_rotateSpeed = 150.0f;
    m_moveSpeed = 1.0f;
    m_willClear = true;
    m_willBlendAdd = false;
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

    m_sprite.move(posDiff * (frameTimeSec * m_moveSpeed));
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
    else if ((key == sf::Keyboard::Left) || (key == sf::Keyboard::Right))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            changeSpeed(key, m_rotateSpeed);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        {
            changeSpeed(key, m_moveSpeed);
        }
    }
}
