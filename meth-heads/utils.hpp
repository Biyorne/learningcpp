#ifndef METH_HEAD_UTIL_HPP_INCLUDED
#define METH_HEAD_UTIL_HPP_INCLUDED

#include "display-constants.hpp"

#include <SFML/Graphics.hpp>

namespace sf
{
    inline bool operator<(const sf::Vector2i & L, const sf::Vector2i & R)
    {
        return std::tie(L.x, L.y) < std::tie(R.x, R.y);
    }
} // namespace sf

namespace methhead
{

    template <typename T>
    void placeInRegion(T & sfThing, const sf::FloatRect & region)
    {
        const auto localBounds(sfThing.getLocalBounds());

        if ((localBounds.width < 1.0f) || (localBounds.height < 1.0f))
        {
            return;
        }

        // scale to fit inside region
        float scale(1.0f);
        if (localBounds.width > localBounds.height)
        {
            scale = (region.width / localBounds.width);
        }
        else
        {
            scale = (region.height / localBounds.height);
        }
        sfThing.setScale(scale, scale);
        sfThing.setOrigin(localBounds.left, localBounds.top);

        // position to center of cell (region)
        const sf::Vector2f finalTextSize(
            sfThing.getGlobalBounds().width, sfThing.getGlobalBounds().height);

        const sf::Vector2f regionPos(region.left, region.top);
        const sf::Vector2f regionSize(region.width, region.height);
        const sf::Vector2f regionPosCenter(regionPos + (regionSize * 0.5f));
        sfThing.setPosition(regionPosCenter - (finalTextSize * 0.5f));
    }

    template <
        typename InputIterator,
        typename OutputIterator,
        typename UnaryFunction,
        typename PredicateFunction>
    OutputIterator transform_if(
        InputIterator sourceIter,
        InputIterator sourceIterLast,
        OutputIterator destIter,
        PredicateFunction willConvertAndStore,
        UnaryFunction convertFunction)
    {
        while (sourceIter != sourceIterLast)
        {
            if (willConvertAndStore(*sourceIter))
            {
                *destIter = convertFunction(*sourceIter);
                ++destIter;
            }

            ++sourceIter;
        }

        return destIter;
    }

    inline void scoreBarSetup(
        std::size_t lazyScore,
        std::size_t greedyScore,
        sf::RectangleShape & lazyScoreRectangle,
        sf::RectangleShape & greedyScoreRectangle,
        const DisplayConstants & displayConstants)
    {
        if (lazyScore > greedyScore)
        {
            if (0 == lazyScore)
            {
                lazyScore = 1;
            }

            const float heightRatio(
                static_cast<float>(greedyScore) / static_cast<float>(lazyScore));

            lazyScoreRectangle.setSize(sf::Vector2f(
                displayConstants.score_rectangle_width, displayConstants.window_size.y));

            greedyScoreRectangle.setSize(sf::Vector2f(
                displayConstants.score_rectangle_width,
                (lazyScoreRectangle.getSize().y * heightRatio)));
        }
        else
        {
            if (0 == greedyScore)
            {
                greedyScore = 1;
            }

            const float heightRatio(
                static_cast<float>(lazyScore) / static_cast<float>(greedyScore));

            greedyScoreRectangle.setSize(sf::Vector2f(
                displayConstants.score_rectangle_width, displayConstants.window_size.y));

            lazyScoreRectangle.setSize(sf::Vector2f(
                displayConstants.score_rectangle_width,
                (greedyScoreRectangle.getSize().y * heightRatio)));
        }

        lazyScoreRectangle.setPosition(
            displayConstants.score_region.left,
            (displayConstants.score_region.height - lazyScoreRectangle.getSize().y));

        greedyScoreRectangle.setPosition(
            (displayConstants.score_region.left + displayConstants.score_rectangle_width),
            (displayConstants.score_region.height - greedyScoreRectangle.getSize().y));
    }

} // namespace methhead

#endif // METH_HEAD_UTIL_HPP_INCLUDED