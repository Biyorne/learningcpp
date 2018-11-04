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
            const GridPos_t & GRID_POSITION_V)
            : m_currentColor(ON_COLOR)
            , m_gridPosition(GRID_POSITION_V)
        {}

        GridPos_t gridPosition() const { return m_gridPosition; }

        sf::Color currentColor() const { return m_currentColor; }

    private:
        sf::Color m_currentColor;
        GridPos_t m_gridPosition;

        // float m_valueRed;
    };
} // namespace lightsout

#endif // LIGHTS_OUT_CELL_VIEW_HPP_INCLUDED
