#ifndef METHHEADS_DISPLAY_VARIABLES_HPP_INCLUDED
#define METHHEADS_DISPLAY_VARIABLES_HPP_INCLUDED

#include "display-constants.hpp"

#include <array>
#include <memory>

#include <SFML/Graphics.hpp>

namespace methhead
{
    struct IActor;
    struct IPickup;

    using IActorUPtr_t = std::unique_ptr<IActor>;
    using IPickupUPtr_t = std::unique_ptr<IPickup>;

    class Settings;
    struct SimContext;

    //

    class DisplayVariables
    {
        using ImageVertArray_t = std::array<sf::Vertex, 4>;

      public:
        explicit DisplayVariables(const sf::Vector2u & windowSize);
        virtual ~DisplayVariables() = default;

        const DisplayConstants & constants() const { return m_constants; }

        void updatePerFrame(const SimContext & context, const float elapsedMs);

        void updatePerStatus(
            const std::size_t framesPerSecond, const int lazyScore, const int greedyScore);

        void draw(
            const SimContext & context, sf::RenderTarget & target, sf::RenderStates states) const;

      private:
        void updateFps(const std::size_t framesPerSecond);
        void updateScoreBars(const int lazyScore, const int greedyScore);

        void drawUsingSprites(
            const SimContext & context, sf::RenderTarget & target, sf::RenderStates states) const;

        void drawUsingVerts(
            const SimContext & context, sf::RenderTarget & target, sf::RenderStates states) const;

        void setupColorsAndStyles();
        void setupScales();
        void setupBoardDrawingVerts();
        void setupImageVertArray(ImageVertArray_t & vertsArray, const sf::Texture & texture) const;
        void setupImageVertArray();

        void appendImageVerts(
            const sf::Vector2f pos, ImageVertArray_t & srcVerts, sf::VertexArray & dstVerts) const;

      private:
        DisplayConstants m_constants;

        sf::Text m_fpsText;

        mutable sf::Text m_pickupText;
        mutable sf::Sprite m_pickupSprite;
        mutable sf::Sprite m_lazySprite;
        mutable sf::Sprite m_greedySprite;

        ImageVertArray_t m_pickupImageVertArray;
        ImageVertArray_t m_lazyImageVertArray;
        ImageVertArray_t m_greedyImageVertArray;

        sf::VertexArray m_boardQuadVerts;
        sf::VertexArray m_boardLineVerts;
        sf::VertexArray m_scoreQuadVerts;
        sf::VertexArray m_allPickupImageVerts;
        sf::VertexArray m_allLazyImageVerts;
        sf::VertexArray m_allGreedyImageVerts;
    };
} // namespace methhead

#endif // METHHEADS_DISPLAY_VARIABLES_HPP_INCLUDED