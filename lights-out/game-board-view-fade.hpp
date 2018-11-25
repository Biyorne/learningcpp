#ifndef LIGHTS_OUT_GAME_BOARD_VIEW_FADE_HPP_INCLUDED
#define LIGHTS_OUT_GAME_BOARD_VIEW_FADE_HPP_INCLUDED

#include "cell-model.hpp"
#include "cell-view.hpp"
#include "game-board-model.hpp"
#include "window.hpp"

#include <SFML/Graphics.hpp>

namespace lightsout
{
    class GameBoardViewFade
    {
    public:
        GameBoardViewFade(const sf::Color & ON_COLOR, const GameBoardModel & GAME_BOARD_MODEL)
            : m_cellViews()
            , m_fadeSpeed(500.0f)
        {
            sf::Keyboard::Key whichKey;

            const sf::Color OFF_COLOR(calcOffColor(ON_COLOR));
            for (const CellModel & CELL_MODEL : GAME_BOARD_MODEL.cells())
            {
                const CellView CELL_VIEW(
                    ON_COLOR, OFF_COLOR, CELL_MODEL.gridPos(), CELL_MODEL.isOn());

                m_cellViews.push_back(CELL_VIEW);
            }
        }

        sf::Color calcOffColor(const sf::Color ON_COLOR) const
        {
            sf::Color offColor(ON_COLOR);
            offColor.r = (ON_COLOR.r / 2);
            offColor.g = (ON_COLOR.g / 2);
            offColor.b = (ON_COLOR.b / 2);
            return offColor;
        }

        void update(const float ELAPSED_TIME_SEC, const GameBoardModel & GAME_BOARD)
        {
            for (const CellModel & CELL_MODEL : GAME_BOARD.cells())
            {
                GridPos_t GRID_POSITION_V(CELL_MODEL.gridPos());

                auto iterToFoundCellView(std::find_if(
                    std::begin(m_cellViews),
                    std::end(m_cellViews),
                    [&GRID_POSITION_V](const CellView & CELL_VIEW) {
                        return (CELL_VIEW.gridPosition() == GRID_POSITION_V);
                    }));

                if (std::end(m_cellViews) != iterToFoundCellView)
                {
                    iterToFoundCellView->setIsOn(CELL_MODEL.isOn());
                    iterToFoundCellView->updateCurrentColor(ELAPSED_TIME_SEC, m_fadeSpeed);
                }
            }
        }

        void draw(const GameBoardModel & GAME_BOARD, Window & window) const
        {
            window.clear();

            for (const CellModel & CELL_MODEL : GAME_BOARD.cells())
            {
                GridPos_t GRID_POSITION_V(CELL_MODEL.gridPos());

                auto iterToFoundCellView(std::find_if(
                    std::begin(m_cellViews),
                    std::end(m_cellViews),
                    [&GRID_POSITION_V](const CellView & CELL_VIEW) {
                        return (CELL_VIEW.gridPosition() == GRID_POSITION_V);
                    }));

                if (std::end(m_cellViews) != iterToFoundCellView)
                {
                    const sf::Color CURRENT_COLOR(iterToFoundCellView->currentColor());
                    window.drawRectangle(CELL_MODEL.region(), CURRENT_COLOR);
                }
            }

            window.display();
        }

    private:
        std::vector<CellView> m_cellViews;
        float m_fadeSpeed;
    };

} // namespace lightsout

#endif // LIGHTS_OUT_GAME_BOARD_VIEW_FADE_HPP_INCLUDED
