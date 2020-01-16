
#include "game.hpp"

#include <iostream>

Game::Game()
    : m_resources()
    , m_window(
          sf::VideoMode(1280, 1024, sf::VideoMode::getDesktopMode().bitsPerPixel), "Simple Effect")
    , m_states()
    , m_willClear(false)
    , m_windowSize(m_window.getSize())
    , m_random()
    , m_audio(m_random, "C:/src/learningcpp/media/sfx")
    , m_context(m_window, m_random, m_audio)
    //, m_effect(m_context, 50.0f, m_resources.rabbit_texture)
    , m_bgSprite(m_resources.bg_texture)
    , m_effect(
          55.0f, m_resources.particle_texture, (m_context.window_size * 0.5f), sf::Texture(), {})
{
    m_window.setFramerateLimit(60);

    // m_audio.loadAll();
    m_resources.bg_texture.setRepeated(true);
    m_bgSprite.setTextureRect({ { 0, 0 }, sf::Vector2i(m_context.window_size) });

    reset();
}

void Game::reset()
{
    m_states = sf::RenderStates();
    m_willClear = true;
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
                // On Nel's laptop, values are whole numbers from[-5,5] but usually just [-1,1] //
                // On Til's laptop, values are reals around [0.0083, 5.0f]
                const float scrollAmount(event.mouseWheelScroll.delta);
                std::cout << scrollAmount << std::endl;
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
    m_effect.update(elapsedTimeSec, m_context);
}

void Game::render()
{
    if (m_willClear)
    {
        m_window.clear();
    }
    m_window.draw(m_bgSprite);
    // m_window.draw(m_bgSprite, sf::BlendAdd);
    m_window.draw(m_effect, m_states);

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
