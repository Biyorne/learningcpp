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
    inline void setSpriteRegion(sf::Sprite & sprite, const sf::FloatRect & region)
    {
        sprite.setPosition(region.left, region.top);

        float widthRatio(1.0f);
        if (sprite.getLocalBounds().width > 1.0f)
        {
            widthRatio = (region.width / sprite.getLocalBounds().width);
        }

        float heightRatio(1.0f);
        if (sprite.getLocalBounds().height > 1.0f)
        {
            heightRatio = (region.height / sprite.getLocalBounds().height);
        }

        sprite.setScale(widthRatio, heightRatio);
    }

    inline void setTextToRegion(sf::Text & text, const sf::FloatRect & region)
    {
        if (text.getString().isEmpty())
        {
            return;
        }

        // scale to fit inside region
        float scale(1.0f);
        if (text.getLocalBounds().width > text.getLocalBounds().height)
        {
            scale = (region.width / text.getLocalBounds().width);
        }
        else
        {
            scale = (region.height / text.getLocalBounds().height);
        }
        text.setScale(scale, scale);
        text.setOrigin(text.getLocalBounds().left, text.getLocalBounds().top);

        // position to center of cell (region)
        const sf::Vector2f finalTextSize(
            text.getGlobalBounds().width, text.getGlobalBounds().height);

        const sf::Vector2f regionPos(region.left, region.top);
        const sf::Vector2f regionSize(region.width, region.height);
        const sf::Vector2f regionPosCenter(regionPos + (regionSize * 0.5f));
        text.setPosition(regionPosCenter - (finalTextSize * 0.5f));
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
