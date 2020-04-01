#ifndef SIMPLE_EFFECTS_STATUS_TEXT_HPP_INCLUDED
#define SIMPLE_EFFECTS_STATUS_TEXT_HPP_INCLUDED

#include "context.hpp"
#include "resources.hpp"

#include <sstream>

#include <SFML/Graphics.hpp>

namespace entity
{

    // struct IEffect : public sf::Drawable
    //{
    //    virtual ~IEffect() = default;
    //
    //    virtual bool isFinished() const = 0;
    //    void draw(sf::RenderTarget & target, sf::RenderStates states) const override = 0;
    //    virtual void update(const Context & context, const float frameTimeSec) = 0;
    //    virtual void handleEvent(const Context & context, const sf::Event & event) = 0;
    //};

    class StatusText : public sf::Drawable
    {
      public:
        StatusText(const Context & context)
            : m_text("", context.resources.font, 99)
            , m_elapsedTimeSec(0.0f)
            , m_postfix()
            , m_regionHeightRatio(0.1f)
            , m_region(
                  0.0f,
                  (context.window_size.y * (1.0f - m_regionHeightRatio)),
                  context.window_size.x,
                  (context.window_size.y * m_regionHeightRatio))
            , m_background(util::makeRectangleShape(m_region, sf::Color(0, 0, 0, 150), true))
        {}

        virtual ~StatusText() = default;

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override //-V813
        {
            target.draw(m_background, states);
            target.draw(m_text, states);
        }

        void update(const Context &, const float frameTimeSec)
        {
            m_elapsedTimeSec += frameTimeSec;
            if (m_elapsedTimeSec < 0.25f)
            {
                return;
            }
            else
            {
                m_elapsedTimeSec = 0.0f;
            }

            std::ostringstream ss;
            ss << "FPS: " << std::setw(4) << int(calcDisplayableFps(frameTimeSec)) << "   "
               << m_postfix;

            m_text.setString(ss.str());

            util::scaleTo(m_text, util::size(m_region));
            util::fixSfTextPos(m_text);

            m_text.setPosition(util::position(m_region));
        }

        void setPostfix(const std::string & postfix) { m_postfix = postfix; }

      private:
        float calcDisplayableFps(const float frameTimeSec) const
        {
            if (!(frameTimeSec > 0.0f))
            {
                return 0.0f;
            }

            const float fpsActual(1.0f / frameTimeSec);
            return (std::round(fpsActual * 10.0f) / 10.0f);
        }

      private:
        sf::Text m_text;
        float m_elapsedTimeSec;
        std::string m_postfix;
        float m_regionHeightRatio;
        sf::FloatRect m_region;
        sf::RectangleShape m_background;
    };

} // namespace entity
#endif // SIMPLE_EFFECTS_STATUS_TEXT_HPP_INCLUDED
