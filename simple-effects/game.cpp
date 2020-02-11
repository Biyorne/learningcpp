// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "game.hpp"

#include <iostream>

Game::Game()
    : m_window(
          sf::VideoMode(1024, 768, sf::VideoMode::getDesktopMode().bitsPerPixel), "Simple Effect")
    , m_resources()
    , m_random()
    , m_audio(m_random, "C:/src/learningcpp/media/sfx")
    , m_context(m_window, m_random, m_audio)
    , m_willClear(false)
    , m_bgSprite(m_resources.bg_texture)
    , m_effects()
//, m_quadVerts(sf::Quads)// Color Gradient
//, m_offScreenTexture()
//, m_image()
{
    // m_window.setFramerateLimit(60);

    // m_audio.loadAll();
    m_resources.bg_texture.setRepeated(true);
    m_bgSprite.setTextureRect({ { 0, 0 }, sf::Vector2i(m_context.window_size) });

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
}

void Game::reset()
{
    m_audio.stopAll();
    m_willClear = true;
    m_effects.clear();
}

void Game::run()
{
    sf::Clock clock;

    while (m_window.isOpen())
    {
        m_context.mouse_pos = sf::Vector2f(sf::Mouse::getPosition(m_window));

        processEvents();
        update(clock.restart().asSeconds());
        render();
    }
}

void Game::processEvents()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        m_effects.push_back(
            std::make_unique<entity::RisingFadeEffect>(m_context, m_resources.particle_texture));
    }

    sf::Event event;
    while (m_window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::KeyPressed:
            {
                m_audio.play("tap");

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

            case sf::Event::MouseButtonPressed: { break;
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
    for (entity::IEffectUPtr_t & effect : m_effects)
    {
        effect->update(m_context, frameTimeSec);
    }
}

void Game::render()
{
    if (m_willClear)
    {
        m_window.clear();
    }

    m_window.draw(m_bgSprite);

    // sf::Vector2u spritePos(m_effect.sprite.getPosition());
    // m_effect.sprite.setColor(m_image.getPixel(spritePos.x, spritePos.y));
    // m_window.draw(m_effect, m_states);

    for (entity::IEffectUPtr_t & effect : m_effects)
    {
        m_window.draw(*effect);
    }

    m_window.display();
}

// void Game::scrollValue(float amount)
//{
//
//}
