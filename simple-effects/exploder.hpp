#ifndef SIMPLE_EFFECTS_EXPLODER_HPP_INCLUDED
#define SIMPLE_EFFECTS_EXPLODER_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"
#include "random.hpp"
#include "steady-mover.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace entity
{
    class ExploderEffect : public EffectBase
    {
      public:
        ExploderEffect(
            const Context & context,
            const sf::Texture & texture,
            const float speedLimit,
            const std::size_t pieceCount)
            : EffectBase(makeSprite(
                  context,
                  texture,
                  context.sprite_size_ratio_default,
                  (context.window_size * 0.5f)))
            , m_pieces()
        {
            m_pieces.reserve(pieceCount);
            divideIntoRects(pieceCount, texture);
            m_sprite.setPosition({});
            m_sprite.move(m_sprite.getOrigin());
        }

        virtual ~ExploderEffect() = default;

        void divideIntoRects(const std::size_t pieceCount, const sf::Texture & texture)
        {
            const float cellSideLength(static_cast<float>(std::sqrt(pieceCount)));
            const sf::Vector2f cellSize(cellSideLength, cellSideLength);

            const sf::Vector2f imageSize(util::size(m_sprite.getGlobalBounds()));
            const sf::Vector2i pieceCounts(imageSize / cellSize);

            for (int vert(0); vert < pieceCounts.y; ++vert)
            {
                for (int horiz(0); horiz < pieceCounts.x; ++horiz)
                {
                    const sf::Vector2i piecePos(sf::Vector2i(horiz, vert) * pieceCounts);

                    // const auto pos = (rectPos + spritePos) - (spriteSize / 2.0f);
                    // rect = {pos, size};

                    // const sf::Vector2f finalPos(
                    //    (piecePos + m_sprite.getPosition()) -
                    //    (util::size(m_sprite.getGlobalBounds()) / 2.0f));

                    const sf::IntRect rect(piecePos, pieceCounts);
                    const auto rs = util::makeRectangleShape(sf::FloatRect(rect));
                    m_pieces.push_back(rs);
                    // sf::Sprite sprite(texture, sf::IntRect(rect));
                    // m_pieces.push_back(sprite);
                }
            }
        }

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            EffectBase::draw(target, states);

            for (const auto & piece : m_pieces)
            {
                target.draw(piece, states);
            }
        }

        void update(const Context &, const float frameTimeSec) override {}

      private:
        std::vector<sf::RectangleShape> m_pieces;
    };

} // namespace entity

#endif // SIMPLE_EFFECTS_EXPLODER_HPP_INCLUDED
