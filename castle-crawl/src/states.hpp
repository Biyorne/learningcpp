#ifndef CASTLECRAWL_STATES_HPP_INCLUDED
#define CASTLECRAWL_STATES_HPP_INCLUDED
//
// states.hpp
//
#include <optional>
#include <ostream>
#include <string>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace castlecrawl
{
    struct Context;

    enum class State : std::size_t
    {
        Init = 0,
        Splash,
        Play,
        Popup,
        Quit,
    };

    using StateOpt_t = std::optional<State>;

    //

    inline std::string toString(const State state)
    {
        switch (state)
        {
            case State::Init: return "Init";
            case State::Splash: return "Splash";
            case State::Popup: return "Popup";
            case State::Play: return "Play";
            case State::Quit: return "Quit";
            default: return "";
        }
    }

    //
    inline std::ostream & operator<<(std::ostream & os, const State state)
    {
        os << toString(state);
        return os;
    }

    //
    struct IState
    {
        virtual ~IState() = default;

        virtual State state() const = 0;
        virtual void update(Context &, const float elapsedSec) = 0;
        virtual void handleEvent(Context & context, const sf::Event & event) = 0;
        virtual void draw(const Context &, sf::RenderTarget &, sf::RenderStates) const = 0;
        virtual void onEnter(Context &) = 0;
        virtual void onExit(Context &) = 0;
    };

    using IStateUPtr_t = std::unique_ptr<IState>;

    //
    class StateBase : public IState
    {
      protected:
        StateBase(const State state);

      public:
        virtual ~StateBase() override = default;

        // prevent all copy and assignment
        StateBase(const StateBase &) = delete;
        StateBase(StateBase &&) = delete;
        //
        StateBase & operator=(const StateBase &) = delete;
        StateBase & operator=(StateBase &&) = delete;

        State state() const final { return m_state; }

        void update(Context &, const float) override {}
        void handleEvent(Context &, const sf::Event &) override {}
        void draw(const Context &, sf::RenderTarget &, sf::RenderStates) const override {}
        void onEnter(Context &) override {}
        void onExit(Context &) override {}

      protected:
        State m_state;
    };

    //
    class StateInit : public StateBase
    {
      public:
        StateInit()
            : StateBase(State::Init)
        {}
    };

    //
    class StateQuit : public StateBase
    {
      public:
        StateQuit(Context &)
            : StateBase(State::Quit)
        {}

        void onEnter(Context & context) override;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_STATES_HPP_INCLUDED
