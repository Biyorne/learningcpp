#ifndef LIGHTS_OUT_GAME_CONTROLLER_HPP_INCLUDED
#define LIGHTS_OUT_GAME_CONTROLLER_HPP_INCLUDED

#include "cell-model.hpp"
#include "game-board-model.hpp"
#include "game-board-view-fade.hpp"
#include "window.hpp"

namespace lightsout
{
    enum class GameState
    {
        Help = 0,
        Play,
        Win,
        Lose
    };

    class GameController
    {
    public:
        GameController()
            : m_state(GameState::Help)
            , m_window("Lights Out", 800, 600, sf::Color::Black)
            , m_boardModel(sf::FloatRect(sf::Vector2f(), m_window.size()))
            , m_boardView(sf::Color(121, 50, 105), m_boardModel)
        {}

        bool isRunning() const
        {
            return (m_window.isOpen() && (m_boardModel.isGameOver() == false));
        }

        void handleEvents() { m_window.handleEvents(m_boardModel); }

        void draw(const float FRAME_TIME_SEC)
        {
            m_window.clear();
            m_boardView.update(FRAME_TIME_SEC, m_boardModel);
            m_boardView.draw(m_boardModel, m_window);
            m_window.display();
        }

    private:
        GameState m_state;
        Window m_window;
        GameBoardModel m_boardModel;
        GameBoardViewFade m_boardView;
    };
} // namespace lightsout

#endif // LIGHTS_OUT_GAME_CONTROLLER_HPP_INCLUDED
