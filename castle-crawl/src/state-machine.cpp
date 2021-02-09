// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-machine.cpp
//
#include "state-machine.hpp"

#include "context.hpp"
#include "state-play.hpp"
#include "state-popup.hpp"
#include "state-splash.hpp"

#include <iostream>

namespace castlecrawl
{

    StateMachine::StateMachine()
        : m_stateUPtr()
        , m_changePendingOpt(State::Init)
    {
        m_stateUPtr = std::make_unique<StateInit>();
        m_changePendingOpt = m_stateUPtr->state();
    }

    void StateMachine::changeIfPending(Context & context)
    {
        if (!m_changePendingOpt)
        {
            return;
        }

        m_stateUPtr->onExit(context);

        m_stateUPtr = makeState(context, m_changePendingOpt.value());
        m_changePendingOpt = std::nullopt;

        m_stateUPtr->onEnter(context);
    }

    IStateUPtr_t StateMachine::makeState(Context & context, const State state)
    {
        switch (state)
        {
            // clang-format off
            case State::Init:   { return std::make_unique<StateInit>();         }
            case State::Splash: { return std::make_unique<StateSplash>();       }
            case State::Play:   { return std::make_unique<StatePlay>(context);  }
            case State::Popup:  { return std::make_unique<StatePopup>(context); }
            case State::Quit:   { return std::make_unique<StateQuit>(context);  }
                // clang-format on
            default: {
                std::cerr << "ERROR:  StateMachine::makeState(\"" << state
                          << "\") not handled in switch.  Bail." << std::endl;

                return std::make_unique<StateQuit>(context);
            }
        };
    }

} // namespace castlecrawl
