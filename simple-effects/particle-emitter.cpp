// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// particle-emitter.cpp
//
#include "particle-emitter.hpp"

namespace entity
{
    ParticleEffect::ParticleEffect(
        const Context & context,
        const sf::Texture & texture,
        const Mover & mover,
        const sf::Vector2f & pos)
        : ParticleEffect(context, makeSprite(context, texture, (1.0f / 10.0f), pos), mover, pos)
    {}

    ParticleEffect::ParticleEffect(
        const Context & context,
        const sf::Sprite & sprite,
        const Mover & mover,
        const sf::Vector2f & pos)
        : EffectBase(sprite)
        , m_mover(mover)
        , m_shrinkRatio(context.random.fromTo(0.965f, 0.985f))
    {
        m_sprite.setPosition(pos);
    }

    void ParticleEffect::update(const Context &, const float frameTimeSec)
    {
        m_sprite.move(m_mover.updateDelta(frameTimeSec));
        m_sprite.scale(m_shrinkRatio, m_shrinkRatio);
    }

    //=========================================
    // Particle Emitter -- Separate from Particle Effect?
    //=========================================

    ParticleEmitter::ParticleEmitter(const Context & context)
        : m_particles()
        , m_spawnPos()
        , m_bgHighlight() //(context.resources.highlight_texture)
        , m_isEmitting(false)
        , m_elapsedSinceEmitSec(0.0f)
        , m_betweenEmitSec(0.0f)
    {
        m_particles.reserve(1000);

        m_bgHighlight.setScale(
            context.window_size / sf::Vector2f(context.resources.backdrop_texture.getSize()));

        m_bgHighlight.setColor(sf::Color(255, 255, 255, 0));
    }

    void ParticleEmitter::reset()
    {
        m_elapsedSinceEmitSec = 0.0f;
        m_betweenEmitSec = 0.0f;
        m_isEmitting = false;
        m_bgHighlight.setColor(sf::Color(255, 255, 255, 0));
        m_particles.clear();
    }

    void ParticleEmitter::toggleEmission(const sf::Vector2f & spawnPos)
    {
        m_spawnPos = spawnPos;
        m_isEmitting = !m_isEmitting;
    }

    void ParticleEmitter::update(const Context & context, const float frameTimeSec)
    {
        m_elapsedSinceEmitSec += frameTimeSec;
        const bool isTimeToEmit(m_elapsedSinceEmitSec > m_betweenEmitSec);

        m_bgHighlight.setColor(m_bgHighlight.getColor() - sf::Color(0, 0, 0, 5));

        if (m_isEmitting && isTimeToEmit)
        {
            m_elapsedSinceEmitSec = 0.0f;
            m_betweenEmitSec = context.random.fromTo(0.025f, 0.5f);

            // clang-format off
            MoverRatios ratios{
                BaseRatios_t(0.5f, 1.3f),
                BaseRatios_t(1.0f, -0.9f),
                2.85f };

            MoverRanges ranges{0.5f, 0.25f};
            // clang-format on

            const Mover mover = MoverFactory::makeFromRanges(context, 150.0f, ratios, ranges);

            m_particles.push_back(
                ParticleEffect(context, context.resources.particle_texture, mover, m_spawnPos));

            m_bgHighlight.setColor(m_bgHighlight.getColor() + sf::Color(0, 0, 0, 32));
        }

        for (ParticleEffect & particle : m_particles)
        {
            particle.update(context, frameTimeSec);
        }
    }

    void ParticleEmitter::draw(sf::RenderTarget & target, sf::RenderStates) const
    {
        target.draw(m_bgHighlight, sf::BlendAdd);
        for (const ParticleEffect & particle : m_particles)
        {
            // target.draw(particle, states);
            // target.draw(particle, sf::BlendAdd);
            target.draw(particle, sf::BlendAdd);
        }
    }

} // namespace entity
