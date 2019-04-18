#ifndef LIGHTS_OUT_GAME_STATE_ENUM_HPP_INCLUDED
#define LIGHTS_OUT_GAME_STATE_ENUM_HPP_INCLUDED

namespace lightsout
{
    enum class GameState
    {
        Help = 0,
        Play,
        Win,
        Lose,
        NewGameAnim
    };
}

#endif // LIGHTS_OUT_GAME_STATE_ENUM_HPP_INCLUDED
