#ifndef CASTLECRAWL_STATEMACHINE_HPP_INCLUDED
#define CASTLECRAWL_STATEMACHINE_HPP_INCLUDED
//
// state-machine.hpp
//
#include "states.hpp"

#include <optional>
#include <ostream>
#include <string>

namespace castlecrawl
{

    class StateMachine
    {
      public:
        StateMachine();
        virtual ~StateMachine() = default;

        // prevent all copy and assignment
        StateMachine(const StateMachine &) = delete;
        StateMachine(StateMachine &&) = delete;
        //
        StateMachine & operator=(const StateMachine &) = delete;
        StateMachine & operator=(StateMachine &&) = delete;

        State stateEnum() const { return m_stateUPtr->state(); }

        IState & state() { return *m_stateUPtr; }
        const IState & state() const { return *m_stateUPtr; }

        bool isChangePending() const { return m_changePendingOpt.has_value(); }
        StateOpt_t getChangePending() const { return m_changePendingOpt; }
        void setChangePending(const State state) { m_changePendingOpt = state; }
        void changeIfPending(Context & context);

      private:
        IStateUPtr_t makeState(Context & context, const State state);

      private:
        IStateUPtr_t m_stateUPtr;
        StateOpt_t m_changePendingOpt;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_STATEMACHINE_HPP_INCLUDED
