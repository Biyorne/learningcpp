#ifndef BOARDGAME_SETTINGS_HPP_INCLUDED
#define BOARDGAME_SETTINGS_HPP_INCLUDED
//
// settings.hpp
//
#include "types.hpp"

#include <filesystem>
#include <string>

#include <SFML/Graphics.hpp>

namespace boardgame
{
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
        bool is_self_test{ false };
    };

    //

    class SnakeGameSettings : public GameSettings
    {
      public:
        SnakeGameSettings() = default;
        virtual ~SnakeGameSettings() = default;

        void reset() override;
        void printStatus() const override;

        float timeBetweenTurnsSec() const { return turn_duration_sec; }

        bool isSnakeGrowing() const { return (tail_pieces_to_add_remaining > 0); }

        bool willRemoveTailPiece(const std::size_t id)
        {
            if (isSnakeGrowing() || has_tail_shrunk_this_turn || (id != tail_id_to_remove_next))
            {
                return false;
            }

            ++tail_id_to_remove_next;
            has_tail_shrunk_this_turn = true;
            return true;
        }

        void handleSnakeHeadTurnStart() { has_tail_shrunk_this_turn = false; }

        void handleSnakeHeadEat()
        {
            ++food_eaten_count;
            tail_pieces_to_add_remaining += tail_growth_per_eat_count;

            turn_duration_sec *= turn_duration_mult_after_eat;
            if (turn_duration_sec < min_turn_duration_sec)
            {
                turn_duration_sec = min_turn_duration_sec;
            }
        }

        std::size_t consumeNextTailId()
        {
            if (tail_pieces_to_add_remaining > 0)
            {
                --tail_pieces_to_add_remaining;
            }

            return tail_id_to_assign_next++;
        }

        float cell_size_window_ratio{ 0.01f };

      private:
        bool has_tail_shrunk_this_turn{ false };
        float min_turn_duration_sec{ 0.05f };
        float max_turn_duration_sec{ 0.05f };
        float turn_duration_sec{ max_turn_duration_sec };
        float turn_duration_mult_after_eat{ 0.975f };

        std::size_t food_eaten_count{ 0 };
        std::size_t tail_growth_per_eat_count{ 5 };

        std::size_t tail_id_to_assign_next{ 0 };
        std::size_t tail_id_to_remove_next{ 0 };
        std::size_t tail_pieces_to_add_remaining{ 0 };
    };
} // namespace boardgame

#endif // BOARDGAME_SETTINGS_HPP_INCLUDED