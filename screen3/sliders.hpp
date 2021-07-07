#ifndef UTIL_SLIDERS_HPP_INCLUDED
#define UTIL_SLIDERS_HPP_INCLUDED
//
// sliders.hpp
//
#include "math-constants.hpp"
#include "util.hpp"

#include <algorithm>

namespace util
{
    // All Sliders start at the given initial value and change when Update() is called until they
    // reach their target value or until Stop() is called.  Once IsStopped() all Sliders will stay
    // stopped.  SliderZeroToOne and SliderFromTo will both Stop() once they reach their target
    // value.  SliderOscillator and SliderDrift will always keep moving and never stop on their own.

    // All sliders can be default constructed which will leave them with all zero members and
    // stopped without logging any errors.

    // Slides Value() from 0 to 1 smoothly using sine motion and then stops.
    //
    // If SPEED < zero then it will be changed to zero, Stop() will be called, and an error
    // will be logged.  If START_AT is not within [0, 1] then it will be clamped to that
    // interval and an error will be logged. If START_AT is not zero then the speed will not
    // start slow and smooth.  Motion is fastest (bounce-like) when START_AT=0.5.
    class SliderZeroToOne
    {
      public:
        SliderZeroToOne()
            : isStopped_(true)
            , speed_(0.0f)
            , value_(0.0f)
            , radiansFrom_(0.0f)
            , radiansTo_(0.0f)
            , radians_(0.0f)
        {}

        explicit SliderZeroToOne(const float SPEED, const float START_AT_ORIG = 0.0f)
            : isStopped_(false)
            , speed_(SPEED)
            , value_(std::clamp(START_AT_ORIG, 0.0f, 1.0f))
            , radiansFrom_(math::half_pi<float>)
            , radiansTo_(1.5f * math::pi<float>)
            , radians_(radiansFrom_ + (math::pi<float> * value_))
        {
            if (isRealClose(value_, 1.0f))
            {
                isStopped_ = true;
            }
        }

        virtual ~SliderZeroToOne() = default;

        float from() const { return 0.0f; }
        float to() const { return 1.0f; }
        float value() const { return value_; }
        float speed() const { return speed_; }
        void speed(const float NEW_SPEED) { speed_ = NEW_SPEED; }
        bool isStopped() const { return isStopped_; }
        bool isMoving() const { return !isStopped(); }
        void stop() { isStopped_ = true; }

        float update(const float ADJUSTMENT)
        {
            if (!isStopped_)
            {
                radians_ += (ADJUSTMENT * speed_);

                if (radians_ < radiansFrom_)
                {
                    radians_ = radiansFrom_;
                    value_ = 0.0f;
                }
                else if (isRealClose(radians_, radiansTo_) || (radians_ > radiansTo_))
                {
                    radians_ = radiansTo_;
                    value_ = 1.0f;
                    stop();
                }
                else
                {
                    value_ = static_cast<float>((2.0f - (sin(radians_) + 1.0f)) * 0.5f);
                    value_ = std::clamp(value_, 0.0f, 1.0f);
                }
            }

            return value_;
        }

      private:
        bool isStopped_;
        float speed_;
        float value_;
        float radiansFrom_;
        float radiansTo_;
        float radians_;
    };

    // Slides Value() over [FROM, TO] smoothly using sine motion and then stops.
    //
    // If SPEED <= zero then it will be changed to zero, Stop() will be called, and an error
    // will be logged.  FROM>TO is supported and not considered an error, however, FROM==TO is
    // not supported and is considered an error that will cause Stop() to be called and an error
    // to be logged.
    template <typename T>
    class SliderFromTo
    {
      public:
        SliderFromTo()
            : isStopped_(true)
            , from_(T(0))
            , to_(T(0))
            , max_(T(0))
            , min_(T(0))
            , diff_(0.0f)
            , speed_(0.0f)
            , value_(T(0))
            , sliderZeroToOne_()
        {}

        SliderFromTo(const T FROM, const T TO, const float SPEED)
            : isStopped_(false)
            , from_(FROM)
            , to_(TO)
            , max_(util::max(FROM, TO))
            , min_(util::min(FROM, TO))
            , diff_(static_cast<float>(TO - FROM))
            , speed_(SPEED)
            , value_(FROM)
            , sliderZeroToOne_(speed_)
        {
            if (isStopped_)
            {
                sliderZeroToOne_.stop();
            }
        }

        virtual ~SliderFromTo() = default;

        T from() const { return from_; }
        T to() const { return to_; }
        T value() const { return value_; }
        float speed() const { return speed_; }
        bool isStopped() const { return isStopped_; }
        bool isMoving() const { return !isStopped(); }
        void stop() { isStopped_ = true; }

        T update(const float ADJUSTMENT)
        {
            if (!isStopped_)
            {
                const float RATIO{ sliderZeroToOne_.update(ADJUSTMENT) };
                value_ = static_cast<T>(static_cast<float>(from_) + (diff_ * RATIO));
                value_ = std::clamp(value_, min_, max_);
                isStopped_ = sliderZeroToOne_.isStopped();
            }

            return value_;
        }

