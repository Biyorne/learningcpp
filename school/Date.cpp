#include "Date.hpp"
#include <sstream>

namespace school
{

    Date::Date(const int YEAR, const int MONTH, const int DAY)
        : m_year(YEAR)
        , m_month(MONTH)
        , m_day(DAY)
    {}

    // YYYY::MM::DD
    std::string Date::toString() const
    {
        std::ostringstream ss;
        ss << m_year << ":" << m_month << ":" << m_day;
        return ss.str();
    }

    bool operator==(const Date & L, const Date & R)
    {
        if ((L.year() == R.year()) && (L.month() == R.month()) && (L.day() == R.day()))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool operator!=(const Date & L, const Date & R) { return !(L == R); }

} // namespace school
