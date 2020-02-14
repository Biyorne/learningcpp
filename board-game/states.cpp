// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// states.cpp
//
/*
#include "states.hpp"

#include "board.hpp"
#include "context.hpp"
#include "pieces.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "util.hpp"

namespace boardgame
{
    StateBase::StateBase(
        const Context & context,
        const GamePhase::Enum currentPhase,
        const GamePhase::Enum nextPhase,
        const float durationSec,
        const sf::Keyboard::Key exitKey,
        const std::string & splashText,
        const float preDelaySec)
        : m_currentPhase(currentPhase)
        , m_nextPhase(nextPhase)
        , m_preDelaySec(preDelaySec)
        , m_durationSec(durationSec)
        , m_elapsedSec(0.0f)
        , m_exitKey(exitKey)
        , m_splashString(splashText)
        , m_splashText()
    {
        if (m_splashBackgroundVerts.getVertexCount() == 0)
        {
            m_splashBackgroundVerts = util::makeRectangleVerts(context.board.view().window_rect);
        }

        if (!m_splashString.empty())
        {
            m_splashText.setCharacterSize(99);
            m_splashText.setFillColor(sf::Color::White);
            m_splashText.setFont(context.resources.font);
            m_splashText.setString(m_splashString);
            util::scaleAndCenterInside(m_splashText, context.board.view().window_rect, true);
        }
    }

    GamePhase::Enum StateBase::update(Context &, const float frameTimeSec)
    {
        if (!isPreDelayFinished())
        {
            m_preDelaySec -= frameTimeSec;
            return GamePhase::Count;
        }

        m_elapsedSec += frameTimeSec;

        if (!(m_durationSec > 0.0f) || (m_elapsedSec < m_durationSec))
        {
            return GamePhase::Count;
        }

        return m_nextPhase;
    }

    GamePhase::Enum StateBase::handleEvent(Context & context, const sf::Event & event)
    {
        if (!isPreDelayFinished())
        {
            return GamePhase::Count;
        }

        return handleCommonEvents(context, event);
    }

    void StateBase::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        if (!isPreDelayFinished())
        {
            return;
        }

        if (!m_splashString.empty())
        {
            target.draw(m_splashBackgroundVerts, states);
            target.draw(m_splashText, states);
        }
    }

    float StateBase::durationRatio() const
    {
        if ((m_elapsedSec > 0.0f) && (m_durationSec > 0.0f))
        {
            return std::clamp((m_durationSec / m_elapsedSec), 0.0f, 1.0f);
        }

        return 0.0f;
    }

    GamePhase::Enum StateBase::handleCommonEvents(Context &, const sf::Event & event)
    {
        if (event.type == sf::Event::Closed)
        {
            return GamePhase::Quit;
        }

        if (sf::Event::KeyPressed != event.type)
        {
            if ((sf::Keyboard::Escape == event.key.code) || (sf::Keyboard::Q == event.key.code))
            {
                return GamePhase::Quit;
            }

            if (m_exitKey == IState::key_any)
            {
                return m_nextPhase;
            }

            if (m_exitKey == IState::key_none)
            {
                return GamePhase::Count;
            }

            if (event.key.code == m_exitKey)
            {
                return m_nextPhase;
            }
        }

        return GamePhase::Count;
    }

    //

    PlayState::PlayState(const Context & context)
        : StateBase(
              context,
              GamePhase::Play,
              GamePhase::Play,
              0.0f,
              IState::key_none,
              "PAUSED\npress any key to resume")
        , m_isPaused(false)
        , m_turnPiece(Piece::Player)
    {}

    GamePhase::Enum PlayState::update(Context &, const float frameTimeSec) {}

    GamePhase::Enum PlayState::handleEvent(Context & context, const sf::Event & event)
    {
        const GamePhase::Enum newPhaseAfterCommonEvents =
            StateBase::handleCommonEvents(context, event);

        if (event.type == sf::Event::LostFocus)
        {
            return GamePhase::Pause;
        }
    }

    //

    StateManager::StateManager(const Context & context)
        : m_stateUPtr(make(context, GamePhase::PressAnyKeyToStart))
    {}

    void StateManager::update(Context &, const float) {}

    void StateManager::handleEvent(Context &, const sf::Event &) {}

    void StateManager::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(*m_stateUPtr, states);
    }

    void StateManager::switchTo(const Context & context, const GamePhase::Enum phase)
    {
        m_stateUPtr = std::move(make(context, phase));
    }

    IStateUPtr_t StateManager::make(const Context & context, const GamePhase::Enum phase) const
    {
        switch (phase)
        {
            case GamePhase::PressAnyKeyToStart:
                return std::make_unique<TextMessageState>(
                    context,
                    "press any key to start",
                    phase,
                    GamePhase::Play,
                    0.0f,
                    IState::key_any);

            case GamePhase::Play: return std::make_unique<PlayState>(context);

            case GamePhase::Win:
                return std::make_unique<TextMessageState>(
                    context, "YOU WIN", phase, GamePhase::Quit, 3.0f, IState::key_any);

            case GamePhase::Lose:
                return std::make_unique<TextMessageState>(
                    context, "YOU LOSE", phase, GamePhase::Quit, 3.0f, IState::key_any);

            case GamePhase::Quit: return std::make_unique<StateBase>(context, phase);

            case GamePhase::Count:
            default:
                throw std::runtime_error(
                    "StateManager::makeState(GamePhase::Enum) value undefined: " +
                    std::to_string(static_cast<int>(phase)));
        }
    }
} // namespace boardgame
*/