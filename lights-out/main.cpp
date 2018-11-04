#include "cell-model.hpp"
#include "game-board-model.hpp"
#include "window.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

namespace lightsout
{
    class CellView
    {
    public:
    private:
        sf::Color m_currentColor;
        sf::Color m_valueRed;
    };

    class GameBoardViewBasic
    {
    public:
        void draw(const GameBoardModel & GAME_BOARD, Window & window) const
        {
            window.clear();
            // for loop over every cell
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
        GameBoardViewFade(const sf::Color & ON_COLOR)
            : m_colorOn(ON_COLOR)
            , m_colorOff(calcOffColor(ON_COLOR))
        {}

        sf::Color calcOffColor(const sf::Color ON_COLOR) const
        {
            sf::Color offColor(ON_COLOR);
            offColor.r = (ON_COLOR.r / 2);
            offColor.g = (ON_COLOR.g / 2);
            offColor.b = (ON_COLOR.b / 2);
            return offColor;
        }

        void update(const float FRAME_TIME_SEC)
        {
            // Gradually change the color of the cells.
        }

        void draw(const GameBoardModel & GAME_BOARD, Window & window) const
        {
            window.clear();

            for (const CellModel & CELL : GAME_BOARD.cells())
            {
                // Draw current cell at its current color.
                // Temp code to get basic functionality before we implement fading.
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
        sf::Color m_colorOn;
        sf::Color m_colorOff;
    };

} // namespace lightsout

int main()
{
    lightsout::Window window("Lights Out", 800, 600, sf::Color::Black);

    lightsout::GameBoardModel gameBoardModel(sf::FloatRect(sf::Vector2f(), window.size()));

    lightsout::GameBoardViewFade gameBoardView(sf::Color::Red); // sf::Color(121, 50, 105));

    sf::Clock frameClock;

    while (window.isOpen() && (gameBoardModel.isGameOver() == false))
    {
        const float FRAME_TIME_SEC(frameClock.getElapsedTime().asSeconds());
        frameClock.restart();

        window.handleEvents(gameBoardModel);
        gameBoardView.update(FRAME_TIME_SEC);
        gameBoardView.draw(gameBoardModel, window);
    }

    return EXIT_SUCCESS;
}
