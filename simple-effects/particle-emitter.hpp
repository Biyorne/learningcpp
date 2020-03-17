#ifndef SIMPLE_EFFECTS_PARTICLE_EMITTER_HPP_INCLUDED
#define SIMPLE_EFFECTS_PARTICLE_EMITTER_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"
#include "movement.hpp"
#include "resources.hpp"
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
        ParticleEffect(
            const Context & context,
            const sf::Texture & texture,
            const Mover & mover,
            const sf::Vector2f & pos)
            : ParticleEffect(context, makeSprite(context, texture, (1.0f / 10.0f), pos), mover, pos)
        {}

        ParticleEffect(
            const Context & context,
            const sf::Sprite & sprite,
            const Mover & mover,
            const sf::Vector2f & pos)
            : EffectBase(sprite)
            , m_mover(mover)
            , m_shrinkRatio(context.random.fromTo(0.9f, 0.99f))
        {
            m_sprite.setPosition(pos);
        }

        virtual ~ParticleEffect() = default;

        void update(const Context &, const float frameTimeSec) override
        {
            m_sprite.move(m_mover.updateDelta(frameTimeSec));
            m_sprite.scale(0.975f, 0.975f);
        }

      private:
        Mover m_mover;
        float m_shrinkRatio;
    };

    //
    // Fountain
    // context.random.fromTo(-300.0f, 300.0f),
    //    context.random.fromTo(-300.0f, 300.0f) },
    //{ 0.0f, 100.0f },
    //    300.0f

    class ParticleEmitter : public sf::Drawable
    {
      public:
        ParticleEmitter()
            : m_particles()
            , m_spawnPos()
            , m_isEmitting(false)
            , m_elapsedSinceEmitSec(0.0f)
            , m_betweenEmitSec(0.0f)
        {
            m_particles.reserve(1000);
        }

        void reset()
        {
            m_elapsedSinceEmitSec = 0.0f;
            m_betweenEmitSec = 0.0f;
            m_isEmitting = false;
            m_particles.clear();
        }

        void toggleEmission(const sf::Vector2f & spawnPos)
        {
            m_spawnPos = spawnPos;
            m_isEmitting = !m_isEmitting;
        }

        void update(const Context & context, const float frameTimeSec)
        {
            m_elapsedSinceEmitSec += frameTimeSec;
            const bool isTimeToEmit(m_elapsedSinceEmitSec > m_betweenEmitSec);

            if (m_isEmitting && isTimeToEmit)
            {
                m_elapsedSinceEmitSec = 0.0f;
                m_betweenEmitSec = context.random.fromTo(0.01f, 0.25f);

                const float base{ 300.0f };
                const float range{ 250.0f };
                const float velHoriz{ context.random.fromTo((base - range), (base + range)) };
                float velVer{ context.random.fromTo((base - range), (base + range)) };
                velVer += range;

                const Mover mover({ velHoriz, velVer }, { velHoriz, -velVer }, base);

                m_particles.push_back(
                    ParticleEffect(context, context.resources.particle_texture, mover, m_spawnPos));
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
                // target.draw(particle, states);
                // target.draw(particle, sf::BlendAdd);
                target.draw(particle, sf::BlendAdd);
            }
        }

      private:
        std::vector<ParticleEffect> m_particles;
        sf::Vector2f m_spawnPos;
        bool m_isEmitting;
        float m_elapsedSinceEmitSec;
        float m_betweenEmitSec;
    };

} // namespace entity

#endif // SIMPLE_EFFECTS_PARTICLE_EMITTER_HPP_INCLUDED
