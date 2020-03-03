// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "game.hpp"

#include "exploder.hpp"
#include "follower.hpp"
#include "rising-fade.hpp"
#include "steady-mover.hpp"
#include "util.hpp"
#include "wall-bouncer.hpp"

#include <iostream>

Game::Game()
    : m_window(
          sf::VideoMode(1024, 768, sf::VideoMode::getDesktopMode().bitsPerPixel), "Simple Effect")
    , m_bloomWindow(m_window)
    , m_resources()
    , m_random()
    , m_audio(m_random, "C:/src/learningcpp/media/sfx")
    , m_context(m_window, m_random, m_audio, m_resources)
    , m_willClear(false)
    , m_bgSprite(m_resources.bg_texture)
    , m_effects()
    , m_emitter()
    , m_simTimeMult(1.0f)
    , m_statusText(m_context)
//, m_quadVerts(sf::Quads)// Color Gradient
//, m_offScreenTexture()
//, m_image()
{
    m_window.setFramerateLimit(60);

    // m_audio.loadAll();
    m_resources.bg_texture.setRepeated(true);
    m_bgSprite.setTextureRect({ { 0, 0 }, sf::Vector2i(m_context.window_size) });

    m_audio.load({ "camera-click" });

    // Color Gradient: Colored vertexes that make a rect/quad
    // m_quadVerts.append(sf::Vertex({ 0.0f, 0.0f }, sf::Color::Red));
    // m_quadVerts.append(sf::Vertex({ m_context.window_size.x, 0.0f }, sf::Color::Blue));
    // m_quadVerts.append(sf::Vertex(m_context.window_size, sf::Color::Green));
    // m_quadVerts.append(sf::Vertex({ 0.0f, m_context.window_size.y }, sf::Color::White));
    //
    // const sf::Vector2u sizeU(m_context.window_size);
    // m_offScreenTexture.create(sizeU.x, sizeU.y);
    // m_offScreenTexture.clear();
    // m_offScreenTexture.draw(m_quadVerts);
    // m_offScreenTexture.display();
    //
    // m_image = m_offScreenTexture.getTexture().copyToImage();

    reset();

    std::cout << "1: Rising Fade\n";
    std::cout << "2: Follower\n";
    std::cout << "3: Wall Bouncer\n";
    std::cout << "4: Toggle Particle Emitter\n";
    std::cout << "5: Exploder\n";
}

void Game::reset()
{
    m_audio.stopAll();
    m_willClear = true;
    m_effects.clear();
    m_emitter.reset();
}

void Game::run()
{
    sf::Clock clock;

    while (m_window.isOpen())
    {
        m_context.mouse_pos = sf::Vector2f(sf::Mouse::getPosition(m_window));

        processEvents();
        update(clock.restart().asSeconds() * m_simTimeMult);
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (m_window.isOpen() && m_window.pollEvent(event))
    {
        processEvent(event);

        // clang-format off
        const bool willEffectsIgnoreEvent{
            (event.type == sf::Event::GainedFocus) ||
            (event.type == sf::Event::LostFocus) ||
            (event.type == sf::Event::MouseEntered) ||
            (event.type == sf::Event::MouseLeft) ||
            (event.type == sf::Event::MouseMoved) };
        // clang-format on

        if (!willEffectsIgnoreEvent)
        {
            for (auto & effect : m_effects)
            {
                effect->handleEvent(m_context, event);
            }
        }
    }
}

void Game::processEvent(const sf::Event & event)
{
    if (event.type == sf::Event::Closed)
    {
        m_window.close();
        return;
    }

    if (event.type == sf::Event::KeyPressed)
    {
        if (sf::Keyboard::Escape == event.key.code)
        {
            m_window.close();
        }
        else if (sf::Keyboard::R == event.key.code)
        {
            reset();
        }
        else if (sf::Keyboard::C == event.key.code)
        {
            m_willClear = !m_willClear;
        }
        else if (sf::Keyboard::B == event.key.code)
        {
            m_bloomWindow.isEnabled(!m_bloomWindow.isEnabled());
        }
        else if (sf::Keyboard::P == event.key.code)
        {
            sf::Vector2u windowSize(m_window.getSize());

            sf::Texture texture;
            texture.create(windowSize.x, windowSize.y);
            texture.update(m_window);

            if (texture.copyToImage().saveToFile("screenshot.jpg"))
            {
                m_audio.play("camera-click");
            }
        }
    }
    else if (event.type == sf::Event::MouseWheelScrolled)
    {
        // On Nel's laptop, values are whole numbers from[-5,5] but usually just [-1,1] //
        // On Til's laptop, values are reals around [0.0083, 5.0f]
        const float scrollAmount(event.mouseWheelScroll.delta);

        if (scrollAmount > 0.0f)
        {
            m_simTimeMult *= 1.1f;
        }
        else
        {
            m_simTimeMult *= 0.9f;
        }

        std::ostringstream ss;
        ss << scrollAmount << "->" << m_simTimeMult;

        std::cout << ss.str() << std::endl;

        m_statusText.setPostfix(ss.str());
    }
    else if (event.type == sf::Event::MouseButtonPressed)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            m_effects.push_back(std::make_unique<entity::RisingFadeEffect>(
                m_context, m_resources.particle_texture, 0.1f));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            m_effects.push_back(std::make_unique<entity::MouseFollowerEffect>(
                m_context, m_resources.rabbit_texture, 0.333f, 1000.0f, 500.0f));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        {
            m_effects.push_back(std::make_unique<entity::WallBouncerEffect>(
                m_context, m_resources.warn_texture, sf::Vector2f(200.0f, -200.0f), m_window));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
        {
            m_emitter.toggleEmission();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
        {
            const std::size_t tileCount{ static_cast<std::size_t>(
                sf::Mouse::getPosition(m_window).y / 2.5f) };

            const auto size{ m_resources.exploder_texture.getSize() };
            if ((tileCount > 4) && (tileCount < std::min(size.x, size.y)))
            {
                m_effects.push_back(std::make_unique<entity::ExploderEffect>(
                    m_context, m_resources.exploder_texture, m_context.mouse_pos.y, tileCount));
            }
        }
    }
}

void Game::update(const float frameTimeSec)
{
    for (entity::IEffectUPtr_t & effect : m_effects)
    {
        effect->update(m_context, frameTimeSec);
    }

    m_emitter.update(m_context, frameTimeSec);

    m_statusText.update(m_context, frameTimeSec);
}

void Game::render()
{
    if (m_willClear)
    {
        m_bloomWindow.clear();
    }

    m_bloomWindow.draw(m_bgSprite);

    for (entity::IEffectUPtr_t & effect : m_effects)
    {
        m_bloomWindow.draw(*effect);
    }

    m_bloomWindow.draw(m_emitter);

    m_bloomWindow.draw(m_statusText);

    m_bloomWindow.display();
}
