// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "cell-view.hpp"

namespace lightsout
{

    CellView::CellView(
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
        , m_greenValue(0.0f)
        , m_blueValue(0.0f)
    {
        if (IS_ON)
        {
            m_currentColor = ON_COLOR;
            m_redValue = static_cast<float>(ON_COLOR.r);
            m_greenValue = static_cast<float>(ON_COLOR.g);
            m_blueValue = static_cast<float>(ON_COLOR.b);
        }
        else
        {
            m_currentColor = OFF_COLOR;
            m_redValue = static_cast<float>(OFF_COLOR.r);
            m_greenValue = static_cast<float>(OFF_COLOR.g);
            m_blueValue = static_cast<float>(OFF_COLOR.b);
        }
    }

    // Assume that on is 255 and off is 0
    void CellView::updateCurrentColor(const float ELAPSED_TIME_SEC, const float FADE_SPEED)
    {
        const float AMOUNT_TO_CHANGE(FADE_SPEED * ELAPSED_TIME_SEC);

        if (m_isOn)
        {
            m_redValue += AMOUNT_TO_CHANGE;
            m_greenValue += AMOUNT_TO_CHANGE;
            m_blueValue += AMOUNT_TO_CHANGE;
        }
        else
        {
            m_redValue -= AMOUNT_TO_CHANGE;
            m_greenValue -= AMOUNT_TO_CHANGE;
            m_blueValue -= AMOUNT_TO_CHANGE;
        }

        // Assume that off value is lower than on value.
        m_redValue = std::clamp(
            m_redValue, static_cast<float>(m_offColor.r), static_cast<float>(m_onColor.r));

        m_greenValue = std::clamp(
            m_greenValue, static_cast<float>(m_offColor.g), static_cast<float>(m_onColor.g));

        m_blueValue = std::clamp(
            m_blueValue, static_cast<float>(m_offColor.b), static_cast<float>(m_onColor.b));

        setCurrentColor();
    }

} // namespace lightsout
