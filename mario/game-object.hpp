#ifndef MARIO_GAMEOBJECT_HPP_INCLUDED
#define MARIO_GAMEOBJECT_HPP_INCLUDED
//
// game-objet.hpp
//
#include "context.hpp"
#include "image.hpp"

#include <SFML/Graphics.hpp>

namespace mario
{
    struct GameObject : public sf::Drawable
    {
        GameObject(const Context & context, const Image::Enum image, const sf::Vector2f & pos);
        virtual ~GameObject() = default;

        inline Image::Enum image() const { return which_image; }

        void update(const Context & context, const float elapsedTimeSec);
        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
        void loopPositionIfOffscreen(const Context & context);
        void flipHorizDirection();

        Image::Enum which_image;
        bool can_move;
        bool will_bounce;
        bool will_fall;
        bool can_hit_obstacles;
        bool is_obstacle;
        bool is_standing_on_something;
        Mover mover;
        sf::Sprite sprite;
    };
} // namespace mario

#endif // MARIO_GAMEOBJECT_HPP_INCLUDED