      private:
        bool isStopped_;
        T from_;
        T to_;
        T max_;
        T min_;
        float diff_;
        float speed_;
        T value_;
        SliderZeroToOne sliderZeroToOne_;
    };

    // Slides Value() back and forth over [FROM, TO] smoothly using sine motion.
    //
    // If SPEED <= zero then it will be changed to zero, Stop() will be called, and an error
    // will be logged.  Supports FROM<TO and FROM>TO, but if FROM==TO then Stop will be
    // called and an error will be logged. If START_AT is not within [0, 1] then it will be
    // clamped to that interval and an error will be logged. If START_AT is not zero then
    // the speed will not start slow and smooth.  Motion is fastest (bounce-like) when
    // START_AT=0.5.
    template <typename T>
    class SliderOscillator
    {
      public:
        SliderOscillator()
            : isStopped_(true)
            , from_(T(0))
            , to_(T(0))
            , speed_(0.0f)
            , value_(T(0))
            , sliderFromTo_()
        {}

        // Use this constructor to start Value() at FROM.
        SliderOscillator(const T FROM, const T TO, const float SPEED)
            : isStopped_(false)
            , from_(FROM)
            , to_(TO)
            , speed_(0.0f)
            , value_(0)
            , sliderFromTo_()
        {
            setup(FROM, TO, SPEED, FROM);
        }

        // Use this constructor if you want to specify the starting value.
        SliderOscillator(const T FROM, const T TO, const float SPEED, const T START_AT)
            : isStopped_(false)
            , from_(FROM)
            , to_(TO)
            , speed_(0.0f)
            , value_(0)
            , sliderFromTo_()
        {
            setup(FROM, TO, SPEED, START_AT);
        }

        virtual ~SliderOscillator() = default;

        T from() const { return from_; }
        T to() const { return to_; }
        float speed() const { return speed_; }
        T value() const { return value_; }
        bool isStopped() const { return isStopped_; }
        bool isMoving() const { return !isStopped(); }
        void stop() { isStopped_ = true; }

        T update(const float ADJUSTMENT)
        {
            if (!isStopped_)
            {
                value_ = sliderFromTo_.update(ADJUSTMENT);

                if (sliderFromTo_.isStopped())
                {
                    // restart the slave slider targeting the end opposite what was just reached.
                    //
                    // Why not just swap sliderFromTo_'s from/to?  -Because the first time the
                    // target is reached and sliderFromTo_ stops, it might have been the first time,
                    // which might have started with a custom START_AT, meaning that
                    // sliderFromTo_.From() might not be the same as from_, so we can't use
                    // sliderFromTo_.From() here.
                    if (isRealClose(sliderFromTo_.To(), to_))
                    {
                        sliderFromTo_ = SliderFromTo<T>(to_, from_, speed_);
                    }
                    else
                    {
                        sliderFromTo_ = SliderFromTo<T>(from_, to_, speed_);
                    }
                }
            }

            return value_;
        }

      private:
        void setup(const T FROM, const T TO, const float SPEED, const T START_AT_ORIG)
        {
            value_ = std::clamp(START_AT_ORIG, min(FROM, TO), max(FROM, TO));

            if (false == isStopped_)
            {
                // If StartAtClamp() set value_ to TO then start reversed
                if (isRealClose(value_, TO))
                {
                    sliderFromTo_ = SliderFromTo<T>(to_, from_, speed_);
                }
                else
                {
                    sliderFromTo_ = SliderFromTo<T>(value_, TO, speed_);
                }
            }
        }

        bool isStopped_;
        T from_;
        T to_;
        float speed_;
        T value_;
        SliderFromTo<T> sliderFromTo_;
    };

    // Slides Value() from it's starting point to a randomly selected target within
    // [VALUE_MIN, VALUE_MAX] smoothly using sine motion at a speed randomly chosen within
    // [SPEED_MIN, SPEED_MAX].  The speed is changed every time the target changes.
    //
    // It is not an error if VALUE_MIN>VALUE_MAX, however, if VALUE_MIN==VALUE_MAX then
    // Stop() will be called and an error will be logged.  It is not an error if
    // SPEED_MIN>SPEED_MAX or if SPEED_MIN=SPEED_MAX.  If VALUE_TO_START_AT or
    // VALUE_OF_FIRST_TARGET is not within [VALUE_MIN, VALUE_MAX] then it is clamped and
    // an error is logged.  If VALUE_TO_START_AT=VALUE_OF_FIRST_TARGET then
    // VALUE_OF_FIRST_TARGET will be changed to either VALUE_MIN or VALUE_MAX.
    template <typename T>
    class SliderDrift
    {
      public:
        SliderDrift()
            : isStopped_(true)
            , valueMin_(T(0))
            , valueMax_(T(0))
            , value_(T(0))
            , speedMin_(0.0f)
            , speedMax_(0.0f)
            , speed_(0.0f)
            , sliderFromTo_()
        {}

