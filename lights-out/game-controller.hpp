#ifndef LIGHTS_OUT_GAME_CONTROLLER_HPP_INCLUDED
#define LIGHTS_OUT_GAME_CONTROLLER_HPP_INCLUDED

#include "cell-model.hpp"
#include "game-board-model.hpp"
#include "game-board-view-fade.hpp"
#include "game-state-enum.hpp"
#include "window.hpp"

namespace lightsout
{
    class GameController
    {
    public:
        GameController();

        bool isRunning() const
        {
            return (
                m_window.isOpen() && (m_state != GameState::Win) && (m_state != GameState::Lose));
        }

        void handleEvents();

        void draw(const float FRAME_TIME_SEC);

    private:
        void changeState(const GameState NEW_STATE);

        void handleEventClose(const sf::Event & EVENT);

        void handleEventGameOver(const sf::Event &);

        void handleEventPlay(const sf::Event & EVENT);

        void handleEventHelp(const sf::Event & EVENT);

    private:
        GameState m_state;
        Window m_window;
        GameBoardModel m_boardModel;
        GameBoardViewFade m_boardView;
    };
} // namespace lightsout

#endif // LIGHTS_OUT_GAME_CONTROLLER_HPP_INCLUDED
