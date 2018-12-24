#ifndef LIGHTS_OUT_CELL_VIEW_HPP_INCLUDED
#define LIGHTS_OUT_CELL_VIEW_HPP_INCLUDED

#include "cell-model.hpp"

#include <SFML/Graphics.hpp>

namespace lightsout
{
    // For now, only using CellView to know what color to draw every cell.
    class CellView
    {
    public:
        CellView(
            const sf::Color & ON_COLOR,
            const sf::Color & OFF_COLOR,
            const GridPos_t & GRID_POSITION_V,
            const bool IS_ON);

        // Assume that on is 255 and off is 0
        void updateCurrentColor(const float ELAPSED_TIME_SEC, const float FADE_SPEED);

        GridPos_t gridPosition() const { return m_gridPosition; }

        sf::Color currentColor() const { return m_currentColor; }

        void setIsOn(const bool IS_ON) { m_isOn = IS_ON; }

    private:
        void setCurrentColor()
        {
            m_currentColor.r = static_cast<sf::Uint8>(m_redValue);
            m_currentColor.g = static_cast<sf::Uint8>(m_greenValue);
            m_currentColor.b = static_cast<sf::Uint8>(m_blueValue);
        }

        sf::Color m_currentColor;
        sf::Color m_onColor;
        sf::Color m_offColor;
        GridPos_t m_gridPosition;
        bool m_isOn;
        float m_redValue;
        float m_greenValue;
        float m_blueValue;
    };
} // namespace lightsout

#endif // LIGHTS_OUT_CELL_VIEW_HPP_INCLUDED
