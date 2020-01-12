
#include "game.hpp"

#include <iostream>

Game::Game()
    : m_resources()
    , m_window(
          sf::VideoMode(1280, 1024, sf::VideoMode::getDesktopMode().bitsPerPixel), "Simple Effect")
    , m_states()
    , m_willClear(false)
    , m_windowSize(m_window.getSize())
    , m_bgSprite()
    , m_bgRotateSpeed(0.0f)
    , m_random()
    , m_audio(m_random, "C:/src/learningcpp/media/sfx")
    , m_context(m_window, m_random, m_audio)
    , m_follower(
          25.0f,
          m_resources.rabbit_texture,
          { 100.0f, 100.0f },
          m_resources.carrot_texture,
          (m_context.window_size * 0.5f))
{
    m_window.setFramerateLimit(60);

    m_audio.loadAll();

    // m_bgSprite.setTexture(m_resources.bg_texture);
    // util::setOrigin2Center(m_bgSprite);
    // m_bgSprite.setPosition(m_windowSize * 0.5f);
    // m_bgSprite.setScale(2.0f, 2.0f);
    // m_bgSprite.setColor(sf::Color(255, 255, 255, 127));

    reset();
}

void Game::reset()
{
    m_states = sf::RenderStates();
    m_willClear = true;
    m_bgRotateSpeed = 0.05f;
}

void Game::run()
{
    sf::Clock clock;

    while (m_window.isOpen())
    {
        const sf::Vector2f mousePos(sf::Mouse::getPosition(m_window));
        m_context.mouse_pos = mousePos;

        processEvents();
        update(clock.restart().asSeconds());
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

            case sf::Event::MouseWheelScrolled:
            {
                const float scrollAmount(event.mouseWheelScroll.delta);
                m_bgRotateSpeed += (scrollAmount * 0.5f);
                break;
            }

            case sf::Event::MouseButtonPressed:
            {
                m_audio.play("pretty.ogg", 0.5f);
                const sf::Vector2f mousePos(sf::Mouse::getPosition(m_window));

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

void Game::update(const float elapsedTimeSec)
{
    // m_bgSprite.rotate(m_bgRotateSpeed);
    m_follower.update(elapsedTimeSec, m_context);
}

void Game::render()
{
    if (m_willClear)
    {
        m_window.clear();
    }

    // m_window.draw(m_bgSprite, m_states);
    m_window.draw(m_follower, m_states);

    m_window.display();
}

void Game::handleKeyPress(const sf::Keyboard::Key key)
{
    m_audio.play("tap");

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
}

// void Game::scrollValue(float amount)
//{
//
//}
