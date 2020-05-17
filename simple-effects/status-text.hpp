#ifndef SIMPLE_EFFECTS_STATUS_TEXT_HPP_INCLUDED
#define SIMPLE_EFFECTS_STATUS_TEXT_HPP_INCLUDED

#include "context.hpp"
#include "resources.hpp"
#include "util.hpp"

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
        StatusText(const Context & context);

        virtual ~StatusText() = default;

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

        void update(const Context &, const float frameTimeSec);

        void setPostfix(const std::string & postfix) { m_postfix = postfix; }

      private:
        float calcDisplayableFps(const float frameTimeSec) const;

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
