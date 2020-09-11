// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "game.hpp"

#include "exploder.hpp"
#include "mouse-follower.hpp"
#include "movement.hpp"
#include "rising-fade.hpp"
#include "util.hpp"
#include "wall-bouncer.hpp"

#include <iostream>

Game::Game()
    : m_window(
          sf::VideoMode(1600, 1200, sf::VideoMode::getDesktopMode().bitsPerPixel),
          "Simple Effects",
          sf::Style::Default)
    , m_bloomWindow(m_window)
    , m_resources()
    , m_random()
    , m_audio(m_random, "media/audio")
    , m_context(m_window, m_random, m_audio, m_resources)
    , m_bgSprite(util::makeTiledSprite(**m_resources.seamless_iter, m_bloomWindow.renderTarget()))
    , m_effects()
    , m_emitter(m_context)
    , m_statusText(m_context)
    , m_simTimeMult(1.0f)
{
    m_window.setFramerateLimit(60);

    const sf::Vector2f backdropScale(
        m_context.window_size / sf::Vector2f(m_resources.backdrop_texture.getSize()));

    m_audio.load({ "camera-click", "tap.ogg" });

    reset();

    std::cout << "1: Add Rising Fade\n";
    std::cout << "2: Add Follower\n";
    std::cout << "3: Add Wall Bouncer\n";
    std::cout << "4: Toggle Particle Emitter at Mouse Position\n";
    // std::cout << "5: Exploder\n";
}

void Game::reset()
{
    m_audio.stopAll();
    m_effects.clear();
    m_emitter.reset();
    m_simTimeMult = 1.0f;
    m_statusText.setPostfix("");

    // start with Particle Emitter
    m_emitter.toggleEmission({ 590, 620 });
    m_bloomWindow.isEnabled(true);
    m_bloomWindow.blurMultipassCount(5);
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
            m_audio.play("tap");
        }
        else if (sf::Keyboard::R == event.key.code)
        {
            reset();
            m_audio.play("tap");
        }
        else if (sf::Keyboard::B == event.key.code)
        {
            m_bloomWindow.isEnabled(!m_bloomWindow.isEnabled());
            m_audio.play("tap");
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
        else if (sf::Keyboard::Left == event.key.code)
        {
            m_bgSprite =
                util::makeTiledSprite(m_resources.bgTexturePrev(), m_bloomWindow.renderTarget());

            m_audio.play("tap");
        }
        else if (sf::Keyboard::Right == event.key.code)
        {
            m_bgSprite =
                util::makeTiledSprite(m_resources.bgTextureNext(), m_bloomWindow.renderTarget());

            m_audio.play("tap");
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            // Rising Fade
            m_effects.push_back(std::make_unique<entity::RisingFadeEffect>(
                m_context, m_resources.particle_texture, 0.1f));

            m_audio.play("tap");
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            // Mouse Follower
            m_effects.push_back(std::make_unique<entity::MouseFollowerEffect>(
                m_context, m_resources.rabbit_texture, 0.2f, 1000.0f, 500.0f));

            m_audio.play("tap");
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        {
            // Wall Bouncer
            m_effects.push_back(std::make_unique<entity::WallBouncerEffect>(
                m_context, m_resources.warn_texture, sf::Vector2f(200.0f, -200.0f)));

            m_audio.play("tap");
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
        {
            // Particle Emitter
            m_emitter.toggleEmission(m_context.mouse_pos);
            m_bloomWindow.isEnabled(m_emitter.isEmitting());
            m_bloomWindow.blurMultipassCount(5);
            m_audio.play("tap");
        }
        // else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
        //{
        //    // Exploder
        //    m_effects.push_back(
        //        std::make_unique<entity::ExploderEffect>(m_context,
        //        m_resources.exploder_texture));
        //
        //    m_audio.play("tap");
        //}

        //
    }
    else if (event.type == sf::Event::MouseWheelScrolled)
    {
        // On Nel's laptop, values are whole numbers from[-5,5] but usually just [-1,1] //
        // On Til's laptop, values are reals around [0.0083, 5.0f]
        const float scrollDelta(event.mouseWheelScroll.delta);

        changeSimTimeOnMouseScroll(scrollDelta);
        m_audio.play("tap");
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
    m_bloomWindow.clear();

    util::tileTarget(m_bgSprite, m_bloomWindow.renderTarget());

    for (entity::IEffectUPtr_t & effect : m_effects)
    {
        m_bloomWindow.draw(*effect);
    }

    m_bloomWindow.draw(m_emitter);

    m_bloomWindow.draw(m_statusText);

    m_bloomWindow.display();
}

void Game::changeSimTimeOnMouseScroll(const float scrollDelta)
{
    std::ostringstream ss;

    if (scrollDelta > 0.0f)
    {
        m_simTimeMult *= 1.1f;
        ss << "(+) ";
    }
    else
    {
        m_simTimeMult *= 0.9f;
        ss << "(-) ";
    }

    const float simTimePercent(std::round((m_simTimeMult * 100.0f) * 10.0f) / 10.0f);
    ss << std::setw(6) << simTimePercent << "% sim speed:  (" << m_simTimeMult << ')';

    m_statusText.setPostfix(ss.str());
}
