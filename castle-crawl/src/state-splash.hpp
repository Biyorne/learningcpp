#ifndef CASTLECRAWL_STATESPLASH_HPP_INCLUDED
#define CASTLECRAWL_STATESPLASH_HPP_INCLUDED
//
// state-splash.hpp
//
#include "states.hpp"

#include <SFML/Graphics.hpp>

namespace castlecrawl
{
    class StateSplash : public StateBase
    {
      public:
        StateSplash();

      protected:
        void handleEvent(Context &, const sf::Event &) override;
        void draw(const Context &, sf::RenderTarget &, const sf::RenderStates &) const override;
        void onEnter(Context &) override;

      protected:
        sf::Texture m_texture;
        sf::Sprite m_sprite;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_STATESPLASH_HPP_INCLUDED