        SliderDrift(
            const T VALUE_MIN, const T VALUE_MAX, const float SPEED_MIN, const float SPEED_MAX)
            : isStopped_(false)
            , valueMin_(VALUE_MIN)
            , valueMax_(VALUE_MAX)
            , value_(VALUE_MIN)
            , speedMin_(SPEED_MIN)
            , speedMax_(SPEED_MAX)
            , speed_(SPEED_MIN)
            , sliderFromTo_()
        {
            const auto [RANDOM_VALUE_TO_START_AT, RANDOM_VALUE_OF_FIRST_TARGET] =
                RandomStartAndTargetValue(VALUE_MIN, VALUE_MAX);

            setup(RANDOM_VALUE_TO_START_AT, RANDOM_VALUE_OF_FIRST_TARGET);
        }

        // This constructor to set a specific starting value and target.
        SliderDrift(
            const T VALUE_MIN,
            const T VALUE_MAX,
            const float SPEED_MIN,
            const float SPEED_MAX,
            const T VALUE_TO_START_AT,
            const T VALUE_OF_FIRST_TARGET)
            : isStopped_(false)
            , valueMin_(VALUE_MIN)
            , valueMax_(VALUE_MAX)
            , value_(VALUE_MIN)
            , speedMin_(SPEED_MIN)
            , speedMax_(SPEED_MAX)
            , speed_(SPEED_MIN)
            , sliderFromTo_()
        {
            setup(VALUE_TO_START_AT, VALUE_OF_FIRST_TARGET);
        }

        virtual ~SliderDrift() = default;

        T valueMin() const { return valueMin_; }
        T valueMax() const { return valueMax_; }
        T value() const { return value_; }

        float speedMin() const { return speedMin_; }
        float speedMax() const { return speedMax_; }
        float speed() const { return speed_; }

        bool isStopped() const { return isStopped_; }
        bool isMoving() const { return !isStopped(); }
        void stop() { isStopped_ = true; }

        T update(const float ADJUSTMENT)
        {
            if (!isStopped_)
            {
                value_ = sliderFromTo_.update(ADJUSTMENT);

                if (sliderFromTo_.isStopped())
                {
                    sliderFromTo_ = SliderFromTo<T>(
                        value_,
                        randomValueWithinIntervalThatIsNot(valueMin_, valueMax_, value_),
                        randomSpeed());
                }
            }

            return value_;
        }

      private:
        float randomSpeed()
        {
            if (isRealClose(speedMin_, speedMax_))
            {
                return speedMin_;
            }
            else
            {
                return util::Random(min(speedMin_, speedMax_), max(speedMin_, speedMax_));
            }
        }

        T randomValueWithinIntervalThatIsNot(
            const T MIN_ORIG, const T MAX_ORIG, const T VALUE_TO_AVOID_ORIG) const
        {
            // convert to doubles because there is no random<T> yet...
            const double MIN_DOUBLE{ static_cast<double>(Min(MIN_ORIG, MAX_ORIG)) };
            const double MAX_DOUBLE{ static_cast<double>(Max(MIN_ORIG, MAX_ORIG)) };
            const T RANDOM_VALUE{ static_cast<T>(Random(MIN_DOUBLE, MAX_DOUBLE)) };

            if (isRealClose(RANDOM_VALUE, VALUE_TO_AVOID_ORIG))
            {
                // if RANDOM_VALUE=VALUE_TO_AVOID then pick either the min or max
                if (isRealClose(MIN_ORIG, VALUE_TO_AVOID_ORIG))
                {
                    return MAX_ORIG;
                }
                else
                {
                    return MIN_ORIG;
                }
            }
            else
            {
                return RANDOM_VALUE;
            }
        }

        // returns (RANDOM_VALUE_TO_START_AT, RANDOM_VALUE_OF_TARGET)
        const std::tuple<T, T> randomStartAndTargetValue(const T MIN_ORIG, const T MAX_ORIG)
        {
            const T MIN_FINAL{ Min(MIN_ORIG, MAX_ORIG) };
            const T MAX_FINAL{ Max(MIN_ORIG, MAX_ORIG) };

            const auto RANDOM_VALUE_TO_START_AT{ randomValueWithinIntervalThatIsNot(
                MIN_FINAL, MAX_FINAL, MIN_FINAL) };

            const auto RANDOM_VALUE_OF_TARGET{ randomValueWithinIntervalThatIsNot(
                MIN_FINAL, MAX_FINAL, RANDOM_VALUE_TO_START_AT) };

            return std::make_tuple(RANDOM_VALUE_TO_START_AT, RANDOM_VALUE_OF_TARGET);
        }

        void setup(const T VALUE_TO_START_AT, const T VALUE_OF_FIRST_TARGET)
        {
            value_ = VALUE_TO_START_AT;

            speed_ = randomSpeed();

            if (false == isStopped_)
            {
                sliderFromTo_ = SliderFromTo<T>(VALUE_TO_START_AT, VALUE_OF_FIRST_TARGET, speed_);
            }
        }

        bool isStopped_;
        T valueMin_;
        T valueMax_;
        T value_;
        float speedMin_;
        float speedMax_;
        float speed_;
        SliderFromTo<T> sliderFromTo_;
    };

} // namespace util

#endif // UTIL_SLIDERS_HPP_INCLUDED
