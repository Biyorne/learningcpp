#ifndef CASTLECRAWL_SPLASHCOORDINATOR_HPP_INCLUDED
#define CASTLECRAWL_SPLASHCOORDINATOR_HPP_INCLUDED
//
// splash-coordinator.hpp
//
#include "settings.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

namespace castlecrawl
{
    class SplashCoordinator
    {
      public:
        SplashCoordinator();

        void run(const GameConfig & config);

      protected:
        void loadAll(const GameConfig & config);
        void openWindow();
        void handleEvents();
        void handleEvent(const sf::Event & event);
        void update(const float elapsedTimeSec);
        void draw();

      protected:
        sf::RenderWindow m_window;
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        GameConfig m_config;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_SPLASHCOORDINATOR_HPP_INCLUDED
