#ifndef CASTLECRAWL_FRAMESPERSECOND_HPP_INCLUDED
#define CASTLECRAWL_FRAMESPERSECOND_HPP_INCLUDED
//
// frames-per-sec.hpp
//
#include "context.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>

namespace castlecrawl
{

    class FramesPerSecond : public sf::Drawable
    {
      public:
        FramesPerSecond();

        void reset(const Context &);
        void update();
        void draw(sf::RenderTarget &, sf::RenderStates) const override;

      private:
        sf::Clock m_clock;
        float m_frameCounter;
        sf::Text m_text;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_FRAMESPERSECOND_HPP_INCLUDED
