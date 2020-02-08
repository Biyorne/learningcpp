#ifndef SIMPLE_EFFECTS_BASE_HPP_INCLUDED
#define SIMPLE_EFFECTS_BASE_HPP_INCLUDED

#include "context.hpp"
#include "steady-mover.hpp"

#include <SFML/Graphics.hpp>

namespace entity
{
    class EffectBase : public sf::Drawable
    {
      public:
        explicit EffectBase(const sf::Sprite & sprite = sf::Sprite())
            : m_sprite(sprite)
        {}

        virtual ~EffectBase() = default;

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            target.draw(m_sprite, states);
        }

        virtual void update(const Context & context, const float elapsedTimeSec) = 0;

      protected:
        static sf::Sprite
            makeSprite(const sf::Texture & texture, const sf::Vector2f & spawnPos = { 0.0f, 0.0f })
        {
            sf::Sprite sprite(texture);
            util::setOrigin2Center(sprite);
            sprite.setPosition(spawnPos);

            return sprite;
        }

        sf::Sprite m_sprite;
    };
} // namespace entity
#endif // SIMPLE_EFFECTS_BASE_HPP_INCLUDED
