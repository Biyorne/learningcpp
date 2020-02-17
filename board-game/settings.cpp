// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// settings.cpp
//
#include "settings.hpp"

#include "pieces.hpp"

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
        time_between_turns_cur_sec = time_between_turns_max_sec;
        tail_pieces_to_grow_remaining = 0;
        total_turns_played = 0;
    };

    void SnakeGameSettings::printStatus() const
    {
        GameSettings::printStatus();

        std::cout << "SNAKE SETTINGS:";
        std::cout << "\n\t turns_per_sec                   = " << time_between_turns_cur_sec;
        std::cout << "\n\t turns_per_sec_min               = " << time_between_turns_min_sec;
        std::cout << "\n\t turns_per_sec_max               = " << time_between_turns_max_sec;
        std::cout << "\n\t time_between_turns_shrink_ratio = " << time_between_turns_shrink_ratio;
        std::cout << "\n\t total_turns_played              = " << total_turns_played;
        std::cout << "\n\t food_eaten_count                = " << food_eaten_count;
        std::cout << "\n\t tail_length                     = " << tailLength();
        std::cout << std::endl;
    }

    std::size_t SnakeGameSettings::tailLength() const
    {
        return (TailPiece::tailLength() + tail_pieces_to_grow_remaining);
    }

    void SnakeGameSettings::handleEat()
    {
        ++food_eaten_count;
        tail_pieces_to_grow_remaining += tail_growth_per_food_count;
        increaseMoveSpeed();

        std::cout << "Ate food #" << food_eaten_count << " means tail will grow to " << tailLength()
                  << ".  (played " << total_turns_played << " turns at "
                  << (1.0f / time_between_turns_cur_sec) << " turns_per_sec, or "
                  << (100.0f * speedDifficultyRatio()) << "% speed_difficulty)" << std::endl;
    }
} // namespace boardgame