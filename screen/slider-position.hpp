#ifndef SLIDER_POSITION_HPP_INCLUDED
#define SLIDER_POSITION_HPP_INCLUDED
//
//  slider-position.hpp
//
#include "slider-base.hpp"

#include <SFML/System/Vector2.hpp>

namespace util
{

    // Uses the smooth motion slider method to oscillate a position between two points.
    class PosSlider : public SliderBase
    {
      public:
        // default constructor has all zero values and is stopped
        PosSlider()
            : SliderBase()
            , from_(0.0f, 0.0f)
            , to_(0.0f, 0.0f)
            , value_(0.0f, 0.0f)
        {}

        PosSlider(
            const sf::Vector2f & FROM,
            const sf::Vector2f & TO,
            const float SPEED,
            const WillOscillate WILL_OSCILLATE,
            const WillAutoStart WILL_AUTO_START,
            const std::size_t CYCLE_COUNT_LIMIT = 0)
            : SliderBase(SPEED, WILL_OSCILLATE, WILL_AUTO_START, CYCLE_COUNT_LIMIT)
            , from_(FROM)
            , to_(TO)
            , value_(FROM)
        {}

        virtual ~PosSlider() = default;

        bool isAtFrom() const override { return isEqual(value_, from_); }
        bool isAtTo() const override { return isEqual(value_, to_); }

        const sf::Vector2f from() const { return from_; }
        const sf::Vector2f to() const { return to_; }
        const sf::Vector2f value() const { return value_; }

        const sf::Vector2f updateAndReturnValue(const float ADJUSTMENT)
        {
            update(ADJUSTMENT);
            return value();
        }

      protected:
        bool updateImpl() override
        {
            const auto VALUE_BEFORE{ value_ };

            const auto RATIO{ positionRatio() };

            const auto HORIZ_DIFF{ static_cast<float>(to().x - from().x) };
            const auto VERT_DIFF{ static_cast<float>(to().y - from().y) };

            const auto NEW_POS_LEFT{ static_cast<float>(from().x) + (HORIZ_DIFF * RATIO) };
            const auto NEW_POS_TOP{ static_cast<float>(from().y) + (VERT_DIFF * RATIO) };

            value_ = sf::Vector2f(NEW_POS_LEFT, NEW_POS_TOP);

            return (isEqual(VALUE_BEFORE, value_) == false);
        }

        void setFromImpl() override { value_ = from_; }
        void setToImpl() override { value_ = to_; }

      protected:
        bool isEqual(const sf::Vector2f & LHS, const sf::Vector2f & RHS) const
        {
            return (isRealClose(LHS.x, RHS.x) && isRealClose(LHS.y, RHS.y));
        }

        sf::Vector2f from_;
        sf::Vector2f to_;
        sf::Vector2f value_;
    };

} // namespace util

#endif // SLIDER_POSITION_HPP_INCLUDED
