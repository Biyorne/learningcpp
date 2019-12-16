// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "display-variables.hpp"

#include "meth-head.hpp"
#include "sim-context.hpp"
#include "utils.hpp"

#include <cassert>
#include <iostream>

namespace methhead
{
    DisplayVariables::DisplayVariables(const sf::Vector2u & windowSize)
        : m_constants(windowSize)
        , m_pickupText(m_constants.default_text)
        , m_pickupSprite(m_constants.loot_texture)
        , m_lazySprite(m_constants.lazy_texture)
        , m_greedySprite(m_constants.greedy_texture)
        , m_boardQuadVerts(sf::Quads)
        , m_boardLineVerts(sf::Lines)
    {
        setupBoardDrawingVerts();
        setupColorsAndStyles();
        setupSprites();
    }

    void DisplayVariables::setupColorsAndStyles()
    {
        m_pickupText.setFillColor(sf::Color::White);
        m_pickupText.setStyle(sf::Text::Bold);
    }

    void DisplayVariables::setupSprites()
    {
        scale(m_pickupText, m_constants.cell_size);
        scale(m_pickupSprite, m_constants.cell_size);
        scale(m_lazySprite, m_constants.cell_size);
        scale(m_greedySprite, m_constants.cell_size);
    }

    void DisplayVariables::setupBoardDrawingVerts()
    {
        m_boardQuadVerts.clear();

        // board background square of solid color
        appendQuadVerts(
            m_constants.board_rect, m_boardQuadVerts, m_constants.cell_background_color);

        // lines that border the board and separate the cells
        const float boardLft{ m_constants.board_rect.left };
        const float boardRgt{ m_constants.board_rect.left + m_constants.board_rect.width };
        const float boardTop{ m_constants.board_rect.top };
        const float boardBot{ m_constants.board_rect.top + m_constants.board_rect.height };

        const sf::Color lineColor{ m_constants.cell_line_color };

        for (std::size_t horiz(0); horiz <= m_constants.cell_counts.x; ++horiz)
        {
            const float lineLft(boardLft + (static_cast<float>(horiz) * m_constants.cell_size.x));

            m_boardLineVerts.append(sf::Vertex({ lineLft, boardTop }, lineColor));
            m_boardLineVerts.append(sf::Vertex({ lineLft, boardBot }, lineColor));
        }

        for (std::size_t vert(0); vert <= m_constants.cell_counts.y; ++vert)
        {
            const float lineTop(boardTop + (static_cast<float>(vert) * m_constants.cell_size.y));

            m_boardLineVerts.append(sf::Vertex({ boardLft, lineTop }, lineColor));
            m_boardLineVerts.append(sf::Vertex({ boardRgt, lineTop }, lineColor));
        }
    }

    void DisplayVariables::draw(
        const SimContext & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_boardQuadVerts, states);
        target.draw(m_boardLineVerts, states);

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
} // namespace methhead