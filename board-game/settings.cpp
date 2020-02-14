// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// settings.cpp
//
#include "settings.hpp"

#include <iostream>

namespace boardgame
{
    void GameSettings::reset()
    {
        is_game_over = false;
        is_game_paused = false;
    };

    void GameSettings::printStatus() const
    {
        std::cout << "GAME SETTINGS: \"" << game_name << "\"";
        std::cout << "\n\t media_path        = " << media_path;
        std::cout << "\n\t window_size       = " << video_mode.width << "x" << video_mode.height;
        std::cout << "\n\t frame_rate_limit  = " << frame_rate_limit;
        std::cout << "\n\t is_self_test      = " << std::boolalpha << is_self_test;
        std::cout << "\n\t is_game_over      = " << std::boolalpha << is_game_over;
        std::cout << std::endl;
    }

    void SnakeGameSettings::reset()
    {
        GameSettings::reset();
        food_eaten_count = 0;
    };

    void SnakeGameSettings::printStatus() const
    {
        GameSettings::printStatus();

        std::cout << "SNAKE SETTINGS:";
        std::cout << "\n\t cell_size_window_ratio       = " << cell_size_window_ratio;
        std::cout << "\n\t turn_duration_min            = " << turn_duration_min_sec;
        std::cout << "\n\t turn_duration_max            = " << turn_duration_max_sec;
        std::cout << "\n\t turn_duration_ratio_after_eat= " << turn_duration_ratio_after_eat;
        std::cout << "\n\t food_eaten_count             = " << food_eaten_count;
        std::cout << "\n\t period_duration_sec          = " << period_duration_sec;
        std::cout << std::endl;
    }
} // namespace boardgame