// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// game-object.cpp
//
#include "game-object.hpp"

namespace mario
{
    GameObject::GameObject(
        const Context & context, const Image::Enum which, const sf::Vector2f & pos)
        : which_image(which)
        , can_move(Image::canMove(which))
        , will_bounce(Image::willBounce(which))
        , will_fall(Image::willFall(which))
        , can_hit_obstacles(Image::canHitObstacles(which))
        , is_obstacle(Image::isObstacle(which))
        , is_standing_on_something(false)
        , mover(Image::makeMover(which))
        , sprite(Image::makeSprite(context, which))
    {
        sprite.setPosition(pos);
    }

    void GameObject::update(const Context & context, const float)
    {
        if (!can_move)
        {
            return;
        }

        mover.updateVelocity(!is_standing_on_something);
        loopPositionIfOffscreen(context);
        sprite.move(mover.velocity);
    }

    void GameObject::flipHorizDirection()
    {
        if (!can_move || !can_hit_obstacles)
        {
            return;
        }
        sprite.scale(-1.0f, 1.0f);

        if (sprite.getScale().x < 0.0f)
        {
            sprite.move(sprite.getGlobalBounds().width, 0.0f);
        }
        else
        {
            sprite.move(-sprite.getGlobalBounds().width, 0.0f);
        }

        mover.velocity.x *= -1.0f;
    }

    void GameObject::loopPositionIfOffscreen(const Context & context)
    {
        const sf::FloatRect bounds{ sprite.getGlobalBounds() };
        const sf::Vector2f pos(bounds.left, bounds.top);
        if ((mover.velocity.x > 0.0f) && (pos.x > context.window_size.x))
        {
            sprite.move(-(bounds.width + context.window_size.x), 0.0f);
        }
        else if ((mover.velocity.x < 0.0f) && ((pos.x + bounds.width) < 0.0f))
        {
            sprite.move((bounds.width + context.window_size.x), 0.0f);
        }
    }

    void GameObject::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(sprite, states);
    }
} // namespace mario