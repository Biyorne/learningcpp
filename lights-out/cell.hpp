#ifndef LIGHTS_OUT_CELL_HPP_INCLUDED
#define LIGHTS_OUT_CELL_HPP_INCLUDED

#include <SFML/Graphics.hpp>

using GridPos_t = sf::Vector2i;

class Cell
{
public:
    Cell(const sf::Vector2f & SIZE_V, const sf::Vector2f & POS_V, const GridPos_t & GRID_POS_V)
        : m_isOn(false)
        , m_rectangle()
        , m_gridPosV(GRID_POS_V)
        , m_colorOn(255, 0, 0)
        , m_colorOff(255, 0, 0, 127)
    {
        m_rectangle.setSize(SIZE_V);
        m_rectangle.setPosition(POS_V);
        change();
    }

    bool isOn() const { return m_isOn; }

    bool doesContain(const sf::Vector2f & POSITION_V) const
    {
        return m_rectangle.getGlobalBounds().contains(POSITION_V);
    }

    GridPos_t gridPos() const { return m_gridPosV; }

    sf::RectangleShape rectangle() const { return m_rectangle; }

    void change()
    {
        m_isOn = !m_isOn;

        if (m_isOn)
        {
            m_rectangle.setFillColor(m_colorOn);
        }
        else
        {
            m_rectangle.setFillColor(m_colorOff);
        }
    }

private:
    bool m_isOn;
    sf::RectangleShape m_rectangle;
    GridPos_t m_gridPosV;
    sf::Color m_colorOn;
    sf::Color m_colorOff;
};

#endif // LIGHTS_OUT_CELL_HPP_INCLUDED
