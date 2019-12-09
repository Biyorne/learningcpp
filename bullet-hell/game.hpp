#ifndef BULLET_HELL_GAME_HPP_INCLUDED
#define BULLET_HELL_GAME_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class Game
{
  public:
    Game();
    void run();

  private:
    void handleEvents();
    void update();
    void draw();

  private:
    sf::RenderWindow m_window;
    sf::CircleShape m_player;
};

#endif // BULLET_HELL_GAME_HPP_INCLUDED
