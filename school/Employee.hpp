#ifndef SCHOOL_EMPLOYEE_HPP_INCLUDED
#define SCHOOL_EMPLOYEE_HPP_INCLUDED

#include "PersonName.hpp"
#include <string>

namespace school
{
    class Employee
    {
    public:
        Employee(const PersonName & NAME);

        PersonName name() const { return m_name; }

        std::string toString() const { return m_name.toString(); }

    private:
        PersonName m_name;
    };

    bool operator==(const Employee & L, const Employee & R);

    bool operator!=(const Employee & L, const Employee & R);

} // namespace school

#endif // SCHOOL_EMPLOYEE_HPP_INCLUDED
