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

        float cell_size_window_ratio{ 0.025f };

        std::size_t food_eaten_count{ 0 };
        std::size_t tail_growth_per_eat_count{ 5 };

        float turn_duration_min_sec{ 0.05f };
        float turn_duration_max_sec{ 0.25f };
        float turn_duration_ratio_after_eat{ 0.9f };

        float period_duration_sec{ 1.0f };
    };
} // namespace boardgame

#endif // BOARDGAME_SETTINGS_HPP_INCLUDED