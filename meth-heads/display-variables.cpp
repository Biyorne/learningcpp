// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "display-variables.hpp"

#include "meth-head.hpp"
#include "settings.hpp"
#include "sim-context.hpp"
#include "utils.hpp"

#include <cassert>
#include <iostream>

namespace methhead
{
    DisplayVariables::DisplayVariables(const sf::Vector2u & windowSize)
        : m_constants(windowSize)
        , m_lazyScoreRectangle()
        , m_greedyScoreRectangle()
        , m_quadVerts(sf::Quads)
        , m_lineVerts(sf::Lines)
        , m_fpsText(m_constants.default_text)
        , m_pickupText(m_constants.default_text)
        , m_pickupSprite(m_constants.loot_texture)
        , m_lazySprite(m_constants.lazy_texture)
        , m_greedySprite(m_constants.greedy_texture)
    {
        setupBoardDrawingVerts();
        setupColorsAndStyles();
        setupScales();
    }

    void DisplayVariables::setupColorsAndStyles()
    {
        m_pickupText.setStyle(sf::Text::Bold);

        m_fpsText.setFillColor(sf::Color::Magenta);
        m_pickupText.setFillColor(sf::Color::White);
        m_lazyScoreRectangle.setFillColor(m_constants.lazy_color);
        m_greedyScoreRectangle.setFillColor(m_constants.greedy_color);
    }

    void DisplayVariables::setupScales()
    {
        scale(m_pickupText, m_constants.cell_size);
        scale(m_pickupSprite, m_constants.cell_size);
        scale(m_lazySprite, m_constants.cell_size);
        scale(m_greedySprite, m_constants.cell_size);
    }

    void DisplayVariables::setupBoardDrawingVerts()
    {
        const sf::FloatRect rect(m_constants.board_rect);
        const float lft{ rect.left };
        const float rgt{ rect.left + rect.width };
        const float top{ rect.top };
        const float bot{ rect.top + rect.height };

        const sf::Color bgColor{ m_constants.cell_background_color };

        m_quadVerts.append(sf::Vertex(sf::Vector2f(lft, top), bgColor));
        m_quadVerts.append(sf::Vertex(sf::Vector2f(rgt, top), bgColor));
        m_quadVerts.append(sf::Vertex(sf::Vector2f(rgt, bot), bgColor));
        m_quadVerts.append(sf::Vertex(sf::Vector2f(lft, bot), bgColor));

        const sf::Color lineColor{ m_constants.cell_line_color };

        for (std::size_t horiz(0); horiz <= m_constants.cell_counts.x; ++horiz)
        {
            const float lineLeft(lft + (static_cast<float>(horiz) * m_constants.cell_size.x));

            m_lineVerts.append(sf::Vertex(sf::Vector2f(lineLeft, top), lineColor));
            m_lineVerts.append(sf::Vertex(sf::Vector2f(lineLeft, bot), lineColor));
        }

        for (std::size_t vert(0); vert <= m_constants.cell_counts.y; ++vert)
        {
            const float lineTop(top + (static_cast<float>(vert) * m_constants.cell_size.y));

            m_lineVerts.append(sf::Vertex(sf::Vector2f(lft, lineTop), lineColor));
            m_lineVerts.append(sf::Vertex(sf::Vector2f(rgt, lineTop), lineColor));
        }
    }

    void DisplayVariables::setFps(const std::size_t framesPerSecond)
    {
        m_fpsText.setString("fps=" + std::to_string(framesPerSecond));
        fit(m_fpsText, m_constants.fps_rect);
    }

    void DisplayVariables::draw(
        const SimContext & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_quadVerts, states);
        target.draw(m_lineVerts, states);

        target.draw(m_lazyScoreRectangle, states);
        target.draw(m_greedyScoreRectangle, states);

        target.draw(m_fpsText, states);

        for (const IPickupUPtr_t & pickup : context.pickups)
        {
            const sf::FloatRect windowRect(m_constants.boardPosToWindowRect(pickup->boardPos()));

            center(m_pickupSprite, windowRect);
            target.draw(m_pickupSprite, states);

            m_pickupText.setString(std::to_string(pickup->value()));
            fit(m_pickupText, windowRect);
            target.draw(m_pickupText, states);
        }

        for (const IActorUPtr_t & actor : context.actors)
        {
            const sf::FloatRect windowRect(m_constants.boardPosToWindowRect(actor->boardPos()));

            if (actor->motivation() == Motivation::lazy)
            {
                center(m_lazySprite, windowRect);
                target.draw(m_lazySprite, states);
            }
            else
            {
                center(m_greedySprite, windowRect);
                target.draw(m_greedySprite, states);
            }
        }
    }

    // TODO Minor optimization: Move some rect settings to the constructor
    void DisplayVariables::updateScoreBars(const int lazyScore, const int greedyScore)
    {
        // split the rect into two halves for lazy vs greedy
        sf::FloatRect rect(m_constants.score_rect);

        // make the width just under half to put a space between that looks nice
        sf::FloatRect lazyRect(rect);
        lazyRect.width *= 0.475f;

        sf::FloatRect greedyRect(lazyRect);
        greedyRect.left = ((rect.left + rect.width) - greedyRect.width);

        // shrink the height of the loser
        if (lazyScore != greedyScore)
        {
            const auto highScore(std::max(lazyScore, greedyScore));
            const auto lowScore(std::min(lazyScore, greedyScore));

            const float shrinkRatio(static_cast<float>(lowScore) / static_cast<float>(highScore));

            if (lazyScore < greedyScore)
            {
                lazyRect.height *= shrinkRatio;
            }
            else
            {
                greedyRect.height *= shrinkRatio;
            }
        }

        // position and size both rectangles
        m_lazyScoreRectangle.setPosition(
            lazyRect.left, ((rect.top + rect.height) - lazyRect.height));

        m_lazyScoreRectangle.setSize({ lazyRect.width, lazyRect.height });

        m_greedyScoreRectangle.setPosition(
            greedyRect.left, ((rect.top + rect.height) - greedyRect.height));

        m_greedyScoreRectangle.setSize({ greedyRect.width, greedyRect.height });
    }
} // namespace methhead