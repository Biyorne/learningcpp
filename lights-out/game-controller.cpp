// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "game-controller.hpp"

namespace lightsout
{

    GameController::GameController()
        : m_state(GameState::Help)
        , m_window("Lights Out", 800, 600, sf::Color::Black)
        , m_boardModel(sf::FloatRect(sf::Vector2f(), m_window.size()))
        , m_boardView(sf::Color(121, 50, 105), m_boardModel)
        , m_hasGameStarted(false)
        , m_animTimer()
        , m_animDelaySec(0.8f)
    {
        // This is because changeState() sets up our overlay.
        changeState(GameState::Help);
    }

    void GameController::handleEvents()
    {
        const std::vector<sf::Event> EVENTS(m_window.gatherEvents());

        for (const sf::Event & EVENT : EVENTS)
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

            // no need to handle events during this state
            // else if (m_state == GameState::NewGameAnim)
            //{
            //    handleEventNewGameAnim(EVENT);
            //}
        }
    }

    void GameController::draw(const float FRAME_TIME_SEC)
    {
        m_window.clear();

        if (GameState::NewGameAnim == m_state)
        {
            m_boardModel.resetForValidNewGame();
        }

        m_boardView.update(FRAME_TIME_SEC, m_boardModel);
        m_boardView.draw(m_boardModel, m_window);
        m_window.display();

        if ((m_animTimer.getElapsedTime().asSeconds() > m_animDelaySec)
            && (GameState::NewGameAnim == m_state))
        {
            changeState(GameState::Play);
        }
    }

    void GameController::changeState(const GameState NEW_STATE)
    {
        m_state = NEW_STATE;
        m_boardView.setupOverlay(m_state, m_window);
    }

    void GameController::handleEventClose(const sf::Event & EVENT)
    {
        if (EVENT.type == sf::Event::Closed)
        {
            changeState(GameState::Lose);
        }
    }

    void GameController::handleEventGameOver(const sf::Event &) {}

    void GameController::handleEventPlay(const sf::Event & EVENT)
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
            else if (EVENT.key.code == sf::Keyboard::H)
            {
                changeState(GameState::Help);
            }
            // else if (EVENT.key.code == sf::Keyboard::F1)
            //{
            //    changeState(GameState::Win);
            //}
        }

        handleEventClose(EVENT);
    }

    void GameController::handleEventHelp(const sf::Event & EVENT)
    {
        if (EVENT.type == sf::Event::KeyPressed)
        {
            if (EVENT.key.code == sf::Keyboard::Escape)
            {
                changeState(GameState::Lose);
            }
            else if (EVENT.key.code == sf::Keyboard::H)
            {
                if (m_hasGameStarted)
                {
                    changeState(GameState::Play);
                }
            }
            else if (EVENT.key.code == sf::Keyboard::N)
            {
                m_hasGameStarted = true;
                m_boardModel.resetForValidNewGame();
                changeState(GameState::NewGameAnim);
                m_animTimer.restart();
            }
        }

        handleEventClose(EVENT);
    }

} // namespace lightsout
