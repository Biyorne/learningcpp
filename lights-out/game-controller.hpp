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
        GameController()
            : m_state(GameState::Help)
            , m_window("Lights Out", 800, 600, sf::Color::Black)
            , m_boardModel(sf::FloatRect(sf::Vector2f(), m_window.size()))
            , m_boardView(sf::Color(121, 50, 105), m_boardModel)
        {
            changeState(GameState::Help);
        }

        bool isRunning() const
        {
            return (
                m_window.isOpen() && (m_state != GameState::Win) && (m_state != GameState::Lose));
        }

        void handleEvents()
        {
            std::vector<sf::Event> events(m_window.gatherEvents());

            for (const sf::Event & EVENT : events)
            {
                if ((m_state == GameState::Win) || (m_state == GameState::Lose))
                {
                    handleEventGameOver(EVENT);
                }
                else if (m_state == GameState::Help)
                {
                    handleEventHelp(EVENT);
                }
                else if (m_state == GameState::Play)
                {
                    handleEventPlay(EVENT);
                }
            }
        }

        void draw(const float FRAME_TIME_SEC)
        {
            m_window.clear();
            m_boardView.update(FRAME_TIME_SEC, m_boardModel);
            m_boardView.draw(m_boardModel, m_window);
            m_window.display();
        }

    private:
        void changeState(const GameState NEW_STATE)
        {
            m_state = NEW_STATE;
            m_boardView.setupOverlay(m_state, m_window);
        }

        void handleEventClose(const sf::Event & EVENT)
        {
            if (EVENT.type == sf::Event::Closed)
            {
                changeState(GameState::Lose);
            }
        }

        void handleEventGameOver(const sf::Event &) {}

        void handleEventPlay(const sf::Event & EVENT)
        {
            if (EVENT.type == sf::Event::MouseButtonPressed)
            {
                const sf::Vector2f MOUSE_POSITION_V(
                    sf::Vector2i(EVENT.mouseButton.x, EVENT.mouseButton.y));

                m_boardModel.handleMouseClick(MOUSE_POSITION_V);

                if (m_boardModel.areAllCellsOff())
                {
                    changeState(GameState::Win);
                    return;
                }
            }
            else if (EVENT.type == sf::Event::KeyPressed)
            {
                if (EVENT.key.code == sf::Keyboard::Escape)
                {
                    changeState(GameState::Lose);
                }
                else if (EVENT.key.code == sf::Keyboard::F1)
                {
                    changeState(GameState::Win);
                }
                else if (EVENT.key.code == sf::Keyboard::H)
                {
                    changeState(GameState::Help);
                }
            }

            handleEventClose(EVENT);
        }

        void handleEventHelp(const sf::Event & EVENT)
        {
            if (EVENT.type == sf::Event::KeyPressed)
            {
                if (EVENT.key.code == sf::Keyboard::Escape)
                {
                    changeState(GameState::Lose);
                }
                else if (EVENT.key.code == sf::Keyboard::H)
                {
                    changeState(GameState::Play);
                }
                else if (EVENT.key.code == sf::Keyboard::N)
                {
                    m_boardModel.reset();
                    changeState(GameState::Play);
                }
            }

            handleEventClose(EVENT);
        }

    private:
        GameState m_state;
        Window m_window;
        GameBoardModel m_boardModel;
        GameBoardViewFade m_boardView;
    };
} // namespace lightsout

#endif // LIGHTS_OUT_GAME_CONTROLLER_HPP_INCLUDED
