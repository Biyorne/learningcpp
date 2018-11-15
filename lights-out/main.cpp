#include "cell-model.hpp"
#include "cell-view.hpp"
#include "game-board-model.hpp"
#include "window.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

namespace lightsout
{

    class GameBoardViewBasic
    {
    public:
        void draw(const GameBoardModel & GAME_BOARD, Window & window) const
        {
            window.clear();
            for (const CellModel & CELL : GAME_BOARD.cells())
            {
                if (CELL.isOn())
                {
                    window.drawRectangle(CELL.region(), sf::Color::Red);
                }
                else
                {
                    window.drawRectangle(CELL.region(), sf::Color::Blue);
                }
            }
            window.display();
        }

    private:
    };

    class GameBoardViewFade
    {
    public:
        GameBoardViewFade(const sf::Color & ON_COLOR, const GameBoardModel & GAME_BOARD_MODEL)
            : m_cellViews()
            , m_fadeSpeed(500.0f)
        {
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

int main()
{
    lightsout::Window window("Lights Out", 800, 600, sf::Color::Black);

    lightsout::GameBoardModel gameBoardModel(sf::FloatRect(sf::Vector2f(), window.size()));

    lightsout::GameBoardViewFade gameBoardView(
        sf::Color::Red, gameBoardModel); // sf::Color(121, 50, 105));

    sf::Clock frameClock;

    while (window.isOpen() && (gameBoardModel.isGameOver() == false))
    {
        const float FRAME_TIME_SEC(frameClock.getElapsedTime().asSeconds());
        frameClock.restart();

        window.handleEvents(gameBoardModel);
        gameBoardView.update(FRAME_TIME_SEC, gameBoardModel);
        gameBoardView.draw(gameBoardModel, window);
    }

    return EXIT_SUCCESS;
}
