#ifndef BULLET_HELL_GAME_HPP_INCLUDED
#define BULLET_HELL_GAME_HPP_INCLUDED

#include <SFML/Graphics.hpp>

// struct Thing
//{
//
//    void update(const float frameTimeSec);
//    void draw(sf::RenderTarget & target, sf::RenderStates states);
//
//    sf::Sprite sprite;
//    float m_rotateSpeed = 120.0f;
//};

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

    void changeSpeed(const sf::Keyboard::Key key, float & speed)
    {
        if (key == sf::Keyboard::Right)
        {
            speed *= 1.15f;
        }
        else
        {
            speed *= 0.85f;
        }
    }

    void reset();

  private:
    sf::RenderWindow m_window;
    sf::Texture m_texture;
    sf::RenderStates m_states;
    bool m_willClear;
    bool m_willBlendAdd;

    sf::Sprite m_sprite;
    float m_rotateSpeed;
    float m_moveSpeed;
};

#endif // BULLET_HELL_GAME_HPP_INCLUDED
