
#include "game.hpp"

#include <iostream>

Game::Game()
    : m_window(
          sf::VideoMode(1280, 1024, sf::VideoMode::getDesktopMode().bitsPerPixel), "Simple Effect")
    , m_bgTexture()
    , m_warnTexture()
    , m_states()
    , m_willClear(false)
    , m_willBlendAdd(false)
    , m_windowSize(m_window.getSize())
    , m_bgSprite()
    , m_warnSprites()
    , m_bgRotateSpeed(0.0f)
    , m_music()
{
    m_window.setFramerateLimit(60);

    if (!m_music.openFromFile("C:/src/learningcpp/media/music/trippy-shpongle.ogg"))
    {
        std::cout << "Unable to load music: trippy-shpongle" << std::endl;
    }

    m_music.setVolume(50.0f);
    m_music.play();

    if (!m_bgTexture.loadFromFile("C:/src/learningcpp/media/image/tiles/kaleidoscope.jpg"))
    {
        std::cout << "Unable to load texure: kaleidoscope" << std::endl;
    }

    m_bgTexture.setSmooth(true);
    m_bgSprite.setTexture(m_bgTexture);

    m_bgSprite.setOrigin(
        (m_bgSprite.getLocalBounds().width * 0.5f), (m_bgSprite.getLocalBounds().height * 0.5f));

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
    m_willBlendAdd = false;
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
                const sf::Vector2f mousePos(sf::Mouse::getPosition(m_window));

                sf::Sprite warnSprite;
                warnSprite.setTexture(m_warnTexture);

                warnSprite.setOrigin(
                    (warnSprite.getLocalBounds().width * 0.5f),
                    (warnSprite.getLocalBounds().height * 0.5f));

                warnSprite.setPosition(mousePos);
                warnSprite.setColor(sf::Color(255, 64, 64, 255));
                warnSprite.setScale(0.2f, 0.2f);

                m_warnSprites.push_back(warnSprite);

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

    const float scaleDelta(1.0f + (elapsedTimeSec * 0.02f));
    m_bgSprite.scale(scaleDelta, scaleDelta);

    for (sf::Sprite & sprite : m_warnSprites)
    {
        sprite.setColor(sprite.getColor() - sf::Color(0, 0, 0, 5));
        sprite.scale(1.03f, 1.03f);
        sprite.rotate(scaleDelta * 10.0f);
    }
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

    m_window.draw(m_bgSprite, m_states);

    for (const sf::Sprite & sprite : m_warnSprites)
    {
        m_window.draw(sprite, m_states);
    }

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
}

// void Game::scrollValue(float amount)
//{
//
//}
