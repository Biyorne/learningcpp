// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// main.cpp
//
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

//

class ParticleEmitter : public sf::Drawable
{
public:
    explicit ParticleEmitter(const sf::Vector2f & position)
        : m_particles()
        , m_vertArray(sf::Points)
        , m_lifetimeMax(sf::seconds(3.0f))
        , m_position(position)
        , m_willDrift(false)
    {}

    std::size_t count() const { return m_vertArray.getVertexCount(); }

    void position(const sf::Vector2f & position) { m_position = position; }

    void add(const std::size_t number = 1000)
    {
        if (number <= 0)
        {
            return;
        }

        const std::size_t newSize(count() + number);

        m_particles.resize(newSize);
        m_vertArray.resize(newSize);
    }

    void clear()
    {
        m_particles.resize(0);
        m_vertArray.resize(0);
    }

    void update(const sf::Time & elapsed, const sf::Vector2f & driftPos)
    {
        const auto particleCount { count() };
        for (std::size_t i(0); i < particleCount; ++i)
        {
            Particle & particle(m_particles[i]);
            sf::Vertex & vertex(m_vertArray[i]);

            particle.lifetime_remaining -= elapsed;
            if (particle.lifetime_remaining <= sf::Time::Zero)
            {
                respawn(particle, vertex);
            }

            vertex.position += (particle.velocity * elapsed.asSeconds());

            if (m_willDrift)
            {
                vertex.position += ((driftPos - vertex.position) * elapsed.asSeconds());
            }

            const float lifetimeRatio(
                particle.lifetime_remaining.asSeconds() / m_lifetimeMax.asSeconds());

            vertex.color.a = static_cast<sf::Uint8>(lifetimeRatio * 255);
        }
    }

    void drift(const bool will) { m_willDrift = will; }

private:
    void draw(sf::RenderTarget & target, sf::RenderStates states) const final
    {
        // states.blendMode = sf::BlendAdd;
        target.draw(m_vertArray, states);
    }

private:
    struct Particle
    {
        sf::Vector2f velocity;
        sf::Time lifetime_remaining;
    };

    void respawn(Particle & particle, sf::Vertex & vertex)
    {
        const float angleRad((std::rand() % 360) * (3.14f / 180.0f));
        const float speed((std::rand() % 100) + 100.0f);
        particle.velocity.x = (std::cos(angleRad) * speed);
        particle.velocity.y = (std::sin(angleRad) * speed);

        particle.lifetime_remaining = sf::milliseconds((std::rand() % 2000) + 1000);

        vertex.position = m_position;
        vertex.color = sf::Color::White;

        // vertex.color.r = randomColorValue();
        // vertex.color.g = randomColorValue();
        // vertex.color.b = randomColorValue();
    }

    unsigned char randomColorValue() const
    {
        return static_cast<unsigned char>((std::rand() % 255));
    }

    std::vector<Particle> m_particles;
    sf::VertexArray m_vertArray;
    sf::Time m_lifetimeMax;
    sf::Vector2f m_position;
    bool m_willDrift;
};

//

int main()
{
    std::cout.imbue(std::locale("")); // this is only to put commas in big numbers

    bool willDrift(false);
    bool isFullscreen(false);
    const std::string appName("Particles");
    const sf::VideoMode videoMode(1600, 1200, sf::VideoMode::getDesktopMode().bitsPerPixel);

    sf::RenderWindow window(videoMode, appName, sf::Style::Default);

    const sf::Vector2f windowSize(window.getSize());

    std::vector<ParticleEmitter> emitters;

    sf::Clock clock;
    sf::Clock statusClock;
    std::size_t framesPerStatusCounter(0);

    while (window.isOpen())
    {
        const sf::Vector2f mousePos(sf::Mouse::getPosition(window));

        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (sf::Event::Closed == event.type)
            {
                window.close();
                break;
            }

            if (sf::Event::KeyPressed != event.type)
            {
                continue;
            }

            if (sf::Keyboard::Escape == event.key.code)
            {
                window.close();
                break;
            }

            if (sf::Keyboard::B == event.key.code)
            {
                if (!sf::Shader::isAvailable())
                {
                    std::cout << "'B' Keypress ignored because shaders are NOT supported.  Your "
                                 "video card sucks."
                              << std::endl;
                    continue;
                }

                willBloom = !willBloom;
            }
            else if (sf::Keyboard::F == event.key.code)
            {
                isFullscreen = !isFullscreen;

                window.close();

                window.create(
                    videoMode,
                    appName,
                    ((isFullscreen) ? sf::Style::Fullscreen : sf::Style::Default));
            }
            else if (sf::Keyboard::S == event.key.code)
            {
                if (!event.key.shift)
                {
                    emitters.emplace_back(mousePos).add();
                }
                else
                {
                    if (emitters.empty())
                    {
                        emitters.emplace_back(mousePos);
                    }

                    for (auto & emitter : emitters)
                    {
                        emitter.add();
                    }
                }
            }
            else if (sf::Keyboard::R == event.key.code)
            {
                emitters.clear();
            }
            else if (sf::Keyboard::D == event.key.code)
            {
                willDrift = !willDrift;
                for (auto & emitter : emitters)
                {
                    emitter.drift(willDrift);
                }
            }
        }

        // status
        {
            ++framesPerStatusCounter;

            const float timeBetweenStatusSec { 1.0f };
            const float timeElapsedSinceLastStatusSec(statusClock.getElapsedTime().asSeconds());
            if (timeElapsedSinceLastStatusSec > timeBetweenStatusSec)
            {
                const int framesPerSecond(static_cast<int>(
                    static_cast<float>(framesPerStatusCounter) / timeElapsedSinceLastStatusSec));

                std::size_t particleCount(0);
                for (const auto & emitter : emitters)
                {
                    particleCount += emitter.count();
                }

                std::cout << emitters.size() << " emitters, " << particleCount << " particles, "
                          << framesPerSecond << "fps" << std::endl;

                statusClock.restart();
                framesPerStatusCounter = 0;
            }
        }

        // update
        {
            const sf::Time elapsed(clock.restart());
            for (auto & emitter : emitters)
            {
                emitter.update(elapsed, mousePos);
            }
        }

        // draw
        {
            window.clear();

            for (const auto & emitter : emitters)
            {
                window.draw(emitter);
            }

            window.display();
        }
    }

    return EXIT_SUCCESS;
}
