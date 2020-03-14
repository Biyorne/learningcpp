#ifndef SIMPLE_EFFECTS_GAME_HPP_INCLUDED
#define SIMPLE_EFFECTS_GAME_HPP_INCLUDED

#include "bloom-shader.hpp"
#include "context.hpp"
#include "effect-base.hpp"
#include "particle-emitter.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "sound-player.hpp"
#include "status-text.hpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

//
// About vectors
//  Two types of vectors. Euclid uses x and y, polar uses angle and magnitude.
//  All vectors of both kinds have a hypotenuse that is often called the magnitude.
//  A normalized vector (sometimes called a unit vector) has a magnitude of 1.
//  To normalize a vector, simply divide it by its magnitude.
//  velocity is a change to position. The hypotenuse can be magnitude or speed.

class Game
{
  public:
    Game();

    void run();

  private:
    void processEvents();
    void processEvent(const sf::Event & event);
    void update(const float frameTimeSec);
    void render();

    void reset();

  private:
    sf::RenderWindow m_window;
    util::BloomEffectHelper m_bloomWindow;

    Resources m_resources;
    util::Random m_random;
    util::SoundPlayer m_audio;
    Context m_context;

    bool m_willClear;
    sf::Sprite m_bgSprite;

    entity::IEffectUVec_t m_effects;

    entity::ParticleEmitter m_emitter;

    entity::StatusText m_statusText;

    float m_simTimeMult;

    // Color Gradient
    // sf::VertexArray m_quadVerts;
    // sf::RenderTexture m_offScreenTexture;
    // sf::Image m_image;
};

#endif // SIMPLE_EFFECTS_GAME_HPP_INCLUDED
