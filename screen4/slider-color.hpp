#ifndef SLIDER_COLOR_HPP_INCLUDED
#define SLIDER_COLOR_HPP_INCLUDED
//
// slider-color.hpp
//
#include "slider-base.hpp"

#include <SFML/Graphics/Color.hpp>

namespace util
{

    // Uses the smooth motion slider method to oscillate between two colors.
    class ColorSlider : public SliderBase
    {
      public:
        // default constructor sets all colors transparent and is stopped
        ColorSlider()
            : SliderBase()
            , from_(sf::Color::Transparent)
            , to_(sf::Color::Transparent)
            , value_(sf::Color::Transparent)
        {}

        ColorSlider(
            const sf::Color & COLOR_FROM,
            const sf::Color & COLOR_TO,
            const float SPEED,
            const WillOscillate WILL_OSCILLATE,
            const WillAutoStart WILL_AUTO_START,
            const std::size_t CYCLE_COUNT_LIMIT = 0)
            : SliderBase(SPEED, WILL_OSCILLATE, WILL_AUTO_START, CYCLE_COUNT_LIMIT)
            , from_(COLOR_FROM)
            , to_(COLOR_TO)
            , value_(COLOR_FROM)
        {}

        virtual ~ColorSlider() = default;

        bool isAtFrom() const override { return (value_ == from_); }
        bool isAtTo() const override { return (value_ == to_); }

        const sf::Color from() const { return from_; }
        const sf::Color to() const { return to_; }
        const sf::Color value() const { return value_; }

        const sf::Color updateAndReturnValue(const float ADJUSTMENT)
        {
            update(ADJUSTMENT);
            return value();
        }

      protected:
        bool updateImpl() override
        {
            const auto VALUE_BEFORE{ value_ };

            const auto RATIO{ positionRatio() };

            const auto RED_DIFF{ static_cast<float>(to().r - from().r) };
            const auto GREEN_DIFF{ static_cast<float>(to().g - from().g) };
            const auto BLUE_DIFF{ static_cast<float>(to().b - from().b) };
            const auto ALPHA_DIFF{ static_cast<float>(to().a - from().a) };

            const auto RED{ static_cast<float>(from().r) + (RED_DIFF * RATIO) };
            const auto GREEN{ static_cast<float>(from().g) + (GREEN_DIFF * RATIO) };
            const auto BLUE{ static_cast<float>(from().b) + (BLUE_DIFF * RATIO) };
            const auto ALPHA{ static_cast<float>(from().a) + (ALPHA_DIFF * RATIO) };

            value_ = sf::Color(
                static_cast<sf::Uint8>(RED),
                static_cast<sf::Uint8>(GREEN),
                static_cast<sf::Uint8>(BLUE),
                static_cast<sf::Uint8>(ALPHA));

            return (VALUE_BEFORE != value_);
        }

        void setFromImpl() override { value_ = from_; }
        void setToImpl() override { value_ = to_; }

      protected:
        sf::Color from_;
        sf::Color to_;
        sf::Color value_;
    };

} // namespace util

#endif // SLIDER_COLOR_HPP_INCLUDED
