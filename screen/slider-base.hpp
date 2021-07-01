#ifndef SLIDER_BASE_HPP_INCLUDED
#define SLIDER_BASE_HPP_INCLUDED
//
// slider-base.hpp
//
#include "sliders.hpp"

namespace util
{

    enum class WillOscillate : bool
    {
        No = false,
        Yes = true
    };

    enum class WillAutoStart : bool
    {
        No = false,
        Yes = true
    };

    struct Moving
    {
        enum Enum : unsigned
        {
            Still = 0,
            Toward,
            Away,
            Count
        };

        static constexpr bool IsMoving(const Moving::Enum ENUM) noexcept
        {
            return ((ENUM == Toward) || (ENUM == Away));
        }
    };

    // Common base class for all non-value Sliders of non-value/arithmetic types.
    //
    // Implement all pure virtual functions exactly as specified in their comments.  Your
    // derived class simply needs three members variables: from_, to_, and value_, and two
    // member functions:
    //      T Value() const { return value_; }
    //  and
    //      T UpdateAndReturnValue(const float ADJUSTMENT) { Update(); return Value(); }
    //
    // Just setup the constructor values and call any of the Update() functions repeatedly.
    // Value() will move smoothly back and forth over the interval [FROM, TO], and that is
    // inclusive so you will always reach both end-point values.  The direction will take care
    // of itself and always be correct.  For more about when the direction changes, see the
    // comments below.
    //
    // If IsAtFrom() then IsMovingToward() always returns true and Direction() always returns
    // Toward, and conversely, if IsAtTo() then IsMovingToward() always returns false and
    // Direction() always returns Away. This is why the ReverseDirection() and SetDirection()
    // functions might do nothing and return false.
    class SliderBase
    {
      protected:
        // default constructor is all zero and false values and stopped
        SliderBase()
            : isStopped_(true)
            , isMovingToward_(true)
            , speed_(0.0f)
            , cycleCount_(0)
            , cycleCountLimit_(0)
            , willOscillate_(WillOscillate::No)
            , sliderZeroToOne_(1.0f)
        {}

        SliderBase(
            const float SPEED,
            const WillOscillate WILL_OSCILLATE,
            const WillAutoStart WILL_AUTO_START,
            const std::size_t CYCLE_COUNT_LIMIT)
            : isStopped_(((WILL_AUTO_START == WillAutoStart::Yes) ? false : true))
            , isMovingToward_(true)
            , speed_(SPEED)
            , cycleCount_(0)
            , cycleCountLimit_(CYCLE_COUNT_LIMIT)
            , willOscillate_(WILL_OSCILLATE)
            , sliderZeroToOne_(SPEED)
        {}

      public:
        virtual ~SliderBase() = default;

        float speed() const { return speed_; }
        std::size_t cycleCount() const { return cycleCount_; }
        std::size_t cycleCountLimit() const { return cycleCountLimit_; }

        bool cycleCountLimitReached() const
        {
            return ((cycleCountLimit_ > 0) && (cycleCount_ >= cycleCountLimit_));
        }

        virtual bool isAtFrom() const = 0;
        virtual bool isAtTo() const = 0;

        bool isAtFromOrTo() const { return (isAtFrom() || isAtTo()); }

        // returns [0,1] of Value()'s position within [FROM, TO]
        float positionRatio() const
        {
            if (isMovingToward())
            {
                return sliderZeroToOne_.value();
            }
            else
            {
                return (1.0f - sliderZeroToOne_.value());
            }
        }

        // returns true if the NEXT Update() will move toward TO, otherwise returns false
        bool isMovingToward() const { return isMovingToward_; }

        // returns only Toward or Away based on IsMovingToward()
        Moving::Enum direction() const
        {
            return ((isMovingToward()) ? Moving::Toward : Moving::Away);
        }

        // If IsStopped() returns Still otherwise returns Direction()
        Moving::Enum moving() const { return ((isStopped()) ? Moving::Still : direction()); }

        bool willOscilate() const { return (WillOscillate::Yes == willOscillate_); }

        void willOscilate(const bool WILL_OSCILLATE)
        {
            willOscillate_ = ((WILL_OSCILLATE) ? WillOscillate::Yes : WillOscillate::No);
        }

