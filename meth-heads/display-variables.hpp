#ifndef METHHEADS_DISPLAY_VARIABLES_HPP_INCLUDED
#define METHHEADS_DISPLAY_VARIABLES_HPP_INCLUDED

#include "display-constants.hpp"

#include <SFML/Graphics.hpp>

namespace methhead
{
    class DisplayVariables
    {
      public:
        explicit DisplayVariables(const sf::Vector2u & windowSize);
        virtual ~DisplayVariables() = default;

        void update(const float elapsedMs, const int lazyScore, const int greedyScore);

        void draw(sf::RenderTarget & target, sf::RenderStates states) const;

        const DisplayConstants & constants() const { return m_constants; }

        void setFps(const std::size_t framesPerSecond);

      private:
        void setScoreBarsHeight(const int lazyScore, const int greedyScore);
        void populateBoardCells();

      private:
        DisplayConstants m_constants;
        sf::RectangleShape m_lazyScoreRectangle;
        sf::RectangleShape m_greedyScoreRectangle;
        std::vector<sf::RectangleShape> m_boardRectangles;
        sf::Text m_fpsText;
    };
} // namespace methhead

#endif // METHHEADS_DISPLAY_VARIABLES_HPP_INCLUDED