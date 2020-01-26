#ifndef SIMPLE_EFFECTS_FOLLOWER_HPP_INCLUDED
#define SIMPLE_EFFECTS_FOLLOWER_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"
#include "steady-mover.hpp"
#include "util.hpp"

struct FollowerEffect : public EffectBase
{
    FollowerEffect(
        const float speed,
        const sf::Texture & followTex,
        const sf::Vector2f & followPos,
        const sf::Texture & leadTex,
        const sf::Vector2f & leadPos)
        : EffectBase(followTex, Mover(speed, followPos, {}, {}, speed))
        , leader_sprite(leadTex)
    {
        util::setOrigin2Center(leaderSprite());
        leaderSprite().setPosition(leadPos);

        followerSprite().setScale(5.0f, 5.0f);
        leaderSprite().setScale(0.5f, 0.5f);
    }

    virtual ~FollowerEffect() = default;

    sf::Sprite & followerSprite() { return sprite; }
    sf::Sprite & leaderSprite() { return leader_sprite; }

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override
    {
        EffectBase::draw(target, states);
        // target.draw(leader_sprite, states);
    }

    void update(const float elapsedTimeSec, const Context & context) override
    {
        // Accelerate rabbit
        const sf::Vector2f posDiff(context.mouse_pos - mover.position);
        const float distance(std::sqrt((posDiff.x * posDiff.x) + (posDiff.y * posDiff.y)));
        float rotate{ 2.0f };
        if (distance > 1.0f)
        {
            const sf::Vector2f posDiffNorm(posDiff / distance);
            mover.velocity += ((posDiffNorm * mover.speed) * elapsedTimeSec);
            rotate += ((distance * 0.05f));
        }
        else
        {
            mover.position = context.mouse_pos;
            mover.velocity = { 0.0f, 0.0f };
        }
        followerSprite().rotate(rotate);

        EffectBase::update(elapsedTimeSec, context);

        // Set carrot to mouse
        leaderSprite().setPosition(context.mouse_pos);
    }

    sf::Sprite leader_sprite;
};

#endif // SIMPLE_EFFECTS_FOLLOWER_HPP_INCLUDED