        void start() { isStopped_ = false; }
        void stop() { isStopped_ = true; }
        bool isStopped() const { return isStopped_; }
        bool isMoving() const { return (isStopped() == false); }

        // returns true if value_ changed
        bool update(const float ADJUSTMENT)
        {
            if (isStopped())
            {
                return false;
            }

            sliderZeroToOne_.update(ADJUSTMENT);

            const bool DID_SLIDER_STOP{ sliderZeroToOne_.isStopped() };
            const auto DID_VALUE_CHANGE{ updateImpl() };

            update_HandleValueChangeOrSliderStop(DID_VALUE_CHANGE, DID_SLIDER_STOP);

            return DID_VALUE_CHANGE;
        }

        bool updateAndReturnTrueIfValueChanged(const float ADJUSTMENT)
        {
            return update(ADJUSTMENT);
        }

        bool updateAndReturnIsMovingToward(const float ADJUSTMENT)
        {
            update(ADJUSTMENT);
            return isMovingToward();
        }

        bool updateAndReturnTrueIfIsMovingTowardChanged(const float ADJUSTMENT)
        {
            const auto IS_MOVING_TOWARD_BEFORE{ isMovingToward() };
            update(ADJUSTMENT);
            return (IS_MOVING_TOWARD_BEFORE != isMovingToward());
        }

        Moving::Enum updateAndReturnDirection(const float ADJUSTMENT)
        {
            update(ADJUSTMENT);
            return direction();
        }

        bool updateAndReturnTrueIfDirectionChanged(const float ADJUSTMENT)
        {
            const auto DIRECTION_BEFORE{ direction() };
            update(ADJUSTMENT);
            return (DIRECTION_BEFORE != direction());
        }

        bool updateAndReturnIsMoving(const float ADJUSTMENT)
        {
            update(ADJUSTMENT);
            return isMoving();
        }

        bool updateAndReturnTrueIfIsMovingChanged(const float ADJUSTMENT)
        {
            const auto IS_MOVING_BEFORE{ isMoving() };
            update(ADJUSTMENT);
            return (IS_MOVING_BEFORE != isMoving());
        }

        bool updateAndReturnIsStopped(const float ADJUSTMENT)
        {
            update(ADJUSTMENT);
            return isStopped();
        }

        bool updateAndReturnTrueIfIsStoppedChanged(const float ADJUSTMENT)
        {
            const auto IS_STOPPED_BEFORE{ isStopped() };
            update(ADJUSTMENT);
            return (IS_STOPPED_BEFORE != isStopped());
        }

        float updateAndReturnPositionRatio(const float ADJUSTMENT)
        {
            update(ADJUSTMENT);
            return positionRatio();
        }

        bool updateAndReturnTrueIfPositionRatioChanged(const float ADJUSTMENT)
        {
            const auto RATIO_BEFORE{ positionRatio() };
            update(ADJUSTMENT);
            return (isRealClose(RATIO_BEFORE, positionRatio()) == false);
        }

        // sets value_=from_ and isMoveToward_=true
        void setFrom()
        {
            isMovingToward_ = true;
            sliderZeroToOne_ = SliderZeroToOne(speed());
            setFromImpl();
        }

        // sets value_=to_ and isMoveToward_=false
        void setTo()
        {
            isMovingToward_ = false;
            sliderZeroToOne_ = SliderZeroToOne(speed());
            setToImpl();
        }

        void reset() { resetFrom(); }

        void resetCycleCount() { cycleCount_ = 0; }

        void resetFrom()
        {
            setFrom();
            resetCycleCount();
            start();
        }

        // calls ResetCycleCount(), SetTo(), and Start()
        void resetTo()
        {
            resetCycleCount();
            setTo();
            start();
        }

        // if IsMovingToward() then calls SetDirectionAway() otherwise calls
        // SetDirectionToward()
        bool reverseDirection()
        {
            if (isMovingToward())
            {
                return setDirectionAway();
            }
            else
            {
                return setDirectionToward();
            }
        }

