#ifndef SCHOOL_TIMEOFDAY_HPP_INCLUDED
#define SCHOOL_TIMEOFDAY_HPP_INCLUDED

#include <string>

namespace school
{
    class TimeOfDay
    {
    public:
        TimeOfDay(const int HOUR, const int MINUTE, const int SECOND);

        int hour() const { return m_hour; }

        int minute() const { return m_minute; }

        int second() const { return m_second; }

        std::string toString() const;

    private:
        int m_hour;
        int m_minute;
        int m_second;
    };

    bool operator==(const TimeOfDay & L, const TimeOfDay & R);

    bool operator!=(const TimeOfDay & L, const TimeOfDay & R);
} // namespace school
#endif // SCHOOL_TIMEOFDAY_HPP_INCLUDED
