// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// splash-coordinator.cpp
//
#include "state-splash.hpp"

#include "check-macros.hpp"
#include "settings.hpp"
#include "state-machine.hpp"
#include "util.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>

#include <SFML/Graphics.hpp>

namespace castlecrawl
{
    StateSplash::StateSplash()
        : StateBase(State::Splash)
        , m_texture()
        , m_sprite()
        , m_clock()
        , m_lightningTexture()
        , m_lightningSprite()
        , m_timing(Timing::InitialWait)
    {}

    void StateSplash::onEnter(Context & context)
    {
        const bool loadSuccess = m_texture.loadFromFile(
            (context.config.media_dir_path / "image" / "splash.png").string());

        M_CHECK_SS(loadSuccess, "failed to load splash.png");

        m_sprite.setTexture(m_texture);

        util::fit(m_sprite, (context.layout.windowSize() * 0.4f));
        util::centerInside(m_sprite, sf::FloatRect({ 0.0f, 0.0f }, context.layout.windowSize()));

        m_lightningTexture.loadFromFile(
            (context.config.media_dir_path / "image" / "lightning.png").string());

        m_lightningSprite.setTexture(m_lightningTexture);

        sf::FloatRect lightningRect{ m_sprite.getGlobalBounds() };
        lightningRect.width *= 0.333f;
        lightningRect.height *= 0.333f;

        util::fit(m_lightningSprite, util::size(lightningRect));
        m_lightningSprite.setPosition(util::position(lightningRect));
    }

    void StateSplash::handleEvent(Context & context, const sf::Event & event)
    {
        if ((sf::Event::KeyPressed == event.type) || (sf::Event::MouseButtonPressed == event.type))
        {
            context.state.setChangePending(State::Play);
        }
    }

    void StateSplash::update(Context &, const float)
    {
        switch (m_timing)
        {
            case Timing::InitialWait: {
                float alpha{ m_clock.getElapsedTime().asSeconds() * 100.0f };

                if (alpha > 255.0f)
                {
                    alpha = 255.0f;
                    m_timing = Timing::FirstStrike;
                    m_clock.restart();
                }

                const sf::Color color{ 255, 255, 255, static_cast<sf::Uint8>(alpha) };

                m_sprite.setColor(color);
                break;
            }

            case Timing::FirstStrike: {
                if (m_clock.getElapsedTime().asSeconds() > 0.15f)
                {
                    m_timing = Timing::ShortWait;
                    m_clock.restart();
                }

                break;
            }

            case Timing::ShortWait: {
                if (m_clock.getElapsedTime().asSeconds() > 1.5f)
                {
                    m_timing = Timing::SecondStrike;
                    m_clock.restart();
                }

                break;
            }

            case Timing::SecondStrike: {
                if (m_clock.getElapsedTime().asSeconds() > 0.25f)
                {
                    m_timing = Timing::LongWait;
                    m_clock.restart();
                }

                break;
            }

            case Timing::LongWait:
            default: {
                if (m_clock.getElapsedTime().asSeconds() > 3.5f)
                {
                    m_timing = Timing::FirstStrike;
                    m_clock.restart();
                }

                break;
            }
        }
    }

    void
        StateSplash::draw(const Context &, sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_sprite, states);

        if ((Timing::FirstStrike == m_timing) || (Timing::SecondStrike == m_timing))
        {
            target.draw(m_lightningSprite, states);
        }
    }

} // namespace castlecrawl
