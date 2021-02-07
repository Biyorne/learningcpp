#ifndef CASTLECRAWL_STATESPLASH_HPP_INCLUDED
#define CASTLECRAWL_STATESPLASH_HPP_INCLUDED
//
// state-splash.hpp
//
#include "states.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace castlecrawl
{
    class StateSplash : public StateBase
    {
      public:
        StateSplash();

      protected:
        void update(Context &, const float) override;
        void handleEvent(Context &, const sf::Event &) override;
        void draw(const Context &, sf::RenderTarget &, sf::RenderStates) const override;
        void onEnter(Context &) override;

      protected:
        enum class Timing
        {
            InitialWait = 0,
            FirstStrike,
            ShortWait,
            SecondStrike,
            LongWait
        };

        sf::Texture m_texture;
        sf::Sprite m_sprite;
        sf::Clock m_clock;
        sf::Texture m_lightningTexture;
        sf::Sprite m_lightningSprite;
        Timing m_timing;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_STATESPLASH_HPP_INCLUDED
