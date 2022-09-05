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

#include "random.hpp"

//

struct Particle
{
    sf::Vector2f velocity { 0.0f, 0.0f };
    sf::Time lifetime_remaining { sf::milliseconds(0) };
};

//

class ParticleEmitter : public sf::Drawable
{
public:
    explicit ParticleEmitter(const sf::Vector2f & position)
        : m_particles()
        , m_vertArray(sf::Points)
        , m_lifetimeMax(sf::seconds(5.0f))
        , m_position(position)
    { }

    std::size_t size() const { return m_vertArray.getVertexCount(); }

    bool empty() const { return (0 == size()); }

    void position(const sf::Vector2f & position) { m_position = position; }

    Particle & particle(const std::size_t index) { return m_particles[index]; }
    const Particle & particle(const std::size_t index) const { return m_particles[index]; }

    sf::Vertex & vertex(const std::size_t index) { return m_vertArray[index]; }
    const sf::Vertex & vertex(const std::size_t index) const { return m_vertArray[index]; }

    void remove(const std::size_t index)
    {
        const auto particleCount { size() };

        if (1 == particleCount)
        {
            m_particles.clear();
            m_vertArray.clear();
            return;
        }

        if (index >= particleCount)
        {
            return;
        }

        // use swap & pop
        m_particles[index] = m_particles[particleCount - 1];
        m_particles.resize(particleCount - 1);

        m_vertArray[index] = m_vertArray[particleCount - 1];
        m_vertArray.resize(particleCount - 1);
    }

    void add(const std::size_t count)
    {
        if (count <= 0)
        {
            return;
        }

        const std::size_t newSize { size() + count };

        m_particles.resize(newSize);
        m_vertArray.resize(newSize);
    }

    void clear()
    {
        m_particles.resize(0);
        m_vertArray.resize(0);
    }

    void update(const sf::Time & elapsed, const bool willDrift, const sf::Vector2f & driftPos)
    {
        const auto particleCount { size() };
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

            if (willDrift)
            {
                vertex.position += ((driftPos - vertex.position) * elapsed.asSeconds());
            }

            // const float lifetimeRatio(
            //    particle.lifetime_remaining.asSeconds() / m_lifetimeMax.asSeconds());

            // vertex.color.a = static_cast<sf::Uint8>(lifetimeRatio * 255);
        }
    }

    std::size_t handleCollisions(ParticleEmitter & otherEmitter)
    {
        std::size_t collisionCount { 0 };

        const std::size_t myParticleCount { size() };
        for (std::size_t myIndex { 0 }; myIndex < myParticleCount; ++myIndex)
        {
            const auto myPosition { vertex(myIndex).position };

            const std::size_t otherParticleCount { otherEmitter.size() };
            for (std::size_t otherIndex { 0 }; otherIndex < otherParticleCount; ++otherIndex)
            {
                const auto otherPosition { otherEmitter.vertex(otherIndex).position };

                const auto positionDiff { myPosition - otherPosition };

                if ((std::abs(positionDiff.x) < 1.0f) && (std::abs(positionDiff.y) < 1.0f))
                {
                    ++collisionCount;
                    remove(myIndex);
                    otherEmitter.remove(otherIndex);
                    continue;
                }
            }
        }

        return collisionCount;
    }

private:
    void draw(sf::RenderTarget & target, sf::RenderStates states) const final
    {
        target.draw(m_vertArray, states);
    }

    void respawn(Particle & particle, sf::Vertex & vertex)
    {
        const float angleRad { m_random.fromTo(0.0f, 360.0f) * (3.14f / 180.0f) };
        const float speed { m_random.fromTo(0.0f, 100.0f) + 100.0f };

        particle.velocity.x = (std::cos(angleRad) * speed);
        particle.velocity.y = (std::sin(angleRad) * speed);

        particle.lifetime_remaining = sf::milliseconds(m_random.fromTo(0, 2000) + 1000);

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

    static util::Random m_random;
    std::vector<Particle> m_particles;
    sf::VertexArray m_vertArray;
    sf::Time m_lifetimeMax;
    sf::Vector2f m_position;
};

util::Random ParticleEmitter::m_random {};

//

int main()
{
    std::cout.imbue(std::locale("")); // this is only to put commas in big numbers

    if (!sf::Shader::isAvailable())
    {
        std::cout << "Shaders are NOT supported.  Your video card sucks." << std::endl;
    }

    auto willDrift { false };
    auto willCollide { false };
    auto collisionCount { std::size_t(0) };
    auto backgroundColor { sf::Color(46, 54, 60) };
    const auto countToAddPerSpawn { std::size_t(1000) };

    const sf::VideoMode videoMode { 1600, 1200, sf::VideoMode::getDesktopMode().bitsPerPixel };
    sf::RenderWindow window(videoMode, "Particles2", sf::Style::Fullscreen);
    const auto windowSize { window.getSize() };

    std::vector<ParticleEmitter> emitters;

    sf::Clock clock;
    sf::Clock statusClock;
    std::size_t framesPerStatusCounter { 0 };

    while (window.isOpen())
    {
        const sf::Vector2f mousePosition { sf::Mouse::getPosition(window) };

        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if ((sf::Event::Closed == event.type) || sf::Keyboard::Escape == event.key.code)
            {
                window.close();
                break;
            }

            if (sf::Event::KeyPressed != event.type)
            {
                continue;
            }

            if (sf::Keyboard::S == event.key.code)
            {
                emitters.emplace_back(mousePosition).add(countToAddPerSpawn);
            }
            else if (sf::Keyboard::R == event.key.code)
            {
                emitters.clear();
                willDrift = false;
            }
            else if (sf::Keyboard::D == event.key.code)
            {
                willDrift = !willDrift;
            }
            else if (sf::Keyboard::C == event.key.code)
            {
                willCollide = !willCollide;
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
                    particleCount += emitter.size();
                }

                std::cout << emitters.size() << " emitters, " << particleCount << " particles, "
                          << collisionCount << " collisions, " << framesPerSecond << "fps"
                          << std::endl;

                statusClock.restart();
                framesPerStatusCounter = 0;
            }
        }

        // update
        {
            // update position
            const sf::Time elapsed(clock.restart());
            for (auto & emitter : emitters)
            {
                emitter.update(elapsed, willDrift, mousePosition);
            }

            // handle collisions
            const auto emitterCount { emitters.size() };
            if (willCollide && (emitterCount > 1))
            {
                for (std::size_t outer { 0 }; outer < emitterCount; ++outer)
                {
                    ParticleEmitter & outerEmitter { emitters[outer] };

                    for (std::size_t inner { outer + 1 }; inner < emitterCount; ++inner)
                    {
                        ParticleEmitter & innerEmitter { emitters[inner] };

                        collisionCount += outerEmitter.handleCollisions(innerEmitter);
                    }
                }
            }
        }

        // draw
        {
            window.clear(backgroundColor);

            for (const auto & emitter : emitters)
            {
                window.draw(emitter);
            }

            window.display();
        }
    }

    return EXIT_SUCCESS;
}
