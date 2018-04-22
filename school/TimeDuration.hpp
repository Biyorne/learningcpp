#ifndef SCHOOL_TIMEDURATION_HPP_INCLUDED
#define SCHOOL_TIMEDURATION_HPP_INCLUDED

#include <string>

namespace school
{
    class TimeDuration
    {
    public:
        TimeDuration(const int HOUR, const int MINUTE, const int SECOND);

        int hour() const { return m_hour; }

        int minute() const { return m_minute; }

        int second() const { return m_second; }

        std::string toString() const;

    private:
        int m_hour;
        int m_minute;
        int m_second;
    };

    bool operator==(const TimeDuration & L, const TimeDuration & R);

    bool operator!=(const TimeDuration & L, const TimeDuration & R);
} // namespace school
#endif // SCHOOL_TIMEDURATION_HPP_INCLUDED
