#ifndef SIMPLE_EFFECTS_GAME_HPP_INCLUDED
#define SIMPLE_EFFECTS_GAME_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"
#include "follower.hpp"
#include "resources.hpp"
#include "sound-player.hpp"
#include "steady-mover.hpp"
#include "util.hpp"
#include "wall-bouncer.hpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

//
// About vectors
//  Two types of vectors. Euclid uses x and y, polar uses angle and magnitude.
//  All vectors of both kinds have a hypotenuse that is often called the magnitude.
//  A normalized vector (sometimes called a unit vector) has a magnitude of 1.
//  To normalize a vector, simply divide it by its magnitude.
//  Velocity is a change to position. The hypotenuse can be magnitude or speed.

class Game
{
  public:
    Game();

    void run();

  private:
    void processEvents();
    void update(const float frameTimeSec);
    void render();

    void handleKeyPress(const sf::Keyboard::Key key);

    // void scrollValue(float amount);

    void reset();

  private:
    Resources m_resources;
    sf::RenderWindow m_window;
    sf::RenderStates m_states;
    bool m_willClear;
    sf::Vector2f m_windowSize;

    util::Random m_random;
    util::SoundPlayer m_audio;

    Context m_context;
    sf::Sprite m_bgSprite;

    entity::WallBouncerEffect m_wallBouncer;

    // Color Gradient
    // sf::VertexArray m_quadVerts;
    // sf::RenderTexture m_offScreenTexture;
    // sf::Image m_image;
};

#endif // SIMPLE_EFFECTS_GAME_HPP_INCLUDED