        // if IsAtTo() returns false, otherwise sets isMovingToward_=true and returns true
        bool setDirectionToward()
        {
            if (isAtTo())
            {
                return false;
            }
            else
            {
                isMovingToward_ = true;
                return true;
            }
        }

        // if IsAtFrom() returns false, otherwise sets isMovingToward_=false and returns true
        bool setDirectionAway()
        {
            if (isAtFrom())
            {
                return false;
            }
            else
            {
                isMovingToward_ = false;
                return true;
            }
        }

      protected:
        // calcs and sets the new adjusted value_, returns true is the new value is different
        virtual bool updateImpl() = 0;

        // sets value_ = from_
        virtual void setFromImpl() = 0;

        // sets value_ = to_
        virtual void setToImpl() = 0;

        // Only the Update() function calls these OnUpdateChange functions, and only when
        // Update() actually changes value_, and only after the Update function is finished and
        // all members have been changed to their final valid values, and each is only called
        // once per call of Update(), and only in this order they are declared in
        virtual void onUpdateChange() {}
        virtual void onUpdateChangeStop() {}
        virtual void onUpdateChangeReachesFrom() {}
        virtual void onUpdateChangeReachesTo() {}
        virtual void onUpdateChangeReachesCycleCountLimit() {}

      private:
        struct CallbackFlags
        {
            bool stopped = false;
            bool reached_from = false;
            bool reached_to = false;
            bool reached_cycle_limit = false;
        };

        void update_HandleValueChangeOrSliderStop(
            const bool DID_VALUE_CHANGE, const bool DID_SLIDER_STOP)
        {
            if ((DID_VALUE_CHANGE == false) && (DID_SLIDER_STOP == false))
            {
                return;
            }

            CallbackFlags callbackFlags;

            if ((isMovingToward() == false) && (isAtFrom() || DID_SLIDER_STOP))
            {
                callbackFlags = update_HandleValueChangeOrSliderStop_HandleReachingFrom();
            }
            else if (isMovingToward() && (isAtTo() || DID_SLIDER_STOP))
            {
                callbackFlags = update_HandleValueChangeOrSliderStop_HandleReachingTo();
            }

            update_HandleValueChangeOrSliderStop_HandleCallingOnChangeFunctions(callbackFlags);
        }

        const CallbackFlags update_HandleValueChangeOrSliderStop_HandleReachingFrom()
        {
            CallbackFlags flags;
            flags.reached_from = true;

            setFrom();

            ++cycleCount_;
            const auto HAS_CYCLE_COUNT_LIMIT_BEEN_REACHED{ cycleCountLimitReached() };

            flags.reached_cycle_limit = HAS_CYCLE_COUNT_LIMIT_BEEN_REACHED;

            const auto WILL_STOP{ (
                (willOscilate() == false) || HAS_CYCLE_COUNT_LIMIT_BEEN_REACHED) };

            if (WILL_STOP)
            {
                stop();
                flags.stopped = true;
            }

            return flags;
        }

        const CallbackFlags update_HandleValueChangeOrSliderStop_HandleReachingTo()
        {
            CallbackFlags flags;
            flags.reached_to = true;

            setTo();

            const auto WILL_STOP{ (willOscilate() == false) };
            if (WILL_STOP)
            {
                stop();
                flags.stopped = true;
            }

            return flags;
        }

        void update_HandleValueChangeOrSliderStop_HandleCallingOnChangeFunctions(
            const CallbackFlags & FLAGS)
        {
            onUpdateChange();

            if (FLAGS.stopped)
            {
                onUpdateChangeStop();
            }

            if (FLAGS.reached_from)
            {
                onUpdateChangeReachesFrom();
            }

            if (FLAGS.reached_to)
            {
                onUpdateChangeReachesTo();
            }

            if (FLAGS.reached_cycle_limit)
            {
                onUpdateChangeReachesCycleCountLimit();
            }
        }

        bool isStopped_;
        bool isMovingToward_;
        float speed_;
        std::size_t cycleCount_;
        std::size_t cycleCountLimit_;
        WillOscillate willOscillate_;
        SliderZeroToOne sliderZeroToOne_;
    };

} // namespace util

#endif // SLIDER_BASE_HPP_INCLUDED
