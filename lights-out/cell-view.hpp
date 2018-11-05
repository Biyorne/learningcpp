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
            : m_currentColor(ON_COLOR)
            , m_onColor(ON_COLOR)
            , m_offColor(OFF_COLOR)
            , m_gridPosition(GRID_POSITION_V)
            , m_isOn(IS_ON)
        {
            setIsOn(m_isOn);
        }

        GridPos_t gridPosition() const { return m_gridPosition; }

        sf::Color currentColor() const { return m_currentColor; }

        void setIsOn(const bool IS_ON)
        {
            m_isOn = IS_ON;
            if (m_isOn)
            {
                m_currentColor = m_onColor;
            }
            else
            {
                m_currentColor = m_offColor;
            }
        }

    private:
        sf::Color m_currentColor;
        sf::Color m_onColor;
        sf::Color m_offColor;
        GridPos_t m_gridPosition;
        bool m_isOn;

        // float m_valueRed;
    };
} // namespace lightsout

#endif // LIGHTS_OUT_CELL_VIEW_HPP_INCLUDED
