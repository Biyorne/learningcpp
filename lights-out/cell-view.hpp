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
            const bool IS_ON)
            : m_currentColor(sf::Color::Transparent)
            , m_onColor(ON_COLOR)
            , m_offColor(OFF_COLOR)
            , m_gridPosition(GRID_POSITION_V)
            , m_isOn(IS_ON)
            , m_redValue(0.0f)
        {
            if (IS_ON)
            {
                m_currentColor = ON_COLOR;
                m_redValue = static_cast<float>(ON_COLOR.r);
            }
            else
            {
                m_currentColor = OFF_COLOR;
                m_redValue = static_cast<float>(OFF_COLOR.r);
            }
        }

        // Assume all colors are red or black
        // This lets us assume that only the red color value is changing.
        // Ignore all other color values.
        // Assume that on is 255 and off is 0

        // Change the value of m_redValue.

        void updateCurrentColor(const float ELAPSED_TIME_SEC, const float FADE_SPEED)
        {
            const float AMOUNT_TO_CHANGE(FADE_SPEED * ELAPSED_TIME_SEC);

            if (m_isOn)
            {
                m_redValue += AMOUNT_TO_CHANGE;
            }
            else
            {
                m_redValue -= AMOUNT_TO_CHANGE;
            }

            // Assume that off value is lower than on value.
            m_redValue = std::clamp(
                m_redValue, static_cast<float>(m_offColor.r), static_cast<float>(m_onColor.r));

            setCurrentColor();
        }

        GridPos_t gridPosition() const { return m_gridPosition; }

        sf::Color currentColor() const { return m_currentColor; }

        void setIsOn(const bool IS_ON) { m_isOn = IS_ON; }

    private:
        void setCurrentColor() { m_currentColor.r = static_cast<sf::Uint8>(m_redValue); }

        sf::Color m_currentColor;
        sf::Color m_onColor;
        sf::Color m_offColor;
        GridPos_t m_gridPosition;
        bool m_isOn;
        float m_redValue;

        // float m_valueRed;
    };
} // namespace lightsout

#endif // LIGHTS_OUT_CELL_VIEW_HPP_INCLUDED
