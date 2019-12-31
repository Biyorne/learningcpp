#ifndef BULLET_HELL_GAME_HPP_INCLUDED
#define BULLET_HELL_GAME_HPP_INCLUDED

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

//
// About vectors
//  Two types of vectors. Euclid uses x and y, polar uses angle and magnitude.
//  All vectors of both kinds have a hypotenuse that is often called the magnitude.
//  A normalized vector (sometimes called a unit vector) has a magnitude of 1.
//  To normalize a vector, simply divide it by its magnitude.
//  Velocity is a change to position. The hypotenuse can be magnitude or speed.

struct Mover
{
    explicit Mover(
        const sf::Vector2f & pos = sf::Vector2f(0.0f, 0.0f),
        const sf::Vector2f & vel = sf::Vector2f(0.0f, 0.0f),
        const sf::Vector2f & acc = sf::Vector2f(0.0f, 0.0f))
        : position(pos)
        , velocity(vel)
        , accelerations()
    {
        accelerations.push_back(acc);
    }

    void move()
    {
        for (const sf::Vector2f & acc : accelerations)
        {
            velocity += acc;
        }

        position += velocity;
    }

    sf::Vector2f position;                   // the result we wanted in the first place
    sf::Vector2f velocity;                   // change to position
    std::vector<sf::Vector2f> accelerations; // changes to velocity
};

//

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
    sf::RenderWindow m_window;
    sf::Texture m_bgTexture;
    sf::Texture m_warnTexture;
    sf::RenderStates m_states;
    bool m_willClear;
    bool m_willBlendAdd;
    sf::Vector2f m_windowSize;

    sf::Sprite m_bgSprite;
    std::vector<sf::Sprite> m_warnSprites;
    float m_bgRotateSpeed;
    sf::Music m_music;
};

#endif // BULLET_HELL_GAME_HPP_INCLUDED
