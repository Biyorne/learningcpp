#ifndef SIMPLE_EFFECTS_BASE_HPP_INCLUDED
#define SIMPLE_EFFECTS_BASE_HPP_INCLUDED

#include "context.hpp"
#include "movement.hpp"

#include <memory>

#include <SFML/Graphics.hpp>

namespace entity
{

    struct IEffect : public sf::Drawable
    {
        virtual ~IEffect() = default;

        virtual bool isFinished() const = 0;
        void draw(sf::RenderTarget & target, sf::RenderStates states) const override = 0;
        virtual void update(const Context & context, const float frameTimeSec) = 0;
        virtual void handleEvent(const Context & context, const sf::Event & event) = 0;
    };

    using IEffectUPtr_t = std::unique_ptr<IEffect>;
    using IEffectUVec_t = std::vector<IEffectUPtr_t>;

    //

    class EffectBase : public IEffect
    {
      public:
        explicit EffectBase(const sf::Sprite & sprite = sf::Sprite())
            : m_sprite(sprite)
        {}

        virtual ~EffectBase() = default;

        bool isFinished() const override { return (m_sprite.getColor().a == 0); }

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            target.draw(m_sprite, states);
        }

        void update(const Context & context, const float frameTimeSec) override = 0;

        void handleEvent(const Context &, const sf::Event &) override {}

      protected:
        static sf::Sprite makeSprite(
            const Context & context,
            const sf::Texture & texture,
            const float textureSizeWindowRatio,
            const sf::Vector2f & spawnPos = { 0.0f, 0.0f })
        {
            const float windowSizeAvg{ (context.window_size.x + context.window_size.y) / 2.0f };
            const float maxDimm{ windowSizeAvg * textureSizeWindowRatio };

            sf::Sprite sprite(texture);
            util::setOrigin2Center(sprite);
            sprite.setPosition(spawnPos);
            util::scaleTo(sprite, sf::Vector2f(maxDimm, maxDimm));

            return sprite;
        }

        sf::Sprite m_sprite;
    };
} // namespace entity
#endif // SIMPLE_EFFECTS_BASE_HPP_INCLUDED
