#ifndef CASTLECRAWL_STATEPLAY_HPP_INCLUDED
#define CASTLECRAWL_STATEPLAY_HPP_INCLUDED
//
// state-play.hpp
//
#include "frames-per-sec.hpp"
#include "states.hpp"

#include <string>

#include <SFML/Graphics/RectangleShape.hpp>

// TODO REMOVE AFTER TESTING
//#include <SFML/Graphics.hpp>

namespace castlecrawl
{
    struct Context;

    class StatePlay : public StateBase
    {
      public:
        StatePlay(Context & context);

        void update(Context &, const float frameTimeSec) override;
        void handleEvent(Context & context, const sf::Event & event) override;
        void draw(const Context &, sf::RenderTarget &, sf::RenderStates) const override;

      private:
        FramesPerSecond m_fps;
        sf::RectangleShape m_windowOutline;
    };

    //
    // class StatePopup : public StatePlay
    //{
    //  public:
    //    StatePopup(Context & context);
    //
    //    bool handleEvent(Context & context, const sf::Event & event) override;
    //    void draw(const Context &, sf::RenderTarget &, const sf::RenderStates &) const override;
    //
    //  private:
    //    sf::Text m_text;
    //    sf::Texture m_paperTexture;
    //    sf::Sprite m_paperSprite;
    //    sf::RectangleShape m_backgroundFade;
    //};

} // namespace castlecrawl

#endif // CASTLECRAWL_STATEPLAY_HPP_INCLUDED
