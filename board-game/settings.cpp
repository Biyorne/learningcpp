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
        std::cout << "\n\t is_self_play_test      = " << std::boolalpha << is_self_play_test;
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
        scoreAdjustmentsRemaining = 0;
        scoreCurrent = 0;
        // will_eating_tail_turn_it_into_wall = false;
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

        std::cout << "\n\t score                           = "
                  << (scoreCurrent + scoreAdjustmentsRemaining);

        std::cout << std::endl;
        // willEatingTailTurnItIntoWall
    }

    std::size_t SnakeGameSettings::tailLength() const
    {
        return (TailPiece::tailLength() + tail_pieces_to_grow_remaining);
    }

    void SnakeGameSettings::handleEat()
    {
        ++food_eaten_count;

        // if (!is_self_play_test || tailLength() < 10)
        {
            tail_pieces_to_grow_remaining += tail_growth_per_food_count;

            tail_pieces_to_grow_remaining +=
                static_cast<int>(static_cast<float>(tailLength()) * .2f);
        }

        increaseMoveSpeed();

        // std::cout << "Ate #" << food_eaten_count << ", tail=" << tailLength()
        //          << ", %=" << (100.0f * speedDifficultyRatio()) << std::endl;
    }
} // namespace boardgame