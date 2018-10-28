#include "cell.hpp"
#include "game-board-model.hpp"
#include "window.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

namespace lightsout
{
    class GameBoardView
    {
    public:
        void draw(const GameBoardModel & GAME_BOARD, Window & window) const
        {
            window.clear();
            // for loop over every cell
            for (const Cell & CELL : GAME_BOARD.cells())
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
} // namespace lightsout

int main()
{
    lightsout::Window window("Lights Out", 800, 600, sf::Color::Black);

    lightsout::GameBoardModel gameBoardModel(
        sf::FloatRect(sf::Vector2f(), window.size())); // sf::Color(121, 50, 105));

    lightsout::GameBoardView gameBoardView;

    // sf::Clock frameClock;

    while (window.isOpen() && (gameBoardModel.isGameOver() == false))
    {
        // const float FRAME_TIME_SEC(frameClock.getElapsedTime().asSeconds());
        // frameClock.restart();

        window.handleEvents(gameBoardModel);
        // gameBoard.update(FRAME_TIME_SEC);

        gameBoardView.draw(gameBoardModel, window);
    }

    return EXIT_SUCCESS;
}
