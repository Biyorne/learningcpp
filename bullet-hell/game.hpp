#ifndef BULLET_HELL_GAME_HPP_INCLUDED
#define BULLET_HELL_GAME_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class Game
{
  public:
    Game();

    void run();

  private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

  private:
    sf::RenderWindow m_window;
    sf::Texture m_playerTexture;
    sf::Sprite m_playerSprite;
    bool m_isMovingUp;
    bool m_isMovingDown;
    bool m_isMovingLeft;
    bool m_isMovingRight;
};

#endif // BULLET_HELL_GAME_HPP_INCLUDED
