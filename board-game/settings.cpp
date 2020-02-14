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
        std::cout << "\nGAME SETTINGS: \"" << game_name << "\"";
        std::cout << "\n\tmedia_path        =" << media_path;
        std::cout << "\n\twindow_size       =" << video_mode.width << "x" << video_mode.height;
        std::cout << "\n\tframe_rate_limit  =" << frame_rate_limit;
        std::cout << "\n\tis_self_test      =" << std::boolalpha << is_self_test;
    }

    void SnakeGameSettings::reset()
    {
        GameSettings::reset();

        turn_duration_sec = max_turn_duration_sec;
        food_eaten_count = 0;
        tail_id_to_assign_next = 0;
        tail_id_to_remove_next = 0;
        tail_pieces_to_add_remaining = 0;
    };

    void SnakeGameSettings::printStatus() const
    {
        GameSettings::printStatus();

        std::cout << "\n\t-";

        std::cout << "\n\tcell_size_window_ratio      =" << cell_size_window_ratio;

        std::cout << "\n\tturn_duration               =" << turn_duration_sec << " ["
                  << min_turn_duration_sec << ',' << max_turn_duration_sec << "]";

        std::cout << "\n\tturn_duration_mult_after_eat=" << turn_duration_mult_after_eat;
        std::cout << "\n\tfood_eaten_count            =" << food_eaten_count;
        std::cout << "\n\tmin_turn_duration_sec       =" << min_turn_duration_sec;
        std::cout << "\n\tmax_turn_duration_sec       =" << max_turn_duration_sec;
        std::cout << "\n\tturn_duration_sec           =" << turn_duration_sec;
        std::cout << "\n\tturn_duration_mult_after_eat=" << turn_duration_mult_after_eat;
        std::cout << "\n\t-";
        std::cout << "\n\tfood_eaten_count            =" << food_eaten_count;
        std::cout << "\n\ttail_growth_per_eat_count   =" << tail_growth_per_eat_count;
        std::cout << "\n\ttail_id_to_assign_next      =" << tail_id_to_assign_next;
        std::cout << "\n\ttail_id_to_remove_next      =" << tail_id_to_remove_next;
        std::cout << "\n\ttail_pieces_to_add_remaining=" << tail_pieces_to_add_remaining;
    }
} // namespace boardgame