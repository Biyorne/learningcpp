#ifndef SIMPLE_EFFECTS_BASE_HPP_INCLUDED
#define SIMPLE_EFFECTS_BASE_HPP_INCLUDED

#include "context.hpp"
#include "steady-mover.hpp"

#include <SFML/Graphics.hpp>

struct EffectBase : public sf::Drawable
{
    explicit EffectBase(const Mover & mov = Mover(), const bool willFen = false)
        : sprite()
        , mover(mov)
        , will_fence(willFen)
        , will_draw_again_add(false)
    {}

    explicit EffectBase(
        const sf::Texture & tex, const Mover & mov = Mover(), const bool willFen = false)
        : EffectBase(mov, willFen)
    {
        sprite.setTexture(tex, true);
        util::setOrigin2Center(sprite);
    }

    virtual ~EffectBase() = default;

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override
    {
        target.draw(sprite, states);

        if (will_draw_again_add)
        {
            target.draw(sprite, sf::BlendAdd);
        }
    }

    virtual void update(const float elapsedTimeSec, const Context & context)
    {
        mover.update(elapsedTimeSec);
        sprite.setPosition(mover.position);
        handleFencing(context);
    }

    void handleFencing(const Context & context)
    {
        if (!will_fence)
        {
            return;
        }

        const sf::Vector2f posCorr{ calcFencePosCorrection(
            sprite.getGlobalBounds(), context.window_rect) };

        sprite.move(posCorr);

        if ((posCorr.x > 0.0f) || (posCorr.x < 0.0f))
        {
            mover.velocity.x *= -1.0f;
        }

        if ((posCorr.y > 0.0f) || (posCorr.y < 0.0f))
        {
            mover.velocity.y *= -1.0f;
        }
    }

    static sf::Vector2f
        calcFencePosCorrection(const sf::FloatRect & bounds, const sf::FloatRect & fence)
    {
        sf::Vector2f posAdj(0.0f, 0.0f);

        if (bounds.left < fence.left)
        {
            posAdj.x += (fence.left - bounds.left);
        }

        if (bounds.top < fence.top)
        {
            posAdj.y += (fence.top - bounds.top);
        }

        const float objRight{ util::right(bounds) };
        const float fenceRight{ util::right(fence) };
        if (objRight > fenceRight)
        {
            posAdj.x -= (objRight - fenceRight);
        }

        const float objBottom{ util::bottom(bounds) };
        const float fenceBottom{ util::bottom(fence) };
        if (objBottom > fenceBottom)
        {
            posAdj.y -= (objBottom - fenceBottom);
        }

        return posAdj;
    }

    bool will_draw_again_add;
    bool will_fence;
    sf::Sprite sprite;
    Mover mover;
};

#endif // SIMPLE_EFFECTS_BASE_HPP_INCLUDED
