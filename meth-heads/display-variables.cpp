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
        , m_fpsText(m_constants.default_text)
        , m_pickupText(m_constants.default_text)
        , m_pickupSprite(m_constants.loot_texture)
        , m_lazySprite(m_constants.lazy_texture)
        , m_greedySprite(m_constants.greedy_texture)
        , m_pickupImageVertArray()
        , m_lazyImageVertArray()
        , m_greedyImageVertArray()
        , m_boardQuadVerts(sf::Quads)
        , m_boardLineVerts(sf::Lines)
        , m_scoreQuadVerts(sf::Quads)
        , m_allPickupImageVerts(sf::Quads)
        , m_allLazyImageVerts(sf::Quads)
        , m_allGreedyImageVerts(sf::Quads)
    {
        setupImageVertArray();
        setupBoardDrawingVerts();
        setupColorsAndStyles();
        setupScales();
    }

    void DisplayVariables::setupImageVertArray(
        ImageVertArray_t & vertsArray, const sf::Texture & texture) const
    {
        vertsArray[0].position = sf::Vector2f(0.0f, 0.0f);
        vertsArray[1].position = sf::Vector2f(m_constants.cell_size.x, 0.0f);
        vertsArray[2].position = m_constants.cell_size;
        vertsArray[3].position = sf::Vector2f(0.0f, m_constants.cell_size.y);

        const sf::Vector2f imageSize{ sf::Vector2f(texture.getSize()) };

        vertsArray[0].texCoords = sf::Vector2f(0.0f, 0.0f);
        vertsArray[1].texCoords = sf::Vector2f(imageSize.x, 0.0f);
        vertsArray[2].texCoords = imageSize;
        vertsArray[3].texCoords = sf::Vector2f(0.0f, imageSize.y);
    }

    void DisplayVariables::setupImageVertArray()
    {
        setupImageVertArray(m_pickupImageVertArray, m_constants.loot_texture);
        setupImageVertArray(m_lazyImageVertArray, m_constants.lazy_texture);
        setupImageVertArray(m_greedyImageVertArray, m_constants.greedy_texture);
    }

    void DisplayVariables::setupColorsAndStyles()
    {
        m_fpsText.setFillColor(sf::Color::Magenta);
        m_pickupText.setFillColor(sf::Color::White);
        m_pickupText.setStyle(sf::Text::Bold);
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

            m_boardLineVerts.append(sf::Vertex(sf::Vector2f(lineLft, boardTop), lineColor));
            m_boardLineVerts.append(sf::Vertex(sf::Vector2f(lineLft, boardBot), lineColor));
        }

        for (std::size_t vert(0); vert <= m_constants.cell_counts.y; ++vert)
        {
            const float lineTop(boardTop + (static_cast<float>(vert) * m_constants.cell_size.y));

            m_boardLineVerts.append(sf::Vertex(sf::Vector2f(boardLft, lineTop), lineColor));
            m_boardLineVerts.append(sf::Vertex(sf::Vector2f(boardRgt, lineTop), lineColor));
        }
    }

    void DisplayVariables::updatePerStatus(
        const std::size_t framesPerSecond, const int lazyScore, const int greedyScore)
    {
        updateFps(framesPerSecond);
        updateScoreBars(lazyScore, greedyScore);
    }

    void DisplayVariables::updateFps(const std::size_t framesPerSecond)
    {
        m_fpsText.setString("fps=" + std::to_string(framesPerSecond));
        fit(m_fpsText, m_constants.fps_rect);
    }

    void DisplayVariables::draw(
        const SimContext & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_boardQuadVerts, states);
        target.draw(m_boardLineVerts, states);
        target.draw(m_scoreQuadVerts, states);
        target.draw(m_fpsText, states);

        if (context.settings.query(Settings::DrawImagesWithVerts))
        {
            drawUsingVerts(context, target, states);
        }
        else
        {
            drawUsingSprites(context, target, states);
        }
    }

    void DisplayVariables::drawUsingSprites(
        const SimContext & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
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

    void DisplayVariables::drawUsingVerts(
        const SimContext & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        states.texture = &m_constants.lazy_texture;
        target.draw(m_allLazyImageVerts, states);

        states.texture = &m_constants.greedy_texture;
        target.draw(m_allGreedyImageVerts, states);

        states.texture = &m_constants.loot_texture;
        target.draw(m_allPickupImageVerts, states);

        states.texture = nullptr;
        for (const IPickupUPtr_t & pickup : context.pickups)
        {
            const sf::FloatRect windowRect(m_constants.boardPosToWindowRect(pickup->boardPos()));

            m_pickupText.setString(std::to_string(pickup->value()));
            fit(m_pickupText, windowRect);
            target.draw(m_pickupText, states);
        }
    }

    void DisplayVariables::appendImageVerts(
        const sf::Vector2f pos, ImageVertArray_t & srcVerts, sf::VertexArray & dstVerts) const
    {
        srcVerts[0].position += pos;
        srcVerts[1].position += pos;
        srcVerts[2].position += pos;
        srcVerts[3].position += pos;

        dstVerts.append(srcVerts[0]);
        dstVerts.append(srcVerts[1]);
        dstVerts.append(srcVerts[2]);
        dstVerts.append(srcVerts[3]);

        srcVerts[0].position -= pos;
        srcVerts[1].position -= pos;
        srcVerts[2].position -= pos;
        srcVerts[3].position -= pos;
    }

    void DisplayVariables::updateScoreBars(const int lazyScore, const int greedyScore)
    {
        sf::FloatRect bothRect(m_constants.score_rect);
        const float bothRight{ bothRect.left + bothRect.width };
        const float bothBottom{ bothRect.top + bothRect.height };

        // split the rect into two halves for lazy vs greedy

        // make the width just under half to put a space between that looks nice
        sf::FloatRect lazyRect(bothRect);
        lazyRect.width *= 0.475f;

        sf::FloatRect greedyRect(lazyRect);
        greedyRect.left = (bothRight - greedyRect.width);

        // shrink the height of the loser
        if (lazyScore != greedyScore)
        {
            const auto highScore{ std::max(lazyScore, greedyScore) };
            const auto lowScore{ std::min(lazyScore, greedyScore) };
            const float shrinkRatio{ static_cast<float>(lowScore) / static_cast<float>(highScore) };

            if (lazyScore < greedyScore)
            {
                lazyRect.height *= shrinkRatio;
            }
            else
            {
                greedyRect.height *= shrinkRatio;
            }
        }

        m_scoreQuadVerts.clear();

        const sf::Vector2f lazyPos{ lazyRect.left, (bothBottom - lazyRect.height) };
        const sf::Vector2f lazySize{ lazyRect.width, lazyRect.height };
        appendQuadVerts(lazyPos, lazySize, m_scoreQuadVerts, m_constants.lazy_color);

        const sf::Vector2f greedyPos{ greedyRect.left, (bothBottom - greedyRect.height) };
        const sf::Vector2f greedySize{ greedyRect.width, greedyRect.height };
        appendQuadVerts(greedyPos, greedySize, m_scoreQuadVerts, m_constants.greedy_color);
    }

    void DisplayVariables::updatePerFrame(const SimContext & context, const float elapsedMs)
    {
        m_allLazyImageVerts.clear();
        m_allGreedyImageVerts.clear();

        for (const IActorUPtr_t & actor : context.actors)
        {
            if (actor->motivation() == Motivation::lazy)
            {
                appendImageVerts(
                    m_constants.boardPosToWindowPos(actor->boardPos()),
                    m_lazyImageVertArray,
                    m_allLazyImageVerts);
            }
            else
            {
                appendImageVerts(
                    m_constants.boardPosToWindowPos(actor->boardPos()),
                    m_greedyImageVertArray,
                    m_allGreedyImageVerts);
            }
        }

        m_allPickupImageVerts.clear();

        for (const IPickupUPtr_t & pickup : context.pickups)
        {
            appendImageVerts(
                m_constants.boardPosToWindowPos(pickup->boardPos()),
                m_pickupImageVertArray,
                m_allPickupImageVerts);
        }
    }
} // namespace methhead