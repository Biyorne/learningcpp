#ifndef BOARDGAME_SETTINGS_HPP_INCLUDED
#define BOARDGAME_SETTINGS_HPP_INCLUDED
//
// settings.hpp
//
#include "types.hpp"
#include "util.hpp"

#include <filesystem>
#include <string>

#include <SFML/Graphics.hpp>

namespace boardgame
{
    using BoardPos_t = sf::Vector2i;

    //

    struct GameSettings
    {
        GameSettings() = default;
        virtual ~GameSettings() = default;

        virtual void reset();
        virtual void printStatus() const;

        virtual sf::Vector2f windowSize() const
        {
            return sf::Vector2f{ sf::Vector2u(video_mode.width, video_mode.height) };
        }

        virtual sf::FloatRect windowBounds() const { return { { 0.0f, 0.0f }, windowSize() }; }

        std::string game_name{ "(no_name)" };

        std::filesystem::path media_path{ std::filesystem::current_path() };

        sf::VideoMode video_mode{ 1600,
                                  1200,
                                  sf::VideoMode::getDesktopMode().bitsPerPixel }; //(2880u, 1800u,

        // i.e.  (sf::Style::Titlebar | sf::Style::Close)
        unsigned int sf_window_style{ sf::Style::Fullscreen };

        // zero will disable the limit
        unsigned int frame_rate_limit{ 60 };

        bool is_game_over{ false };
        bool is_game_paused{ false };
        bool is_self_play_test{ false };
        bool is_god_mode{ false };
    };

    //

    class SnakeGameSettings : public GameSettings
    {
      public:
        SnakeGameSettings() = default;
        virtual ~SnakeGameSettings() = default;

        void reset() override;
        void printStatus() const override;

        float cell_size_window_ratio{ 0.0175f };

        // state
        std::size_t total_turns_played{ 0 };
        bool will_eating_tail_turn_it_into_wall{ true };

        float timeBetweenTurnsSec() const { return time_between_turns_cur_sec; }
        bool isTailGrowing() const { return (tail_pieces_to_grow_remaining > 0); }
        void handleEat();

        void handleTailLengthIncrease()
        {
            if (tail_pieces_to_grow_remaining > 0)
            {
                --tail_pieces_to_grow_remaining;
            }
        }

        std::size_t tailLength() const;

        float speedDifficultyRatio() const
        {
            return std::clamp(
                ((time_between_turns_max_sec - time_between_turns_min_sec) /
                 time_between_turns_cur_sec),
                0.0f,
                1.0f);
        }

        int scoreIncrementAndGet()
        {
            int multiplier{ 0 };

            if (scoreAdjustmentsRemaining > 0)
            {
                multiplier = 1;
            }
            else if (scoreAdjustmentsRemaining < 0)
            {
                if (scoreCurrent <= 0)
                {
                    multiplier = 0;
                }
                else
                {
                    multiplier = -1;
                }
            }
            else
            {
                multiplier = 0;
            }

            int scoreChange{ 0 };
            const int adjAbs{ std::abs(scoreAdjustmentsRemaining) };

            if (adjAbs > 10)
            {
                scoreChange = (multiplier * (adjAbs / 10));
            }
            else
            {
                scoreChange = multiplier;
            }

            scoreCurrent += scoreChange;
            scoreAdjustmentsRemaining -= scoreChange;

            if (scoreCurrent <= 0)
            {
                scoreCurrent = 0;

                if (scoreAdjustmentsRemaining < 0)
                {
                    scoreAdjustmentsRemaining = 0;
                }
            }

            return scoreCurrent;
        }

        int adjustScore() const { return scoreAdjustmentsRemaining; }

        void adjustScore(const int adj)
        {
            if (scoreCurrent <= 0)
            {
                scoreCurrent = 0;

                if (scoreAdjustmentsRemaining < 0)
                {
                    scoreAdjustmentsRemaining = 0;
                }
            }
            scoreAdjustmentsRemaining += adj;
        }

        std::size_t foodEatenCount() const { return food_eaten_count; }

      private:
        void increaseMoveSpeed()
        {
            time_between_turns_cur_sec *= time_between_turns_shrink_ratio;
            if (time_between_turns_cur_sec < time_between_turns_min_sec)
            {
                time_between_turns_cur_sec = time_between_turns_min_sec;
            }
        }

        int scoreAdjustmentsRemaining{ 0 };
        int scoreCurrent{ 0 };

        std::size_t food_eaten_count{ 0 };
        std::size_t tail_growth_per_food_count{ 5 };
        std::size_t tail_pieces_to_grow_remaining{ 0 };

        float time_between_turns_min_sec{ 0.025f };
        float time_between_turns_max_sec{ 0.1f };
        float time_between_turns_cur_sec{ time_between_turns_max_sec };
        float time_between_turns_shrink_ratio{ 0.96f };
    };
} // namespace boardgame

#endif // BOARDGAME_SETTINGS_HPP_INCLUDED