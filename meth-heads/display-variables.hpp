#ifndef METHHEADS_DISPLAY_VARIABLES_HPP_INCLUDED
#define METHHEADS_DISPLAY_VARIABLES_HPP_INCLUDED

#include "display-constants.hpp"
#include "meth-head.hpp"

#include <array>
#include <memory>

#include <SFML/Graphics.hpp>

namespace methhead
{
    struct SimContext;

    //

    class DisplayVariables
    {
        using ImageVertArray_t = std::array<sf::Vertex, 4>;

      public:
        explicit DisplayVariables(const sf::Vector2u & windowSize);
        virtual ~DisplayVariables() = default;

        const DisplayConstants & constants() const { return m_constants; }

        void draw(
            const SimContext & context, sf::RenderTarget & target, sf::RenderStates states) const;

      private:
        void setupColorsAndStyles();
        void setupSprites();
        void setupBoardDrawingVerts();

      private:
        DisplayConstants m_constants;

        mutable sf::Text m_pickupText;
        mutable sf::Sprite m_pickupSprite;
        mutable sf::Sprite m_lazySprite;
        mutable sf::Sprite m_greedySprite;

        sf::VertexArray m_boardQuadVerts;
        sf::VertexArray m_boardLineVerts;
    };
} // namespace methhead

#endif // METHHEADS_DISPLAY_VARIABLES_HPP_INCLUDED