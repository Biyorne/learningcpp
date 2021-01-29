// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// frames-per-sec.cpp
//
#include "frames-per-sec.hpp"

#include "resources.hpp"
#include "settings.hpp"
#include "util.hpp"

#include <SFML/Graphics.hpp>

namespace castlecrawl
{

    FramesPerSecond::FramesPerSecond()
        : m_clock()
        , m_frameCounter(0.0f)
        , m_text()
    {}

    void FramesPerSecond::reset(const Context & context)
    {
        m_text.setFont(context.media.font());
        m_text.setFillColor(sf::Color(150, 150, 170));

        util::fitAndCenterInside(
            m_text, sf::FloatRect({ 0.0f, 0.0f }, { 1000.0, context.layout.mapCellSize().y }));

        m_text.setPosition(0.0f, (context.layout.windowSize().y - context.layout.mapCellSize().y));

        m_clock.restart();
    }

    void FramesPerSecond::update()
    {
        m_frameCounter += 1.0f;
        const float elapsedSec = m_clock.getElapsedTime().asSeconds();

        if (elapsedSec > 1.0f)
        {
            const int fps = static_cast<int>(m_frameCounter / elapsedSec);

            m_text.setString("FPS:" + std::to_string(fps));

            m_clock.restart();
            m_frameCounter = 0.0f;
        }
    }

    void FramesPerSecond::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_text, states);
    }

} // namespace castlecrawl
