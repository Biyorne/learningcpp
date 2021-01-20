#ifndef CASTLECRAWL_STATEPLAY_HPP_INCLUDED
#define CASTLECRAWL_STATEPLAY_HPP_INCLUDED
//
// state-play.hpp
//
#include "frames-per-sec.hpp"
#include "states.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

namespace castlecrawl
{
    struct Context;

    class StatePlay : public StateBase
    {
      public:
        StatePlay(Context & context);

        void update(Context &, const float frameTimeSec) override;
        bool handleEvent(Context & context, const sf::Event & event) override;
        void draw(const Context &, sf::RenderTarget &, const sf::RenderStates &) const override;

      private:
        FramesPerSecond m_fps;
        sf::RectangleShape m_windowOutline;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_STATEPLAY_HPP_INCLUDED
