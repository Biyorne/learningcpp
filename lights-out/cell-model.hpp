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
            , m_gridPosV(GRID_POS_V)
            , m_region(REGION)
        {}

        sf::FloatRect region() const { return m_region; }

        bool isOn() const { return m_isOn; }

        bool doesContain(const sf::Vector2f & POSITION_V) const
        {
            return m_region.contains(POSITION_V);
        }

        GridPos_t gridPos() const { return m_gridPosV; }

        void change() { m_isOn = !m_isOn; }

    private:
        bool m_isOn;
        GridPos_t m_gridPosV;
        sf::FloatRect m_region;
    };

} // namespace lightsout

#endif // LIGHTS_OUT_CELL_MODEL_HPP_INCLUDED
