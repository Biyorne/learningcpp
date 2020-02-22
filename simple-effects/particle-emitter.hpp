#ifndef SIMPLE_EFFECTS_PARTICLE_EMITTER_HPP_INCLUDED
#define SIMPLE_EFFECTS_PARTICLE_EMITTER_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"
#include "resources.hpp"
#include "steady-mover.hpp"
#include "util.hpp"

// Variations per particle
//  velocity vector (both direction and magnitude)
//  lifetime_ratio (color, scale)
//  rotation
//  speedLimit
//  scale

namespace entity
{
    class ParticleEffect : public EffectBase
    {
      public:
        ParticleEffect(const Context & context, const sf::Texture & texture)
            : EffectBase(makeSprite(context, texture, 0.01f, (context.window_size * 0.5f)))
            , m_velocity(
                  { context.random.fromTo(-50.0f, 50.0f), context.random.fromTo(-300.0f, -100.0f) })
            , m_acceleration({ 0.0f, 100.0f })
            , m_speedLimit(300.0f)
        {}

        virtual ~ParticleEffect() = default;

        void update(const Context &, const float frameTimeSec) override
        {
            m_velocity.vector += m_acceleration.updateDelta(frameTimeSec);
            m_velocity.vector = m_speedLimit.clamp(m_velocity.vector);

            m_sprite.scale(1.005f, 1.005f);

            m_sprite.move(m_velocity.updateDelta(frameTimeSec));
            m_sprite.rotate(1.0f);
        }

      private:
        Velocity m_velocity;
        Acceleration m_acceleration;
        MagnitudeClamp m_speedLimit;
    };

    //

    class ParticleEmitter : public sf::Drawable
    {
      public:
        ParticleEmitter()
            : m_particles()
            , m_isEmitting(false)
        {
            m_particles.reserve(1000);
        }

        void reset()
        {
            m_isEmitting = false;
            m_particles.clear();
        }

        void toggleEmission() { m_isEmitting = !m_isEmitting; }

        void update(const Context & context, const float frameTimeSec)
        {
            if (m_isEmitting)
            {
                m_particles.push_back(ParticleEffect(context, context.resources.particle_texture));
            }

            for (ParticleEffect & particle : m_particles)
            {
                particle.update(context, frameTimeSec);
            }
        }

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            for (const ParticleEffect & particle : m_particles)
            {
                target.draw(particle, states);
                target.draw(particle, sf::BlendAdd);
                target.draw(particle, sf::BlendAdd);
            }
        }

      private:
        std::vector<ParticleEffect> m_particles;
        bool m_isEmitting;
    };

} // namespace entity

#endif // SIMPLE_EFFECTS_PARTICLE_EMITTER_HPP_INCLUDED
