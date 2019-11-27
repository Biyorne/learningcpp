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
        , m_boardRectangles()
        , m_fpsText(m_constants.default_text)
        , m_pickupText(m_constants.default_text)
        , m_pickupSprite(m_constants.loot_texture)
        , m_actorSprite(m_constants.lazy_texture) // since both same, either works here
    {
        populateBoardDrawables();

        m_fpsText.setFillColor(sf::Color::Magenta);
        m_pickupText.setFillColor(sf::Color::White);
        m_lazyScoreRectangle.setFillColor(m_constants.lazy_color);
        m_greedyScoreRectangle.setFillColor(m_constants.greedy_color);

        m_pickupText.setStyle(sf::Text::Bold);

        scale(m_pickupText, m_constants.cell_size);
        scale(m_pickupSprite, m_constants.cell_size);
        scale(m_actorSprite, m_constants.cell_size);
    }

    void DisplayVariables::setFps(const std::size_t framesPerSecond)
    {
        m_fpsText.setString("fps=" + std::to_string(framesPerSecond));
        fit(m_fpsText, m_constants.fps_rect);
    }

    void DisplayVariables::drawBoardUsingRectangleShapes(sf::RenderTarget & target) const
    {
        for (const sf::RectangleShape & rectangle : m_boardRectangles)
        {
            target.draw(rectangle);
        }
    }

    void DisplayVariables::drawBorderUsingVerts(sf::RenderTarget & target) const
    {
        target.draw(&m_quadVerts[0], m_quadVerts.size(), sf::Quads);
        target.draw(&m_lineVerts[0], m_lineVerts.size(), sf::Lines);
    }

    void DisplayVariables::draw(
        const SimContext & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        if (context.settings.query(Settings::DrawBoardWithVerts))
        {
            drawBorderUsingVerts(target);
        }
        else
        {
            drawBoardUsingRectangleShapes(target);
        }

        target.draw(m_lazyScoreRectangle, states);
        target.draw(m_greedyScoreRectangle, states);

        target.draw(m_fpsText, states);

        for (const IPickupUPtr_t & pickup : context.pickups)
        {
            const sf::FloatRect windowRect(m_constants.boardPosToWindowRect(pickup->boardPos()));

            center(m_pickupSprite, windowRect);
            target.draw(m_pickupSprite, states);

            m_pickupText.setString(std::to_string(pickup->value()));
            center(m_pickupText, windowRect);
            target.draw(m_pickupText, states);
        }

        for (const IActorUPtr_t & actor : context.actors)
        {
            const sf::FloatRect windowRect(m_constants.boardPosToWindowRect(actor->boardPos()));

            if (actor->motivation() == Motivation::lazy)
            {
                m_actorSprite.setTexture(m_constants.lazy_texture, true);
            }
            else
            {
                m_actorSprite.setTexture(m_constants.greedy_texture, true);
            }

            center(m_actorSprite, windowRect);
            target.draw(m_actorSprite, states);
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

    void DisplayVariables::populateBoardDrawables()
    {
        sf::RectangleShape rectangle;

        rectangle.setSize(m_constants.cell_size);
        rectangle.setOutlineThickness(2.0f);
        rectangle.setFillColor(m_constants.cell_background_color);
        rectangle.setOutlineColor(m_constants.cell_line_color);

        for (int horiz(0); horiz < m_constants.cell_countsI.x; ++horiz)
        {
            for (int vert(0); vert < m_constants.cell_countsI.y; ++vert)
            {
                const BoardPos_t boardPos(static_cast<int>(horiz), static_cast<int>(vert));

                assert((boardPos.x >= 0) && (boardPos.x < m_constants.cell_countsI.x));
                assert((boardPos.y >= 0) && (boardPos.y < m_constants.cell_countsI.y));

                const sf::Vector2f windowPos(m_constants.boardPosToWindowPos(boardPos));

                assert(
                    !(windowPos.x < 0.0f) && (windowPos.x < m_constants.inner_window_rect.width));

                assert(
                    !(windowPos.y < 0.0f) && (windowPos.y < m_constants.inner_window_rect.height));

                rectangle.setPosition(windowPos);
                m_boardRectangles.push_back(rectangle);
            }
        }

        // create verts for a single rectangle of the whole board background
        // this quad has no lines to it, only a color filled into the center
        {
            const sf::FloatRect rect(m_constants.board_rect);

            // clang-format off
            m_quadVerts.push_back(sf::Vertex( { rect.left,                rect.top               }, m_constants.cell_background_color));
            m_quadVerts.push_back(sf::Vertex( {(rect.left + rect.width),  rect.top               }, m_constants.cell_background_color));
            m_quadVerts.push_back(sf::Vertex( {(rect.left + rect.width), (rect.top + rect.height)}, m_constants.cell_background_color));
            m_quadVerts.push_back(sf::Vertex( { rect.left,               (rect.top + rect.height)}, m_constants.cell_background_color));
            // clang-format on
        }

        // create verts that separate the individual cells, and also draw the border around the
        // rectangle/board/quad above
        {
            const sf::FloatRect rect(m_constants.board_rect);

            // clang-format off
            for (std::size_t horiz(0); horiz <= m_constants.cell_counts.x; ++horiz)
            {
                const float left(rect.left + (static_cast<float>(horiz) * m_constants.cell_size.x));

                m_lineVerts.push_back(sf::Vertex( { left, rect.top },                 m_constants.cell_line_color ));
                m_lineVerts.push_back(sf::Vertex( { left, (rect.top + rect.height) }, m_constants.cell_line_color ));
            }

            for (std::size_t vert(0); vert <= m_constants.cell_counts.y; ++vert)
            {
                const float top(rect.top + (static_cast<float>(vert) * m_constants.cell_size.y));

                m_lineVerts.push_back(sf::Vertex( { rect.left,               top }, m_constants.cell_line_color ));
                m_lineVerts.push_back(sf::Vertex( {(rect.left + rect.width), top }, m_constants.cell_line_color ));
            }
            // clang-format on
        }
    }
} // namespace methhead