#ifndef BOARDGAME_STATES_HPP_INCLUDED
#define BOARDGAME_STATES_HPP_INCLUDED
//
// states.hpp
//
/*
#include "types.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

namespace boardgame
{
    //

    struct Context;
    struct IBoard;
    struct IResources;

    //

    struct GamePhase
    {
        enum Enum
        {
            PressAnyKeyToStart = 0,
            Play,
            Win,
            Lose,
            Quit,
            Count
        };

        static std::string name(const Enum phase)
        {
            switch (phase)
            {
                case PressAnyKeyToStart: return "PressAnyKeyToStart";
                case Play: return "Play";
                case Win: return "Win";
                case Lose: return "Lose";
                case Quit: return "Quit";
                case Count: return "Count";

                default:
                    return "GamePhase::Enum value undefined: " +
                           std::to_string(static_cast<int>(phase));
            }
        }
    };

    //

    struct IState : public sf::Drawable
    {
        virtual ~IState() = default;

        virtual GamePhase::Enum phase() const = 0;

        // returns the new state to start if this state is finished
        virtual GamePhase::Enum update(Context &, const float) = 0;

        // returns the new state to start if this state is finished
        virtual GamePhase::Enum handleEvent(Context &, const sf::Event &) = 0;

        void draw(sf::RenderTarget &, sf::RenderStates) const override = 0;

        static inline const sf::Keyboard::Key key_any = sf::Keyboard::Unknown;
        static inline const sf::Keyboard::Key key_none = sf::Keyboard::KeyCount;
    };

    //

    using IStateOpt_t = std::optional<std::reference_wrapper<IState>>;
    using IStateUPtr_t = std::unique_ptr<IState>;
    using IStateUVec_t = std::vector<IStateUPtr_t>;

    //

    class StateBase : public IState
    {
      public:
        StateBase(
            const Context & context,
            const GamePhase::Enum currentPhase,
            const GamePhase::Enum nextPhase,
            const float durMaxSec = 0.0f,
            const sf::Keyboard::Key exitKey = IState::key_none,
            const std::string & splashText = {},
            const float preDelaySec = 0.0f);

        virtual ~StateBase() = default;

        GamePhase::Enum phase() const override { return m_currentPhase; }

        GamePhase::Enum update(Context &, const float frameTimeSec) override;
        GamePhase::Enum handleEvent(Context & context, const sf::Event & event) override;
        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

      protected:
        float durationRatio() const;
        virtual GamePhase::Enum handleCommonEvents(Context &, const sf::Event & event);
        bool isPreDelayFinished() const { return !(m_preDelaySec > 0.0f); }

      protected:
        GamePhase::Enum m_currentPhase;
        GamePhase::Enum m_nextPhase;
        float m_preDelaySec;
        float m_durationSec;
        float m_elapsedSec;
        sf::Keyboard::Key m_exitKey;
        std::string m_splashString;
        sf::Text m_splashText;

        static sf::VertexArray m_splashBackgroundVerts;
    };

    //

    struct TextMessageState : public StateBase
    {
        TextMessageState(
            const Context & context,
            const std::string & splashText,
            const GamePhase::Enum currentPhase,
            const GamePhase::Enum nextPhase,
            const float durationSec = 0.0f,
            const sf::Keyboard::Key exitKey = IState::key_none)
            : StateBase(context, currentPhase, nextPhase, durationSec, exitKey, splashText, 0.0f)
        {}

        virtual ~TextMessageState() = default;
    };

    //

    class PlayState : public StateBase
    {
      public:
        explicit PlayState(const Context & context);
        virtual ~PlayState() = default;

        GamePhase::Enum update(Context &, const float frameTimeSec) override;
        GamePhase::Enum handleEvent(Context & context, const sf::Event & event) override;

      private:
        bool m_isPaused;
        Piece::Enum m_turnPiece;
    };

    //

    class StateManager : public sf::Drawable
    {
      public:
        explicit StateManager(const Context & context);

        // IState & current() { return *m_stateUPtr; }
        // const IState & current() const { return *m_stateUPtr; }

        GamePhase::Enum phase() const { return m_stateUPtr->phase(); }

        void update(Context &, const float);
        void handleEvent(Context &, const sf::Event &);
        void draw(sf::RenderTarget &, sf::RenderStates) const final;

      private:
        void switchTo(const Context & context, const GamePhase::Enum phase);
        IStateUPtr_t make(const Context & context, const GamePhase::Enum phase) const;

      private:
        IStateUPtr_t m_stateUPtr;
    };
} // namespace boardgame

#endif // BOARDGAME_STATES_HPP_INCLUDED

*/