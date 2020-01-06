
#include "game.hpp"

#include <iostream>

Game::Game()
    : m_window(
          sf::VideoMode(1280, 1024, sf::VideoMode::getDesktopMode().bitsPerPixel), "Simple Effect")
    , m_bgTexture()
    , m_warnTexture()
    , m_states()
    , m_willClear(false)
    , m_windowSize(m_window.getSize())
    , m_bgSprite()
    , m_bgRotateSpeed(0.0f)
    , m_music()
    , m_random()
    , m_audio(m_random, "C:/src/learningcpp/media/sfx")
    , m_context(m_window, m_random, m_audio)
    , m_risingEffect()
    , m_trippyMushrooms()
{
    m_window.setFramerateLimit(60);

    m_audio.loadAll();

    if (!m_music.openFromFile("C:/src/learningcpp/media/music/trippy-shpongle.ogg"))
    {
        std::cout << "Unable to load music: trippy-shpongle" << std::endl;
    }

    m_music.setVolume(10.0f);
    m_music.play();

    if (!m_bgTexture.loadFromFile("C:/src/learningcpp/media/image/tiles/kaleidoscope.jpg"))
    {
        std::cout << "Unable to load texure: kaleidoscope" << std::endl;
    }

    m_bgTexture.setSmooth(true);
    m_bgSprite.setTexture(m_bgTexture);

    util::setOrigin2Center(m_bgSprite);

    m_bgSprite.setPosition(m_windowSize * 0.5f);
    m_bgSprite.setScale(2.0f, 2.0f);

    m_bgSprite.setColor(sf::Color(255, 255, 255, 127));

    if (!m_warnTexture.loadFromFile("C:/src/learningcpp/media/image/warning.png"))
    {
        std::cout << "Unable to load texure: warning" << std::endl;
    }

    m_warnTexture.setSmooth(true);

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

                m_risingEffect = RisingFadeEffect(m_warnTexture, mousePos);

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
    m_bgSprite.rotate(m_bgRotateSpeed);
    m_risingEffect.update(elapsedTimeSec, m_context);
    m_trippyMushrooms.update(elapsedTimeSec, m_context);
}

void Game::render()
{
    if (m_willClear)
    {
        m_window.clear();
    }

    m_window.draw(m_bgSprite, m_states);
    m_window.draw(m_risingEffect, m_states);
    m_window.draw(m_trippyMushrooms, m_states);

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
