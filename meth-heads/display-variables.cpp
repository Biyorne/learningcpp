// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "display-variables.hpp"

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
    {
        m_lazyScoreRectangle.setFillColor(m_constants.lazy_color);
        m_greedyScoreRectangle.setFillColor(m_constants.greedy_color);
        populateBoardCells();

        m_fpsText.setFillColor(sf::Color::Magenta);
    }

    void DisplayVariables::update(const float, const int lazyScore, const int greedyScore)
    {
        setScoreBarsHeight(lazyScore, greedyScore);
    }

    void DisplayVariables::setFps(const std::size_t framesPerSecond)
    {
        m_fpsText.setString(std::to_string(framesPerSecond));
        fit(m_fpsText, m_constants.fps_rect);
    }

    void DisplayVariables::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_lazyScoreRectangle, states);
        target.draw(m_greedyScoreRectangle, states);

        for (const sf::RectangleShape & rectangle : m_boardRectangles)
        {
            target.draw(rectangle);
        }

        target.draw(m_fpsText, states);
    }

    // TODO Minor optimization: Move some rect settings to the constructor
    void DisplayVariables::setScoreBarsHeight(const int lazyScore, const int greedyScore)
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

    void DisplayVariables::populateBoardCells()
    {
        sf::RectangleShape rectangle;

        rectangle.setSize(m_constants.cell_size);
        rectangle.setOutlineThickness(2.0f);
        rectangle.setFillColor(m_constants.cell_background_color);
        rectangle.setOutlineColor(m_constants.cell_line_color);

        for (std::size_t horiz(0); horiz < m_constants.horiz_cell_count; ++horiz)
        {
            for (std::size_t vert(0); vert < m_constants.vert_cell_count; ++vert)
            {
                const BoardPos_t boardPos(static_cast<int>(horiz), static_cast<int>(vert));

                assert((boardPos.x >= 0) && (boardPos.x < m_constants.horiz_cell_count));
                assert((boardPos.y >= 0) && (boardPos.y < m_constants.vert_cell_count));

                const sf::Vector2f windowPos(m_constants.boardPosToWindowPos(boardPos));

                assert(
                    !(windowPos.x < 0.0f) && (windowPos.x < m_constants.inner_window_rect.width));

                assert(
                    !(windowPos.y < 0.0f) && (windowPos.y < m_constants.inner_window_rect.height));

                rectangle.setPosition(windowPos);
                m_boardRectangles.push_back(rectangle);
            }
        }
    }
} // namespace methhead