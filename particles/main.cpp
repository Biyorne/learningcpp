// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// main.cpp
//
#include <cstdlib>

#include <SFML/Graphics.hpp>

class ParticleEmitter : public sf::Drawable
{
public:
    explicit ParticleEmitter(const sf::Vector2f & position)
        : m_particles(5000)
        , m_vertices(sf::Points, m_particles.size())
        , m_lifetimeMax(sf::seconds(3.0f))
        , m_position(position)
        , m_willDrift(false)
    {}

    void position(const sf::Vector2f & position) { m_position = position; }

    void update(const sf::Time & elapsed, const sf::Vector2f & driftPos)
    {
        for (std::size_t i(0); i < m_particles.size(); ++i)
        {
            Particle & particle(m_particles[i]);
            sf::Vertex & vertex(m_vertices[i]);

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
        target.draw(m_vertices, states);
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

        vertex.color.r = randomColorValue();
        vertex.color.g = randomColorValue();
        vertex.color.b = randomColorValue();
    }

    unsigned char randomColorValue() const
    {
        return static_cast<unsigned char>((std::rand() % 255));
    }

    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;
    sf::Time m_lifetimeMax;
    sf::Vector2f m_position;
    bool m_willDrift;
};

int main()
{
    bool willDrift(false);

    sf::RenderWindow window(
        sf::VideoMode(1024, 768, sf::VideoMode::getDesktopMode().bitsPerPixel),
        "Particles",
        sf::Style::Fullscreen);

    std::vector<ParticleEmitter> emitters;

    sf::Clock clock;

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
            }
            else if (sf::Event::KeyPressed == event.type)
            {
                if (sf::Keyboard::Escape == event.key.code)
                {
                    window.close();
                }
                else if (sf::Keyboard::S == event.key.code)
                {
                    emitters.push_back(ParticleEmitter(mousePos));
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
        }

        // update
        const sf::Time elapsed(clock.restart());
        for (auto & emitter : emitters)
        {
            emitter.update(elapsed, mousePos);
        }

        // draw
        window.clear();

        for (const auto & emitter : emitters)
        {
            window.draw(emitter);
        }

        window.display();
    }

    return EXIT_SUCCESS;
}
