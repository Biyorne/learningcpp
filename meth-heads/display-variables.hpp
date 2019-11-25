#ifndef METHHEADS_DISPLAY_VARIABLES_HPP_INCLUDED
#define METHHEADS_DISPLAY_VARIABLES_HPP_INCLUDED

#include "display-constants.hpp"

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

namespace methhead
{
    struct IActor;
    struct IPickup;

    using IActorUPtr_t = std::unique_ptr<IActor>;
    using IPickupUPtr_t = std::unique_ptr<IPickup>;

    //

    class DisplayVariables
    {
      public:
        explicit DisplayVariables(const sf::Vector2u & windowSize);
        virtual ~DisplayVariables() = default;

        void updateScoreBars(const int lazyScore, const int greedyScore);

        void draw(
            const bool willDrawDoardWithVerts,
            const std::vector<IActorUPtr_t> & actors,
            const std::vector<IPickupUPtr_t> & pickups,
            sf::RenderTarget & target,
            sf::RenderStates states) const;

        const DisplayConstants & constants() const { return m_constants; }

        void setFps(const std::size_t framesPerSecond);

      private:
        void populateBoardDrawables();
        void drawBoardUsingRectangleShapes(sf::RenderTarget & target) const;
        void drawBorderUsingVerts(sf::RenderTarget & target) const;

      private:
        DisplayConstants m_constants;
        sf::RectangleShape m_lazyScoreRectangle;
        sf::RectangleShape m_greedyScoreRectangle;

        std::vector<sf::RectangleShape> m_boardRectangles;
        std::vector<sf::Vertex> m_quadVerts;
        std::vector<sf::Vertex> m_lineVerts;

        sf::Text m_fpsText;
        mutable sf::Text m_pickupText;
        mutable sf::Sprite m_pickupSprite;
        mutable sf::Sprite m_actorSprite;
    };
} // namespace methhead

#endif // METHHEADS_DISPLAY_VARIABLES_HPP_INCLUDED