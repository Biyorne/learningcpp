// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// status-tex.cpp
//
#include "status-text.hpp"

namespace entity
{
    StatusText::StatusText(const Context & context)
        : m_text("", context.resources.font, 99)
        , m_elapsedTimeSec(0.0f)
        , m_postfix()
        , m_regionHeightRatio(0.03f)
        , m_region(
              0.0f,
              (context.window_size.y * (1.0f - m_regionHeightRatio)),
              context.window_size.x,
              (context.window_size.y * m_regionHeightRatio))
        , m_background(util::makeRectangleShape(m_region, sf::Color(0, 0, 0, 150), true))
    {}

    void StatusText::draw(sf::RenderTarget & target, sf::RenderStates states) const //-V813
    {
        target.draw(m_background, states);
        target.draw(m_text, states);
    }

    void StatusText::update(const Context &, const float frameTimeSec)
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

    float StatusText::calcDisplayableFps(const float frameTimeSec) const
    {
        if (!(frameTimeSec > 0.0f))
        {
            return 0.0f;
        }

        const float fpsActual(1.0f / frameTimeSec);
        return (std::round(fpsActual * 10.0f) / 10.0f);
    }
} // namespace entity
