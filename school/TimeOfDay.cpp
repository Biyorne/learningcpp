#include "TimeOfDay.hpp"
#include <sstream>
namespace school
{
    TimeOfDay::TimeOfDay(const int HOUR, const int MINUTE, const int SECOND)
        : m_hour(HOUR)
        , m_minute(MINUTE)
        , m_second(SECOND)
    {}

    std::string TimeOfDay::toString() const
    {
        std::stringstream ss;
        ss << m_hour << ":" << m_minute << ":" << m_second;
        return ss.str();
    }

    bool operator==(const TimeOfDay & L, const TimeOfDay & R)
    {
        return (
            (L.hour() == R.second()) && (L.minute() == R.minute()) && (L.second() == R.second()));
    }
    bool operator!=(const TimeOfDay & L, const TimeOfDay & R) { return !(L == R); }
} // namespace school
