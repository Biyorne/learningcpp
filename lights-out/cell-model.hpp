#ifndef LIGHTS_OUT_CELL_MODEL_HPP_INCLUDED
#define LIGHTS_OUT_CELL_MODEL_HPP_INCLUDED

#include <SFML/Graphics.hpp>

#include <algorithm>

namespace lightsout
{

    using GridPos_t = sf::Vector2i;

    class CellModel
    {
    public:
        CellModel(const sf::FloatRect & REGION, const GridPos_t & GRID_POS_V)
            : m_isOn(true)
            //, m_rectangle()
            , m_gridPosV(GRID_POS_V)
            //, m_colorOn(COLOR)
            //, m_colorOff((m_colorOn.r / 2), (m_colorOn.g / 2), (m_colorOn.b / 2), m_colorOn.a)
            //, m_valueRed(m_colorOn.r)
            , m_region(REGION)
        {
            // m_rectangle.setSize(sf::Vector2f(REGION.width, REGION.height));
            // m_rectangle.setPosition(sf::Vector2f(REGION.left, REGION.top));
        }

        sf::FloatRect region() const { return m_region; }

        // void update(const float) // ELAPSED_TIME)
        //{
        // const float ANIMATION_SPEED(500.0f);
        // if (m_isOn)
        //{
        //    m_valueRed += (ELAPSED_TIME * ANIMATION_SPEED);
        //}
        // else
        //{
        //    m_valueRed -= (ELAPSED_TIME * ANIMATION_SPEED);
        //}
        //
        // m_valueRed = std::clamp(
        //    m_valueRed, static_cast<float>(m_colorOff.r), static_cast<float>(m_colorOn.r));
        //
        // m_rectangle.setFillColor(sf::Color(static_cast<sf::Uint8>(m_valueRed), 0, 0));
        //}

        bool isOn() const { return m_isOn; }

        bool doesContain(const sf::Vector2f & POSITION_V) const
        {
            return m_region.contains(POSITION_V);
        }

        GridPos_t gridPos() const { return m_gridPosV; }

        // sf::RectangleShape rectangle() const { return m_rectangle; }

        void change() { m_isOn = !m_isOn; }

    private:
        bool m_isOn;
        // sf::RectangleShape m_rectangle;
        GridPos_t m_gridPosV;
        // sf::Color m_colorOn;
        // sf::Color m_colorOff;
        // float m_valueRed;
        sf::FloatRect m_region;
    };

} // namespace lightsout

#endif // LIGHTS_OUT_CELL_MODEL_HPP_INCLUDED
