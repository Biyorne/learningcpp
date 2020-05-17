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
            const sf::Vector2f & pos);

        ParticleEffect(
            const Context & context,
            const sf::Sprite & sprite,
            const Mover & mover,
            const sf::Vector2f & pos);

        virtual ~ParticleEffect() = default;

        void update(const Context &, const float frameTimeSec) override;

      private:
        Mover m_mover;
        float m_shrinkRatio;
    };

    //=========================================
    // Particle Emitter -- Separate from Particle Effect?
    //=========================================

    //
    // Fountain
    // context.random.fromTo(-300.0f, 300.0f),
    //    context.random.fromTo(-300.0f, 300.0f) },
    //{ 0.0f, 100.0f },
    //    300.0f

    class ParticleEmitter : public sf::Drawable
    {
      public:
        ParticleEmitter(const Context & context);

        void reset();

        void toggleEmission(const sf::Vector2f & spawnPos);

        bool isEmitting() const { return m_isEmitting; }

        void update(const Context & context, const float frameTimeSec);

        void draw(sf::RenderTarget & target, sf::RenderStates) const override;

      private:
        std::vector<ParticleEffect> m_particles;
        sf::Vector2f m_spawnPos;
        sf::Sprite m_bgHighlight;
        bool m_isEmitting;
        float m_elapsedSinceEmitSec;
        float m_betweenEmitSec;
    };

} // namespace entity

#endif // SIMPLE_EFFECTS_PARTICLE_EMITTER_HPP_INCLUDED
