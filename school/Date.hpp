#ifndef SCHOOL_DATE_HPP_INCLUDED
#define SCHOOL_DATE_HPP_INCLUDED

#include <string>

namespace school
{
    class Date
    {
    public:
        Date(const int YEAR, const int MONTH, const int DAY);

        int year() const { return m_year; }

        int month() const { return m_month; }

        int day() const { return m_day; }

        // YYYY::MM::DD
        std::string toString() const;

    private:
        int m_year;
        int m_month;
        int m_day;
    };

    bool operator==(const Date & L, const Date & R);

    bool operator!=(const Date & L, const Date & R);

} // namespace school
#endif // SCHOOL_DATE_HPP_INCLUDED
