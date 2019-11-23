// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "display-variables.hpp"

#include "utils.hpp"

namespace methhead
{
    DisplayVariables::DisplayVariables(const sf::Vector2u & windowSize)
        : m_constants(windowSize)
        , m_lazyScoreRectangle()
        , m_greedyScoreRectangle()
    {
        m_lazyScoreRectangle.setFillColor(m_constants.lazy_color);
        m_greedyScoreRectangle.setFillColor(m_constants.greedy_color);
    }

    void DisplayVariables::update(const float, const int lazyScore, const int greedyScore)
    {
        setScoreBarsHeight(lazyScore, greedyScore);
    }

    void DisplayVariables::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_lazyScoreRectangle, states);
        target.draw(m_greedyScoreRectangle, states);

        for (const auto & [boardPos, rectangle] : m_constants.board_map)
        {
            target.draw(rectangle);
        }
    }

    // TODO Minor optimization: Move some bounds settings to the constructor
    void DisplayVariables::setScoreBarsHeight(const int lazyScore, const int greedyScore)
    {
        // split the bounds into two halves for lazy vs greedy
        sf::FloatRect bounds(m_constants.score_window_bounds);

        // make the width just under half to put a space between that looks nice
        sf::FloatRect lazyBounds(bounds);
        lazyBounds.width *= 0.475f;

        sf::FloatRect greedyBounds(lazyBounds);
        greedyBounds.left = ((bounds.left + bounds.width) - greedyBounds.width);

        // shrink the height of the loser
        if (lazyScore != greedyScore)
        {
            const auto highScore(std::max(lazyScore, greedyScore));
            const auto lowScore(std::min(lazyScore, greedyScore));

            const float shrinkRatio(static_cast<float>(lowScore) / static_cast<float>(highScore));

            if (lazyScore < greedyScore)
            {
                lazyBounds.height *= shrinkRatio;
            }
            else
            {
                greedyBounds.height *= shrinkRatio;
            }
        }

        // position and size both rectangles
        m_lazyScoreRectangle.setPosition(
            lazyBounds.left, ((bounds.top + bounds.height) - lazyBounds.height));

        m_lazyScoreRectangle.setSize({ lazyBounds.width, lazyBounds.height });

        m_greedyScoreRectangle.setPosition(
            greedyBounds.left, ((bounds.top + bounds.height) - greedyBounds.height));

        m_greedyScoreRectangle.setSize({ greedyBounds.width, greedyBounds.height });
    }

} // namespace methhead
